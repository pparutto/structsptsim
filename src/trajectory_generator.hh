#ifndef TRAJECTORY_GENERATOR_HH_
#define TRAJECTORY_GENERATOR_HH_

# include <random>

# include "motion.hh"
# include "trajectory_start_generator.hh"
# include "trajectory_end_condition.hh"
# include "trajectory_recorder.hh"
# include "collider.hh"

class TrajectoryGenerator
{
public:
  TrajectoryGenerator(TrajectoryStartGenerator& traj_start,
		      Motion& motion_model,
		      TrajectoryEndCondition* traj_end,
		      TrajectoryRecorder* traj_rec,
		      Collider& collider);
  ~TrajectoryGenerator();

  void init();
  void generate_step();
  bool finished();
  double cur_t();
  Trajectory get();

  Trajectory generate();
protected:
  TrajectoryStartGenerator& traj_start_;
  Motion& motion_model_;
  TrajectoryEndCondition* traj_end_;
  TrajectoryRecorder* traj_rec_;
  Collider& collider_;
};

class TrajectoryGeneratorFactory
{
public:
  TrajectoryGeneratorFactory(TrajectoryStartGenerator& traj_start,
			     Motion& motion_model,
			     TrajectoryEndConditionFactory& traj_end_facto,
			     TrajectoryRecorderFactory& traj_rec_facto,
			     Collider& collider);

  TrajectoryGenerator* get(double t0) const;
protected:
  TrajectoryStartGenerator& traj_start_;
  Motion& motion_model_;
  TrajectoryEndConditionFactory& traj_end_facto_;
  TrajectoryRecorderFactory& traj_rec_facto_;
  Collider& collider_;
};


#endif /// !TRAJECTORY_GENERATOR_HH
