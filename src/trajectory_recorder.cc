#include "trajectory_recorder.hh"

#include <cmath>
#include <iostream>
#include <cassert>

TrajectoryRecorder::
TrajectoryRecorder(double t0, double DT)
  : t0_(t0)
  , DT_(DT)
  , traj_()
{
}

TimedPoint
TrajectoryRecorder::last_rec_point() const
{
  return this->traj_[this->traj_.size()-1];
}

FullTrajectoryRecorder::
FullTrajectoryRecorder(double t0, double DT)
  : TrajectoryRecorder(t0, DT)
{
}

FullTrajectoryRecorder*
FullTrajectoryRecorder::clone_reset(double t0) const
{
  return new FullTrajectoryRecorder(t0, this->DT_);
}

void
FullTrajectoryRecorder::record(const Point& p)
{
  this->traj_.push_back(TimedPoint({this->t0_ + this->traj_.size() * this->DT_,
				    p[0], p[1]}));
}

TimedPoint
FullTrajectoryRecorder::last_simu_point() const
{
  assert(!this->traj_.empty());
  return this->traj_[this->traj_.size()-1];
}



SubsambleTrajectoryRecorder::
SubsambleTrajectoryRecorder(double t0, double DT, unsigned step)
  : TrajectoryRecorder(t0, DT)
  , step_(step)
  , cnt_(0)
  , last_simu_pt_({NAN, NAN, NAN})
{
}

SubsambleTrajectoryRecorder*
SubsambleTrajectoryRecorder::clone_reset(double t0) const
{
  return new SubsambleTrajectoryRecorder(t0, this->DT_, this->step_);
}

void
SubsambleTrajectoryRecorder::record(const Point& p)
{
  TimedPoint tp({this->t0_ + this->traj_.size() * this->DT_,
		 p[0], p[1]});
  this->last_simu_pt_ = tp;
  if (this->cnt_ % this->step_ == 0)
    this->traj_.push_back(tp);
  ++this->cnt_;
}

TimedPoint
SubsambleTrajectoryRecorder::last_simu_point() const
{
  return this->last_simu_pt_;
}

TrajectoryRecorderFactory::
TrajectoryRecorderFactory(TrajectoryRecorder& recorder_template)
  : recorder_template_(recorder_template)
{
}

TrajectoryRecorder*
TrajectoryRecorderFactory::get(double t0)
{
  return this->recorder_template_.clone_reset(t0);
}
