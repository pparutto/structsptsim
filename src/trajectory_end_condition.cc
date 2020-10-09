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

NumberPointsPoissonianEndCondition::
NumberPointsPoissonianEndCondition(std::poisson_distribution<int>& distrib,
				   std::mt19937_64& mt)
  : NumberPointsEndCondition(distrib(mt))
  , distrib_(distrib)
  , mt_(mt)
{
}

void
NumberPointsPoissonianEndCondition::reset()
{
  this->max_npts_ = this->distrib_(this->mt_);
}
