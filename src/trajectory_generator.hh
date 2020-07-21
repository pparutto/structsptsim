#ifndef TRAJECTORY_GENERATOR_HH_
#define TRAJECTORY_GENERATOR_HH_

# include <random>

# include "motion.hh"
# include "trajectory_start_generator.hh"
# include "trajectory_end_condition.hh"
# include "trajectory_recorder.hh"


class TrajectoryGenerator
{
public:
  TrajectoryGenerator(TrajectoryStartGenerator& traj_start,
		      Motion& motion_model,
		      TrajectoryEndCondition& traj_end,
		      TrajectoryRecorder& traj_rec,
		      double dt);

  Trajectory generate();
  void reset();
protected:
  TrajectoryStartGenerator& traj_start_;
  Motion& motion_model_;
  TrajectoryEndCondition& traj_end_;
  TrajectoryRecorder& traj_rec_;
  double dt_;
};


#endif /// !TRAJECTORY_GENERATOR_HH
