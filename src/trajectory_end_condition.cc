#include "trajectory_end_condition.hh"

#include <iostream>
#include <cmath>

NumberPointsEndCondition::NumberPointsEndCondition(unsigned max_npts)
  : max_npts_(max_npts)
{
}

bool
NumberPointsEndCondition::evaluate(const Trajectory& traj)
{
  return traj.size() >= this->max_npts_;
}

void
NumberPointsEndCondition::update_max_npts(unsigned new_max)
{
  this->max_npts_ = new_max;
}

NumberPointsEndCondition*
NumberPointsEndCondition::clone_reset()
{
  return new NumberPointsEndCondition(this->max_npts_);
}

NumberPointsExpEndCondition::
NumberPointsExpEndCondition(std::exponential_distribution<double>& distrib,
			    std::mt19937_64& mt)
  : NumberPointsEndCondition(round(distrib(mt)))
  , distrib_(distrib)
  , mt_(mt)
{
}

NumberPointsExpEndCondition*
NumberPointsExpEndCondition::clone_reset()
{
  return new NumberPointsExpEndCondition(this->distrib_, this->mt_);
}

EscapeEndCondition::
EscapeEndCondition(const Shape& reg)
  : reg_(reg)
{
}

bool
EscapeEndCondition::evaluate(const Trajectory& traj)
{
  return !this->reg_.inside(to_point(traj.at(traj.size() - 1)));
}

EscapeEndCondition*
EscapeEndCondition::clone_reset()
{
  return new EscapeEndCondition(this->reg_);
}

CompoundEndCondition::
CompoundEndCondition(std::vector<TrajectoryEndCondition*>& end_conds)
  : end_conds_(end_conds)
{
}

CompoundEndCondition::~CompoundEndCondition()
{
  for (TrajectoryEndCondition* end_cond: this->end_conds_)
    delete end_cond;
}


bool
CompoundEndCondition::evaluate(const Trajectory& traj)
{
  for (TrajectoryEndCondition* end_cond: this->end_conds_)
    if (end_cond->evaluate(traj))
      return true;
  return false;
}

CompoundEndCondition*
CompoundEndCondition::clone_reset()
{
  std::vector<TrajectoryEndCondition*> new_conds;
  for (TrajectoryEndCondition* end_cond: this->end_conds_)
    new_conds.push_back(end_cond->clone_reset());
  return new CompoundEndCondition(new_conds);
}


TrajectoryEndConditionFactory::
TrajectoryEndConditionFactory(TrajectoryEndCondition& template_condition)
  : template_condition_(template_condition)
{
}

TrajectoryEndCondition*
TrajectoryEndConditionFactory::get()
{
  return this->template_condition_.clone_reset();
}

TrajectoryEndCondition&
TrajectoryEndConditionFactory::template_condition()
{
  return this->template_condition_;
}
