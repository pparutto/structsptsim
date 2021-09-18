#ifndef TRAJECTORY_RECORDER_HH_
#define TRAJECTORY_RECORDER_HH_

# include <random>

# include "trajectory.hh"

template <size_t N>
class TrajectoryRecorder
{
public:
  TrajectoryRecorder(double t0, double DT);
  virtual ~TrajectoryRecorder() {}

  virtual TrajectoryRecorder<N>* clone_reset(double t0) const = 0;
  virtual void record(const Point<N>& p) = 0;
  virtual TimedPoint<N> last_simu_point() const = 0;;

  TimedPoint<N> last_rec_point() const;

  const Trajectory<N>& traj() const { return this->traj_; }

protected:
  double t0_;
  double DT_;
  Trajectory<N> traj_;
};

template <size_t N>
class FullTrajectoryRecorder: public TrajectoryRecorder<N>
{
public:
  FullTrajectoryRecorder(double t0, double DT);
  virtual ~FullTrajectoryRecorder() {}

  virtual FullTrajectoryRecorder* clone_reset(double t0) const;
  virtual void record(const Point<N>& p) override;
  virtual TimedPoint<N> last_simu_point() const;
};

template <size_t N>
class SubsambleTrajectoryRecorder: public TrajectoryRecorder<N>
{
public:
  SubsambleTrajectoryRecorder(double t0, double DT, unsigned step);

  virtual SubsambleTrajectoryRecorder<N>* clone_reset(double t0) const;
  virtual void record(const Point<N>& p) override;
  virtual TimedPoint<N> last_simu_point() const;
protected:
  unsigned step_;
  unsigned cnt_;
  TimedPoint<N> last_simu_pt_;
};

template <size_t N>
class TrajectoryRecorderFactory
{
public:
  TrajectoryRecorderFactory(TrajectoryRecorder<N>& recorder_template);
  virtual ~TrajectoryRecorderFactory() {}

  TrajectoryRecorder<N>* get(double t0);
protected:
  TrajectoryRecorder<N>& recorder_template_;
};

#endif /// !TRAJECTORY_RECORDER_HH
