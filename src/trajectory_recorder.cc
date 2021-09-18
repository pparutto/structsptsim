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
  this->traj_.push_back(TimedPoint<N>({this->t0_ + this->traj_.size() * this->DT_,
				    p[0], p[1]}));
}

template <size_t N>
TimedPoint<N>
FullTrajectoryRecorder<N>::last_simu_point() const
{
  assert(!this->traj_.empty());
  return this->traj_[this->traj_.size()-1];
}


template <size_t N>
SubsambleTrajectoryRecorder<N>::
SubsambleTrajectoryRecorder(double t0, double DT, unsigned step)
  : TrajectoryRecorder<N>(t0, DT)
  , step_(step)
  , cnt_(0)
  , last_simu_pt_({NAN, NAN, NAN})
{
}

template <size_t N>
SubsambleTrajectoryRecorder<N>*
SubsambleTrajectoryRecorder<N>::clone_reset(double t0) const
{
  return new SubsambleTrajectoryRecorder<N>(t0, this->DT_, this->step_);
}

template <size_t N>
void
SubsambleTrajectoryRecorder<N>::record(const Point<N>& p)
{
  TimedPoint<N> tp({this->t0_ + this->traj_.size() * this->DT_,
		    p[0], p[1]});
  this->last_simu_pt_ = tp;
  if (this->cnt_ % this->step_ == 0)
    this->traj_.push_back(tp);
  ++this->cnt_;
}

template <size_t N>
TimedPoint<N>
SubsambleTrajectoryRecorder<N>::last_simu_point() const
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
