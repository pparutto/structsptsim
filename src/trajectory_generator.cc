#include "trajectory_generator.hh"

#include <cassert>
#include <iostream>

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
  Point p2 = {NAN, NAN};
  while (!traj_end_.evaluate(this->traj_rec_.traj()))
  {
    p2 = this->motion_model_.step_euler(p1);

    if (this->collider_.outside(p2))
    {

      Point tmp = p2;

      p2 = this->collider_.collide(p1, p2);

      if (p2 == (Point) {0, 0})
      {
	for (const TimedPoint& pt: this->traj_rec_.traj())
	  std::cout << pt[0] << " " << pt[1] << " " << pt[2] << " " << this->collider_.outside({pt[1], pt[2]}) << std::endl;
	std::cout << tmp[0] << " " << tmp[1] << " " << this->collider_.outside(tmp) << std::endl;
	assert(false);
      }
    }

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
