#include "simulation.hh"
#include "simulation_end_condition.hh"

#include <list>
#include <iostream>

#include <cassert>

namespace
{
  template <size_t N>
  void
  generate_step_unstuck(TrajectoryGenerator<N>* tg)
  {
    bool done = false;
    while (!done)
    {
      try
      {
	tg->generate_step();
	done = true;
      }
      catch (std::exception& e)
      {
	std::cerr << "Exception " << e.what() << std::endl;
      }
    }
  }
}

template <size_t N>
Simulation<N>::Simulation(TrajectoryGeneratorFactory<N>& traj_gen_facto)
  : traj_gen_facto_(traj_gen_facto)
  , trajs_()
{
}

template <size_t N>
Simulation<N>::~Simulation()
{
}

template <size_t N>
void
Simulation<N>::shift_trajs_coords(const Point<N>& shifts)
{
  for (Trajectory<N>& tr: this->trajs_)
    shift_coords(tr, shifts);
}

template <size_t N>
SimulationTrajectory<N>::
SimulationTrajectory(TrajectoryGeneratorFactory<N>& traj_gen_facto,
		     SimulationEndCondition<N>& end_cond)
  : Simulation<N>(traj_gen_facto)
  , end_cond_(end_cond)
{
}

template <size_t N>
SimulationTrajectory<N>::~SimulationTrajectory()
{
}

template <size_t N>
void
SimulationTrajectory<N>::run()
{
  int cpt = 0;
  while (!this->end_cond_.evaluate(*this))
  {
    std::cout << cpt << std::endl;
    TrajectoryGenerator<N>* tg = this->traj_gen_facto_.get(0.0);
    this->trajs_.push_back(tg->generate());

    delete tg;
    ++cpt;
  }
}

template <size_t N>
SimulationDensity<N>::
SimulationDensity(TrajectoryGeneratorFactory<N>& traj_gen_facto,
		  SimulationEndCondition<N>& end_cond,
		  unsigned ntrajs,
		  double DT,
		  unsigned tratio)
  : Simulation<N>(traj_gen_facto)
  , end_cond_(end_cond)
  , ntrajs_(ntrajs)
  , DT_(DT)
  , tratio_(tratio)
{
}

template <size_t N>
SimulationDensity<N>::~SimulationDensity()
{
}

