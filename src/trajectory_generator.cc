#include "trajectory_generator.hh"

#include <cassert>
#include <iostream>

template <size_t N>
TrajectoryGenerationException<N>::
TrajectoryGenerationException(const CollisionException<N>& base_except,
			      std::string what)
  : base_except_(base_except)
  , what_(what)
{
}

template <size_t N>
const char*
TrajectoryGenerationException<N>::what() const noexcept
{
  return this->what_.c_str();
}


template <size_t N>
TrajectoryGenerator<N>::
TrajectoryGenerator(TrajectoryStartGenerator<N>& traj_start,
		    Motion<N>& motion_model,
		    TrajectoryEndCondition<N>* traj_end,
		    TrajectoryRecorder<N>* traj_rec,
		    Collider<N>& collider,
		    const Shape<N>* sim_reg,
		    Logger* log)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_(traj_end)
  , traj_rec_(traj_rec)
  , collider_(collider)
  , sim_reg_(sim_reg)
  , done_(false)
  , log_(log)
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
  Point<N> p = zero<N>();
  while (!done)
  {
    try
    {
      int i = 0;
      do
      {
	p = this->traj_start_.generate();
	p = round_to_precision<N>(p);
      }
      while (i < 500 && this->collider_.outside(p));
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
  if (!this->done_)
    this->done_ = this->traj_end_->evaluate(this->traj_rec_->traj());
  return this->done_;
}

template <size_t N>
void
TrajectoryGenerator<N>::generate_step()
{
  assert(!this->traj_rec_->traj().empty());

  Point<N> p1 = to_point<N>(this->traj_rec_->last_simu_point());
  Point<N> p2;

  unsigned cnt = 0;
  bool retry = true;
  CollisionException<N> last_e = CollisionException<N> ();
  while (retry)
  {
    p2 = this->motion_model_.step_euler(p1);

    try
    {
      this->collider_.collide(p1, p2, p2);
      retry = false;
    }
    catch (CollisionException<N>& e)
    {
      this->log_->write(e.what());
      last_e = e;
      std::cerr << "GenerateStep:" << std::string(e.what()) << std::endl;
    }

    ++cnt;
    if (retry && cnt > 50)
    {
      this->log_->write("Could not generate a valid successor");
      throw TrajectoryGenerationException<N> (last_e, "Next point generation failed");
    }
  }

  //make sure to record only the points inside the simulation region
  if (this->sim_reg_ == nullptr || this->sim_reg_->inside(p2))
    this->traj_rec_->record(p2);
  else
    this->done_ = true;
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
    unsigned cpt = 0;
    bool done = false;
    while (!done)
    {
      this->generate_step();
      done = true;
      if (cpt > 50)
	throw TrajectoryGenerationException<N> (CollisionException<N> (), "Next point generation failed");
      ++cpt;
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
			   Collider<N>& collider,
			   const Shape<N>* sim_reg,
			   Logger* log)
  : traj_start_(traj_start)
  , motion_model_(motion_model)
  , traj_end_facto_(traj_end_facto)
  , traj_rec_facto_(traj_rec_facto)
  , collider_(collider)
  , sim_reg_(sim_reg)
  , log_(log)
{
}

template <size_t N>
TrajectoryGenerator<N>*
TrajectoryGeneratorFactory<N>::get(double t0) const
{
  return new TrajectoryGenerator<N>(this->traj_start_, this->motion_model_,
				    this->traj_end_facto_.get(),
				    this->traj_rec_facto_.get(t0),
				    this->collider_, this->sim_reg_,
				    this->log_);
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


template class TrajectoryGenerationException<2>;
template class TrajectoryGenerator<2>;
template class TrajectoryGeneratorFactory<2>;


template class TrajectoryGenerator<3>;
