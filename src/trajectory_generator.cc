#include "trajectory_generator.hh"

#include <cassert>
#include <iostream>

#include "utils.hh"


TrajectoryGenerator::TrajectoryGenerator(TrajectoryStartGenerator& traj_start,
					 Motion& motion_model,
					 TrajectoryEndCondition* traj_end,
					 TrajectoryRecorder* traj_rec,
					 Collider& collider)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_(traj_end)
  , traj_rec_(traj_rec)
  , collider_(collider)
{
}

TrajectoryGenerator::
~TrajectoryGenerator()
{
  delete this->traj_end_;
  delete this->traj_rec_;
}

void
TrajectoryGenerator::init()
{
  assert(this->traj_rec_->traj().empty());
  this->traj_rec_->record(this->traj_start_.generate());
}

double
TrajectoryGenerator::cur_t()
{
  return this->traj_rec_->traj()[this->traj_rec_->traj().size()-1][0];
}

bool
TrajectoryGenerator::finished()
{
  return this->traj_end_->evaluate(this->traj_rec_->traj());
}

void
TrajectoryGenerator::generate_step()
{
  assert(!this->traj_rec_->traj().empty());
  Point p1 = to_point(this->traj_rec_->last_simu_point());
  Point p2 = this->motion_model_.step_euler(p1);

  if (this->collider_.outside(p2))
    p2 = this->collider_.collide(p1, p2);

  this->traj_rec_->record(p2);
}

Trajectory
TrajectoryGenerator::get()
{
  return this->traj_rec_->traj();
}

Trajectory
TrajectoryGenerator::generate()
{
  this->init();

  while (!this->finished())
    this->generate_step();

  return this->get();
}

TrajectoryGeneratorFactory::
TrajectoryGeneratorFactory(TrajectoryStartGenerator& traj_start,
			   Motion& motion_model,
			   TrajectoryEndConditionFactory& traj_end_facto,
			   TrajectoryRecorderFactory& traj_rec_facto,
			   Collider& collider)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_facto_(traj_end_facto)
  , traj_rec_facto_(traj_rec_facto)
  , collider_(collider)
{
}

TrajectoryGenerator*
TrajectoryGeneratorFactory::get(double t0) const
{
  return new TrajectoryGenerator(this->traj_start_, this->motion_model_,
				 this->traj_end_facto_.get(),
				 this->traj_rec_facto_.get(t0),
				 this->collider_);
}
