#include "trajectory_end_condition.hh"

#include <iostream>
#include <cmath>

template <size_t N>
TimeEndCondition<N>::TimeEndCondition(double max_t)
  : max_t_(max_t)
{
}

template <size_t N>
bool
TimeEndCondition<N>::evaluate(const Trajectory<N>& traj)
{
  return traj.at(traj.size()-1)[0] >= this->max_t_;
}

template <size_t N>
TimeEndCondition<N>*
TimeEndCondition<N>::clone_reset()
{
  return new TimeEndCondition<N>(this->max_t_);
}


template <size_t N>
NumberPointsEndCondition<N>::NumberPointsEndCondition(unsigned max_npts)
  : max_npts_(max_npts)
{
}

template <size_t N>
bool
NumberPointsEndCondition<N>::evaluate(const Trajectory<N>& traj)
{
  return traj.size() >= this->max_npts_;
}

template <size_t N>
void
NumberPointsEndCondition<N>::update_max_npts(unsigned new_max)
{
  this->max_npts_ = new_max;
}

template <size_t N>
NumberPointsEndCondition<N>*
NumberPointsEndCondition<N>::clone_reset()
{
  return new NumberPointsEndCondition<N>(this->max_npts_);
}

template <size_t N>
NumberPointsExpEndCondition<N>::
NumberPointsExpEndCondition(std::exponential_distribution<double>& distrib,
			    std::mt19937_64& mt)
  : NumberPointsEndCondition<N>(round(distrib(mt)))
  , distrib_(distrib)
  , mt_(mt)
{
}

template <size_t N>
NumberPointsExpEndCondition<N>*
NumberPointsExpEndCondition<N>::clone_reset()
{
  return new NumberPointsExpEndCondition<N>(this->distrib_, this->mt_);
}

template <size_t N>
EscapeEndCondition<N>::
EscapeEndCondition(const Shape<N>& reg)
  : reg_(reg)
{
}

template <size_t N>
bool
EscapeEndCondition<N>::evaluate(const Trajectory<N>& traj)
{
  return !this->reg_.inside(to_point<N>(traj.at(traj.size() - 1)));
}

template <size_t N>
EscapeEndCondition<N>*
EscapeEndCondition<N>::clone_reset()
{
  return new EscapeEndCondition<N>(this->reg_);
}

template <size_t N>
EnterRegionEndCondition<N>::EnterRegionEndCondition(const Shape<N>& reg)
  : reg_(reg)
{
}

template <size_t N>
bool
EnterRegionEndCondition<N>::evaluate(const Trajectory<N>& traj)
{
  return this->reg_.inside(to_point<N>(traj.at(traj.size() - 1)));
}

template <size_t N>
EnterRegionEndCondition<N>*
EnterRegionEndCondition<N>::clone_reset()
{
  return new EnterRegionEndCondition<N>(this->reg_);
}

template <size_t N>
CompoundEndCondition<N>::
CompoundEndCondition(std::vector<TrajectoryEndCondition<N>*>& end_conds)
  : end_conds_(end_conds)
{
}

template <size_t N>
CompoundEndCondition<N>::~CompoundEndCondition()
{
  for (TrajectoryEndCondition<N>* end_cond: this->end_conds_)
    delete end_cond;
}

template <size_t N>
bool
CompoundEndCondition<N>::evaluate(const Trajectory<N>& traj)
{
  for (TrajectoryEndCondition<N>* end_cond: this->end_conds_)
    if (end_cond->evaluate(traj))
      return true;
  return false;
}

template <size_t N>
CompoundEndCondition<N>*
CompoundEndCondition<N>::clone_reset()
{
  std::vector<TrajectoryEndCondition<N>*> new_conds;
  for (TrajectoryEndCondition<N>* end_cond: this->end_conds_)
    new_conds.push_back(end_cond->clone_reset());
  return new CompoundEndCondition<N>(new_conds);
}


template <size_t N>
TrajectoryEndConditionFactory<N>::
TrajectoryEndConditionFactory(TrajectoryEndCondition<N>& template_condition)
  : template_condition_(template_condition)
{
}

template <size_t N>
TrajectoryEndCondition<N>*
TrajectoryEndConditionFactory<N>::get()
{
  return this->template_condition_.clone_reset();
}

template <size_t N>
TrajectoryEndCondition<N>&
TrajectoryEndConditionFactory<N>::template_condition()
{
  return this->template_condition_;
}


template class TimeEndCondition<2>;
template class NumberPointsEndCondition<2>;
template class NumberPointsExpEndCondition<2>;
template class EscapeEndCondition<2>;
template class EnterRegionEndCondition<2>;
template class CompoundEndCondition<2>;
template class TrajectoryEndConditionFactory<2>;

template class NumberPointsEndCondition<3>;
