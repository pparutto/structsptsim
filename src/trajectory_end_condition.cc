#include "trajectory_end_condition.hh"

NumberPointsEndCondition::NumberPointsEndCondition(unsigned max_npts)
  : max_npts_(max_npts)
{
}

bool
NumberPointsEndCondition::evaluate(const Trajectory& traj)
{
  return traj.size() >= this->max_npts_;
}
