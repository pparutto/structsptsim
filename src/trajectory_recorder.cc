#include "trajectory_recorder.hh"

TrajectoryRecorder::
TrajectoryRecorder(double DT)
  : traj_()
  , DT_(DT)
  , cnt_(0)
{
}

void
TrajectoryRecorder::reset()
{
  this->traj_.clear();
  this->cnt_ = 0;
}


FullTrajectoryRecorder::
FullTrajectoryRecorder(double DT)
  : TrajectoryRecorder(DT)
{
}

void
FullTrajectoryRecorder::record(const Point& p)
{
  this->traj_.push_back(TimedPoint({this->cnt_ * this->DT_, p[0], p[1]}));
  ++this->cnt_;
}


SubsambleTrajectoryRecorder::
SubsambleTrajectoryRecorder(double DT, unsigned step)
  : TrajectoryRecorder(DT)
  , step_(step)
{
}

void
SubsambleTrajectoryRecorder::record(const Point& p)
{
  if (this->cnt_ % this->step_ == 0)
    this->traj_.push_back(TimedPoint({(this->cnt_ / this->step_) * this->DT_,
				      p[0], p[1]}));
  ++this->cnt_;
}
