#include "trajectory_generator.hh"

#include <cassert>
#include <iostream>

#include "utils.hh"

TrajectoryGenerator::TrajectoryGenerator(TrajectoryStartGenerator& traj_start,
					 Motion& motion_model,
					 TrajectoryEndCondition& traj_end,
					 TrajectoryRecorder& traj_rec,
					 Collider& collider)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_(traj_end)
  , traj_rec_(traj_rec)
  , collider_(collider)
{
}

Trajectory
TrajectoryGenerator::generate()
{
  Point p1 = this->traj_start_.generate();
  //round_to_precision(p1);

  this->traj_rec_.record(p1);

  Point p2 = {NAN, NAN};
  while (!traj_end_.evaluate(this->traj_rec_.traj()))
  {
    p2 = this->motion_model_.step_euler(p1);
    //round_to_precision(p2);

    if (this->collider_.outside(p2))
    {
      p2 = this->collider_.collide(p1, p2);
      round_to_precision(p2);
    }

    this->traj_rec_.record(p2);

    p1 = p2;
  }

  return this->traj_rec_.traj();
}

void
TrajectoryGenerator::reset()
{
  this->traj_rec_.reset();
}
