#ifndef TRAJECTORY_END_CONDITION_HH_
#define TRAJECTORY_END_CONDITION_HH_

# include <random>

# include "trajectory.hh"
# include "shape.hh"

class TrajectoryEndCondition
{
public:
  virtual ~TrajectoryEndCondition() = default;
  virtual bool evaluate(const Trajectory& traj) = 0;
  virtual TrajectoryEndCondition* clone_reset() = 0;
};

class NumberPointsEndCondition: public TrajectoryEndCondition
{
public:
  NumberPointsEndCondition(unsigned max_npts);
  ~NumberPointsEndCondition() {}

  virtual bool evaluate(const Trajectory& traj) override;
  virtual NumberPointsEndCondition* clone_reset() override;

protected:
  unsigned max_npts_;
};

class NumberPointsPoissonianEndCondition: public NumberPointsEndCondition
{
public:
  NumberPointsPoissonianEndCondition(std::poisson_distribution<int>& distrib,
				     std::mt19937_64& mt);
  ~NumberPointsPoissonianEndCondition() {}

  virtual NumberPointsPoissonianEndCondition* clone_reset() override;
protected:
  std::poisson_distribution<int> distrib_;
  std::mt19937_64& mt_;
};

class EscapeEndCondition: public TrajectoryEndCondition
{
public:
  EscapeEndCondition(const Shape& reg);
  ~EscapeEndCondition() {}

  virtual bool evaluate(const Trajectory& traj);
  virtual EscapeEndCondition* clone_reset() override;
protected:
  const Shape& reg_;
};

class CompoundEndCondition: public TrajectoryEndCondition
{
public:
  CompoundEndCondition(std::vector<TrajectoryEndCondition*>& end_conds);
  ~CompoundEndCondition();

  virtual bool evaluate(const Trajectory& traj);
  virtual CompoundEndCondition* clone_reset() override;
protected:
  std::vector<TrajectoryEndCondition*> end_conds_;
};

class TrajectoryEndConditionFactory
{
 public:
  TrajectoryEndConditionFactory(TrajectoryEndCondition& template_condition);

  TrajectoryEndCondition* get();
 protected:
  TrajectoryEndCondition& template_condition_;
};

#endif /// !TRAJECTORY_END_CONDITION_HH
