#ifndef TRAJECTORY_END_CONDITION_HH_
#define TRAJECTORY_END_CONDITION_HH_

# include <random>

# include "trajectory.hh"

class TrajectoryEndCondition
{
public:
  virtual ~TrajectoryEndCondition() = default;
  virtual bool evaluate(const Trajectory& traj) = 0;
  virtual void reset() = 0;
};

class NumberPointsEndCondition: public TrajectoryEndCondition
{
public:
  NumberPointsEndCondition(unsigned max_npts);

  virtual bool evaluate(const Trajectory& traj) override;
  virtual void reset() override {};

protected:
  unsigned max_npts_;
};

class NumberPointsPoissonianEndCondition: public NumberPointsEndCondition
{
public:
  NumberPointsPoissonianEndCondition(std::poisson_distribution<int>& distrib,
				     std::mt19937_64& mt);

  virtual void reset() override;
protected:
  std::poisson_distribution<int> distrib_;
  std::mt19937_64& mt_;
};


#endif /// !TRAJECTORY_END_CONDITION_HH
