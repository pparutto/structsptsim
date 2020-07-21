#ifndef TRAJECTORY_END_CONDITION_HH_
#define TRAJECTORY_END_CONDITION_HH_

# include "trajectory.hh"

class TrajectoryEndCondition
{
public:
  virtual bool evaluate(const Trajectory& traj) = 0;
};

class NumberPointsEndCondition: public TrajectoryEndCondition
{
public:
  NumberPointsEndCondition(unsigned max_npts);

  virtual bool evaluate(const Trajectory& traj) override;

protected:
  unsigned max_npts_;
};


#endif /// !TRAJECTORY_END_CONDITION_HH
