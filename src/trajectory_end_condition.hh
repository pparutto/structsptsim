#ifndef TRAJECTORY_END_CONDITION_HH_
#define TRAJECTORY_END_CONDITION_HH_

# include <random>

# include "trajectory.hh"
# include "shape.hh"

template <size_t N>
class TrajectoryEndCondition
{
public:
  virtual ~TrajectoryEndCondition() = default;
  virtual bool evaluate(const Trajectory<N>& traj) = 0;
  virtual TrajectoryEndCondition<N>* clone_reset() = 0;
};

template <size_t N>
class TimeEndCondition: public TrajectoryEndCondition<N>
{
public:
  TimeEndCondition(double max_t);
  ~TimeEndCondition() {}

  virtual bool evaluate(const Trajectory<N>& traj) override;
  virtual TimeEndCondition<N>* clone_reset() override;
protected:
  double max_t_;
};

template <size_t N>
class NumberPointsEndCondition: public TrajectoryEndCondition<N>
{
public:
  NumberPointsEndCondition(unsigned max_npts);
  ~NumberPointsEndCondition() {}

  void update_max_npts(unsigned new_max);

  virtual bool evaluate(const Trajectory<N>& traj) override;
  virtual NumberPointsEndCondition<N>* clone_reset() override;
protected:
  unsigned max_npts_;
};

template <size_t N>
class NumberPointsExpEndCondition: public NumberPointsEndCondition<N>
{
public:
  NumberPointsExpEndCondition(std::exponential_distribution<double>& distrib,
			      std::mt19937_64& mt);
  ~NumberPointsExpEndCondition() {}

  virtual NumberPointsExpEndCondition<N>* clone_reset() override;
protected:
  std::exponential_distribution<double> distrib_;
  std::mt19937_64& mt_;
};

template <size_t N>
class EscapeEndCondition: public TrajectoryEndCondition<N>
{
public:
  EscapeEndCondition(const Shape<N>& reg);
  ~EscapeEndCondition() {}

  virtual bool evaluate(const Trajectory<N>& traj);
  virtual EscapeEndCondition* clone_reset() override;
protected:
  const Shape<N>& reg_;
};

template <size_t N>
class EnterRegionEndCondition: public TrajectoryEndCondition<N>
{
public:
  EnterRegionEndCondition(const Shape<N>& reg);
  ~EnterRegionEndCondition() {}

  virtual bool evaluate(const Trajectory<N>& traj);
  virtual EnterRegionEndCondition<N>* clone_reset() override;
protected:
  const Shape<N>& reg_;
};


template <size_t N>
class CompoundEndCondition: public TrajectoryEndCondition<N>
{
public:
  CompoundEndCondition(std::vector<TrajectoryEndCondition<N>*>& end_conds);
  ~CompoundEndCondition();

  virtual bool evaluate(const Trajectory<N>& traj);
  virtual CompoundEndCondition* clone_reset() override;
protected:
  std::vector<TrajectoryEndCondition<N>*> end_conds_;
};

template <size_t N>
class TrajectoryEndConditionFactory
{
 public:
  TrajectoryEndConditionFactory(TrajectoryEndCondition<N>& template_condition);

  TrajectoryEndCondition<N>* get();

  TrajectoryEndCondition<N>& template_condition();
 protected:
  TrajectoryEndCondition<N>& template_condition_;
};

#endif /// !TRAJECTORY_END_CONDITION_HH
