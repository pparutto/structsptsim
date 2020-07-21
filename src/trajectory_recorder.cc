#include "trajectory_recorder.hh"

void
TrajectoryRecorder::reset()
{
  this->traj_.clear();
}

void
FullTrajectoryRecorder::record(const TimedPoint& p)
{
  this->traj_.push_back(p);
}