template <size_t N>
void
SimulationDensity<N>::run()
{
  unsigned frame = 0;

  std::list<TrajectoryGenerator<N>*> alive_trajs;
  for (unsigned i = 0; i < this->ntrajs_; ++i)
  {
    alive_trajs.push_back(this->traj_gen_facto_.get(frame * this->DT_));
    alive_trajs.back()->init();
  }
  assert(alive_trajs.size() == this->ntrajs_);


  std::list<typename std::list<TrajectoryGenerator<N>*>::iterator> to_del;
  for (typename std::list<TrajectoryGenerator<N>*>::iterator it =
	 alive_trajs.begin(); it != alive_trajs.end(); ++it)
  {
    if ((*it)->finished())
      to_del.push_back(it);
  }

  //unsigned generated = 0;
  //unsigned ok = 0;
  std::list<TrajectoryGenerator<N>*> new_trajs;
  for (typename std::list<TrajectoryGenerator<N>*>::iterator del: to_del)
  {
    //bool has_gen = (*del)->has_generated();
    this->trajs_.push_back((*del)->get());
    delete (*del);
    alive_trajs.erase(del);

    new_trajs.push_back(this->traj_gen_facto_.get((frame + 1) * this->DT_));
    new_trajs.back()->init();
  }

  ++frame;
  while (!this->end_cond_.evaluate(*this))
  {
    std::cout << frame << std::endl;
    assert(alive_trajs.size() + new_trajs.size() == this->ntrajs_);
    //generated = 0;
    //ok = 0;

    for (TrajectoryGenerator<N>* tg: alive_trajs)
    {
      if (tg->subsample())
      {
	for (unsigned j = 0; j < this->tratio_; ++j)
	{
	  if (tg->finished())
	    break;
	  generate_step_unstuck(tg);
	}
      }
      else
	generate_step_unstuck(tg);
    }

    // unsigned tmp = 0;
    // for (TrajectoryGenerator<N>* tg: alive_trajs)
    // {
    //   if (tg->has_generated())
    //   {
    // 	if (tg->get().back()[0] != frame)
    // 	  std::cout << "b " << frame << " " << tg->get().back()[0] << " " << tg->get().back().size() << std::endl;
    //   	assert(tg->get().back()[0] == frame);
    // 	++tmp;
    //   }
    // }
    // std::cout << "b " << tmp << std::endl;

    alive_trajs.insert(alive_trajs.end(), new_trajs.begin(), new_trajs.end());
    new_trajs.clear();

    // for (TrajectoryGenerator<N>* tg: alive_trajs)
    // {
    //   generated += tg->has_generated();
    //   if (tg->has_generated())
    //   {
    // 	if (abs(tg->get().back()[0] - frame * 0.01) > 0.015)
    // 	  std::cout << frame * 0.01 << " " << tg->get().back()[0] << std::endl;
    //   	assert(abs(tg->get().back()[0] - frame * 0.01) < 0.015);
    // 	++ok;
    //   }
    // }

    // for (TrajectoryGenerator<N>* tg: alive_trajs)
    // {
    //   generated += tg->has_generated();
    //   if (tg->has_generated())
    //   {
    // 	if (tg->get().back()[0] != frame)
    // 	  std::cout << frame << " " << tg->get().back()[0] << std::endl;
    //   	assert(tg->get().back()[0] == frame);
    // 	++ok;
    //   }
    // }

    std::list<typename std::list<TrajectoryGenerator<N>*>::iterator> to_del;
    for (typename std::list<TrajectoryGenerator<N>*>::iterator it =
	   alive_trajs.begin(); it != alive_trajs.end(); ++it)
    {
      if ((*it)->finished())
	to_del.push_back(it);
    }

    for (typename std::list<TrajectoryGenerator<N>*>::iterator del: to_del)
    {
      //bool has_gen = (*del)->has_generated();
      this->trajs_.push_back((*del)->get());
      delete (*del);
      alive_trajs.erase(del);

      new_trajs.push_back(this->traj_gen_facto_.get((frame + 1) * this->DT_));
      new_trajs.back()->init();


      // if (has_gen)
      // {
      // 	new_trajs.push_back(this->traj_gen_facto_.get(frame + 1)); //) * this->DT_
      // 	new_trajs.back()->init();
      // 	assert(new_trajs.back()->has_generated());
      // }
      // else
      // {
      // 	std::cout << "AAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
      // 	alive_trajs.push_back(this->traj_gen_facto_.get(frame)); // * this->DT_
      // 	alive_trajs.back()->init();
      // 	generated += alive_trajs.back()->has_generated();
      // 	assert(alive_trajs.back()->has_generated());

      // 	// if (abs(alive_trajs.back()->get().back()[0] - frame * 0.01) > 0.015)
      // 	//   std::cout << frame * 0.01 << " " << alive_trajs.back()->get().back()[0] << std::endl;
      // 	// assert(abs(alive_trajs.back()->get().back()[0] - frame * 0.01) < 0.015);

      // 	if (alive_trajs.back()->get().back()[0] != frame)
      // 	  std::cout << frame << " " << alive_trajs.back()->get().back()[0] << std::endl;
      // 	assert(alive_trajs.back()->get().back()[0] == frame);

      // 	++ok;
      // }
    }

    // if (ok != this->ntrajs_)
    //   std::cout << ok << " " << this->ntrajs_ << std::endl;
    // assert(ok == this->ntrajs_);

    //assert(generated == this->ntrajs_);
    ++frame;
  }

  for (TrajectoryGenerator<N>* tgen: alive_trajs)
  {
    this->trajs_.push_back(tgen->get());
    delete tgen;
  }

  for (TrajectoryGenerator<N>* tgen: new_trajs)
    delete tgen;

  alive_trajs.clear();
}


SimulationEmpirical::
SimulationEmpirical(TrajectoryGeneratorFactory<2>& traj_gen_facto,
		    const TrajectoryCharacsMap& traj_characs,
		    double DT)
  : Simulation(traj_gen_facto)
  , traj_characs_(traj_characs)
  , DT_(DT)
{
}

SimulationEmpirical::
~SimulationEmpirical()
{
}

void
SimulationEmpirical::run()
{
  NumberPointsEndCondition<2>& end_cond = dynamic_cast<NumberPointsEndCondition<2>&>
    (this->traj_gen_facto_.traj_end_facto().template_condition());
  FixedPointTrajectoryStartGenerator<2>& start = dynamic_cast<FixedPointTrajectoryStartGenerator<2>&>
    (this->traj_gen_facto_.traj_start());

  for (TrajectoryCharacsMap::const_iterator it = this->traj_characs_.begin();
       it != this->traj_characs_.end(); ++it)
  {
    for (std::vector<TrajectoryCharacs>::const_iterator jt = it->second.begin();
	 jt != it->second.end(); ++jt)
    {
      start.update_start_point(jt->p0);
      end_cond.update_max_npts(jt->npts);
      try
      {
	TrajectoryGenerator<2>* tg =
	  this->traj_gen_facto_.get(it->first * this->DT_);

	this->trajs_.push_back(tg->generate());
	delete tg;
      }
      catch (std::exception& e)
      {
	std::cerr << "Exception : " << e.what() << std::endl;
      }
    }
  }
}


template class SimulationTrajectory<2>;
template class SimulationDensity<2>;
