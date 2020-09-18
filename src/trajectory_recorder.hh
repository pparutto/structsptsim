#ifndef TRAJECTORY_RECORDER_HH_
#define TRAJECTORY_RECORDER_HH_

# include <random>

# include "trajectory.hh"

class TrajectoryRecorder
{
public:
  TrajectoryRecorder(double DT);

  virtual void record(const Point& p) = 0;
  virtual void reset();

  const Trajectory& traj() const { return this->traj_; }

protected:
  Trajectory traj_;
  double DT_;
  unsigned cnt_;
};

class FullTrajectoryRecorder: public TrajectoryRecorder
{
public:
  FullTrajectoryRecorder(double DT);

  virtual void record(const Point& p) override;
};

class SubsambleTrajectoryRecorder: public TrajectoryRecorder
{
public:
  SubsambleTrajectoryRecorder(double DT, unsigned step);

  virtual void record(const Point& p) override;

protected:
  unsigned step_;
};

#endif /// !TRAJECTORY_RECORDER_HH
