#ifndef TRAJECTORY_RECORDER_HH_
#define TRAJECTORY_RECORDER_HH_

# include <random>

# include "trajectory.hh"

class TrajectoryRecorder
{
public:
  TrajectoryRecorder() = default;

  virtual void record(const TimedPoint& p) = 0;
  void reset();

  const Trajectory& traj() const { return this->traj_; }

protected:
  Trajectory traj_;
};

class FullTrajectoryRecorder: public TrajectoryRecorder
{
public:
  FullTrajectoryRecorder() = default;

  virtual void record(const TimedPoint& p) override;
};


#endif /// !TRAJECTORY_RECORDER_HH
