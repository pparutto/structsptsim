#include "trajectory_generator.hh"

TrajectoryGenerator::TrajectoryGenerator(TrajectoryStartGenerator& traj_start,
					 Motion& motion_model,
					 TrajectoryEndCondition& traj_end,
					 TrajectoryRecorder& traj_rec,
					 Collider& collider,
					 double dt)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_(traj_end)
  , traj_rec_(traj_rec)
  , collider_(collider)
  , dt_(dt)
{
}

Trajectory
TrajectoryGenerator::generate()
{
  Point p1 = this->traj_start_.generate();
  this->traj_rec_.record(from_point(0, p1));

  double t = this->dt_;
  Point p2 = p1;
  while (!traj_end_.evaluate(this->traj_rec_.traj()))
  {
    p2 = this->motion_model_.step_euler(p1);

    if (this->collider_.outside(p2))
      p2 = this->collider_.collide(p1, p2);

    this->traj_rec_.record(from_point(t, p2));

    p1 = p2;
    t += this->dt_;
  }

  return this->traj_rec_.traj();
}

void
TrajectoryGenerator::reset()
{
  this->traj_rec_.reset();
}
