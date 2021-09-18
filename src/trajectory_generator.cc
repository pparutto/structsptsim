#include "trajectory_generator.hh"

#include <cassert>
#include <iostream>

#include "utils.hh"

template <size_t N>
TrajectoryGenerator<N>::
TrajectoryGenerator(TrajectoryStartGenerator<N>& traj_start,
		    Motion<N>& motion_model,
		    TrajectoryEndCondition<N>* traj_end,
		    TrajectoryRecorder<N>* traj_rec,
		    Collider<N>& collider)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_(traj_end)
  , traj_rec_(traj_rec)
  , collider_(collider)
{
}

template <size_t N>
TrajectoryGenerator<N>::
~TrajectoryGenerator()
{
  delete this->traj_end_;
  delete this->traj_rec_;
}

template <size_t N>
void
TrajectoryGenerator<N>::init()
{
  assert(this->traj_rec_->traj().empty());

  bool done = false;
  Point<N> p = zero<N> ();
  while (!done)
  {
    try
    {
      this->traj_start_.generate();
      for (int i = 0; i < 500 && this->collider_.outside(p); ++i)
	p = this->traj_start_.generate();
      done = true;
    }
    catch (std::runtime_error& e)
    {
      std::cerr << "Re-launching initial point generation" << std::endl;
    }
  }

  if (this->collider_.outside(p))
    throw std::runtime_error("Failed to generate a point in polygon");

  this->traj_rec_->record(p);
}

template <size_t N>
double
TrajectoryGenerator<N>::cur_t()
{
  return this->traj_rec_->traj()[this->traj_rec_->traj().size()-1][0];
}

template <size_t N>
bool
TrajectoryGenerator<N>::finished()
{
  return this->traj_end_->evaluate(this->traj_rec_->traj());
}

template <size_t N>
void
TrajectoryGenerator<N>::generate_step()
{
  assert(!this->traj_rec_->traj().empty());
  Point<N> p1 = to_point(this->traj_rec_->last_simu_point());
  Point<N> p2 = this->motion_model_.step_euler(p1);

  //std::cout << "p2 = " << p2[0] << " " << p2[1] << std::endl;

  if (this->collider_.outside(p2))
    p2 = this->collider_.collide(p1, p2);

  //std::cout << "p22 = " << p2[0] << " " << p2[1] << std::endl;

  this->traj_rec_->record(p2);
}

template <size_t N>
Trajectory<N>
TrajectoryGenerator<N>::get()
{
  return this->traj_rec_->traj();
}

template <size_t N>
Trajectory<N>
TrajectoryGenerator<N>::generate()
{
  this->init();

  while (!this->finished())
  {
    bool done = false;
    while (!done)
    {
      try
      {
	this->generate_step();
	done = true;
      }
      catch (std::exception& e)
      {
	std::cerr << e.what() << std::endl;
      }
    }
  }

  return this->get();
}

template <size_t N>
bool
TrajectoryGenerator<N>::subsample() const
{
  return this->motion_model_.subsample();
}

template <size_t N>
TrajectoryGeneratorFactory<N>::
TrajectoryGeneratorFactory(TrajectoryStartGenerator<N>& traj_start,
			   Motion<N>& motion_model,
			   TrajectoryEndConditionFactory<N>& traj_end_facto,
			   TrajectoryRecorderFactory<N>& traj_rec_facto,
			   Collider<N>& collider)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_facto_(traj_end_facto)
  , traj_rec_facto_(traj_rec_facto)
  , collider_(collider)
{
}

template <size_t N>
TrajectoryGenerator<N>*
TrajectoryGeneratorFactory<N>::get(double t0) const
{
  return new TrajectoryGenerator<N>(this->traj_start_, this->motion_model_,
				    this->traj_end_facto_.get(),
				    this->traj_rec_facto_.get(t0),
				    this->collider_);
}

template <size_t N>
TrajectoryStartGenerator<N>&
TrajectoryGeneratorFactory<N>::traj_start()
{
  return this->traj_start_;
}

template <size_t N>
TrajectoryEndConditionFactory<N>&
TrajectoryGeneratorFactory<N>::traj_end_facto()
{
  return this->traj_end_facto_;
}
