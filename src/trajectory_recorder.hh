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
  virtual void record_ncoll(unsigned ncoll) = 0;
  virtual TimedPoint<N> last_simu_point() const = 0;;

  TimedPoint<N> last_rec_point() const;
  bool has_generated() { return this->has_gen_; };

  virtual const Trajectory<N>& traj() const { return this->traj_; }

protected:
  double t0_;
  double DT_;
  Trajectory<N> traj_;
  bool has_gen_;
};

template <size_t N>
class FullTrajectoryRecorder: public TrajectoryRecorder<N>
{
public:
  FullTrajectoryRecorder(double t0, double DT);
  virtual ~FullTrajectoryRecorder() {}

  virtual FullTrajectoryRecorder* clone_reset(double t0) const;
  virtual void record(const Point<N>& p) override;
  virtual void record_ncoll(unsigned ncoll) override { (void) ncoll; };
  virtual TimedPoint<N> last_simu_point() const;
};

template <size_t N>
class SubsampleTrajectoryRecorder: public TrajectoryRecorder<N>
{
public:
  SubsampleTrajectoryRecorder(double t0, double DT, unsigned step);

  virtual SubsampleTrajectoryRecorder<N>* clone_reset(double t0) const;
  virtual void record(const Point<N>& p) override;
  virtual void record_ncoll(unsigned ncoll) override { (void) ncoll; };
  virtual TimedPoint<N> last_simu_point() const;
protected:
  unsigned step_;
  unsigned cnt_;
  TimedPoint<N> last_simu_pt_;
};

template <size_t N>
class CollTrajectoryRecorder: public TrajectoryRecorder<N>
{
public:
  CollTrajectoryRecorder(TrajectoryRecorder<N>* trec);
  ~CollTrajectoryRecorder();

  virtual CollTrajectoryRecorder<N>* clone_reset(double t0) const;
  virtual void record(const Point<N>& p) override;
  virtual void record_ncoll(unsigned ncoll) override;
  virtual TimedPoint<N> last_simu_point() const;

  bool has_generated() const;

  virtual const Trajectory<N>& traj() const override;
  const std::vector<unsigned>& ncolls() const { return this->ncolls_; };
protected:
  TrajectoryRecorder<N>* trec_;
  std::vector<unsigned> ncolls_;
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
