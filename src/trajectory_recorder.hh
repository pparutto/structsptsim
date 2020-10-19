#ifndef TRAJECTORY_RECORDER_HH_
#define TRAJECTORY_RECORDER_HH_

# include <random>

# include "trajectory.hh"

class TrajectoryRecorder
{
public:
  TrajectoryRecorder(double t0, double DT);
  virtual ~TrajectoryRecorder() {}

  virtual TrajectoryRecorder* clone_reset(double t0) const = 0;
  virtual void record(const Point& p) = 0;
  virtual TimedPoint last_simu_point() const = 0;;

  TimedPoint last_rec_point() const;

  const Trajectory& traj() const { return this->traj_; }

protected:
  double t0_;
  double DT_;
  Trajectory traj_;
};

class FullTrajectoryRecorder: public TrajectoryRecorder
{
public:
  FullTrajectoryRecorder(double t0, double DT);
  virtual ~FullTrajectoryRecorder() {}

  virtual FullTrajectoryRecorder* clone_reset(double t0) const;
  virtual void record(const Point& p) override;
  virtual TimedPoint last_simu_point() const;
};

class SubsambleTrajectoryRecorder: public TrajectoryRecorder
{
public:
  SubsambleTrajectoryRecorder(double t0, double DT, unsigned step);

  virtual SubsambleTrajectoryRecorder* clone_reset(double t0) const;
  virtual void record(const Point& p) override;
  virtual TimedPoint last_simu_point() const;
protected:
  unsigned step_;
  unsigned cnt_;
  TimedPoint last_simu_pt_;
};

class TrajectoryRecorderFactory
{
public:
  TrajectoryRecorderFactory(TrajectoryRecorder& recorder_template);
  virtual ~TrajectoryRecorderFactory() {}

  TrajectoryRecorder* get(double t0);
protected:
  TrajectoryRecorder& recorder_template_;
};

#endif /// !TRAJECTORY_RECORDER_HH
