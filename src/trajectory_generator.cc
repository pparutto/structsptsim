#include "trajectory_generator.hh"

TrajectoryGenerator::TrajectoryGenerator(TrajectoryStartGenerator& traj_start,
					 Motion& motion_model,
					 TrajectoryEndCondition& traj_end,
					 TrajectoryRecorder& traj_rec,
					 double dt)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_(traj_end)
  , traj_rec_(traj_rec)
  , dt_(dt)
{
}

Trajectory
TrajectoryGenerator::generate()
{
  Point p = this->traj_start_.generate();
  this->traj_rec_.record(from_point(0, p));

  double t = this->dt_;
  while (!traj_end_.evaluate(this->traj_rec_.traj()))
  {
    this->traj_rec_.record(from_point(t, this->motion_model_.step_euler(p)));
    t += this->dt_;
  }

  return this->traj_rec_.traj();
}

void
TrajectoryGenerator::reset()
{
  this->traj_rec_.reset();
}
