#include "trajectory_recorder.hh"

#include <cmath>
#include <iostream>
#include <cassert>

template <size_t N>
TrajectoryRecorder<N>::
TrajectoryRecorder(double t0, double DT)
  : t0_(t0)
  , DT_(DT)
  , traj_()
{
}

template <size_t N>
TimedPoint<N>
TrajectoryRecorder<N>::last_rec_point() const
{
  return this->traj_[this->traj_.size()-1];
}

template <size_t N>
FullTrajectoryRecorder<N>::
FullTrajectoryRecorder(double t0, double DT)
  : TrajectoryRecorder<N>(t0, DT)
{
}

template <size_t N>
FullTrajectoryRecorder<N>*
FullTrajectoryRecorder<N>::clone_reset(double t0) const
{
  return new FullTrajectoryRecorder<N>(t0, this->DT_);
}

template <size_t N>
void
FullTrajectoryRecorder<N>::record(const Point<N>& p)
{
  this->traj_.push_back(to_timed_point<N>(this->t0_ + this->traj_.size() * this->DT_,
					  p));
}

template <size_t N>
TimedPoint<N>
FullTrajectoryRecorder<N>::last_simu_point() const
{
  assert(!this->traj_.empty());
  return this->traj_[this->traj_.size()-1];
}


template <size_t N>
SubsampleTrajectoryRecorder<N>::
SubsampleTrajectoryRecorder(double t0, double DT, unsigned step)
  : TrajectoryRecorder<N>(t0, DT)
  , step_(step)
  , cnt_(0)
  , last_simu_pt_(null_timed_point<N>())
{
}

template <size_t N>
SubsampleTrajectoryRecorder<N>*
SubsampleTrajectoryRecorder<N>::clone_reset(double t0) const
{
  return new SubsampleTrajectoryRecorder<N>(t0, this->DT_, this->step_);
}

template <size_t N>
void
SubsampleTrajectoryRecorder<N>::record(const Point<N>& p)
{
  TimedPoint<N> tp =
    to_timed_point<N>(this->t0_ + this->traj_.size() * this->DT_, p);

  //we need the last simu point in the collider
  this->last_simu_pt_ = tp;
  if (this->cnt_ % this->step_ == 0)
    this->traj_.push_back(tp);
  ++this->cnt_;
}

template <size_t N>
TimedPoint<N>
SubsampleTrajectoryRecorder<N>::last_simu_point() const
{
  return this->last_simu_pt_;
}

template <size_t N>
TrajectoryRecorderFactory<N>::
TrajectoryRecorderFactory(TrajectoryRecorder<N>& recorder_template)
  : recorder_template_(recorder_template)
{
}

template <size_t N>
TrajectoryRecorder<N>*
TrajectoryRecorderFactory<N>::get(double t0)
{
  return this->recorder_template_.clone_reset(t0);
}

template class FullTrajectoryRecorder<2>;
template class SubsampleTrajectoryRecorder<2>;
template class TrajectoryRecorderFactory<2>;

template class FullTrajectoryRecorder<3>;
