#include "simulation.hh"
#include "simulation_end_condition.hh"

#include <list>

#include <iostream>

Simulation::Simulation(const TrajectoryGeneratorFactory& traj_gen_facto,
		       SimulationEndCondition& end_cond)
  : traj_gen_facto_(traj_gen_facto)
  , end_cond_(end_cond)
  , trajs_()
{
}

Simulation::~Simulation()
{
}

SimulationTrajectory::
SimulationTrajectory(const TrajectoryGeneratorFactory& traj_gen_facto,
		     SimulationEndCondition& end_cond)
  : Simulation(traj_gen_facto, end_cond)
{
}

SimulationTrajectory::~SimulationTrajectory()
{
}

void
SimulationTrajectory::run()
{
  int cpt = 0;
  while (!this->end_cond_.evaluate(*this))
  {
    TrajectoryGenerator* tg = this->traj_gen_facto_.get(0.0);
    std::cout << cpt << std::endl;
    try
    {
      this->trajs_.push_back(tg->generate());
    }
    catch (std::runtime_error& e)
    {
      std::cout << e.what() << std::endl;
      std::cout << "Failed collision" << std::endl;
    }

    delete tg;
    ++cpt;
  }
}


SimulationDensity::
SimulationDensity(const TrajectoryGeneratorFactory& traj_gen_facto,
		  SimulationEndCondition& end_cond,
		  unsigned ntrajs,
		  double DT,
		  unsigned tratio)
  : Simulation(traj_gen_facto, end_cond)
  , ntrajs_(ntrajs)
  , DT_(DT)
  , tratio_(tratio)
{
}

SimulationDensity::~SimulationDensity()
{
}

void
generate_step_unstuck(TrajectoryGenerator* tg)
{
  bool done = false;
  while (!done)
  {
    try
    {
      tg->generate_step();
      done = true;
    }
    catch (std::runtime_error& e)
    {
      tg->generate_step();
    }
  }
}

void
SimulationDensity::run()
{
  std::list<TrajectoryGenerator*> alive_trajs;
  for (unsigned i = 0; i < this->ntrajs_; ++i)
  {
    alive_trajs.push_back(this->traj_gen_facto_.get(0.0));
    alive_trajs.back()->init();
  }

  int frame = 1;
  std::list<TrajectoryGenerator*> new_trajs;
  while (!this->end_cond_.evaluate(*this))
  {
    std::cout << frame << std::endl;
    for (TrajectoryGenerator* tg: alive_trajs)
    {
      if (tg->subsample())
      {
	for (unsigned j = 0; j < this->tratio_; ++j)
	  generate_step_unstuck(tg);
      }
      else
	generate_step_unstuck(tg);
    }

    alive_trajs.insert(alive_trajs.end(), new_trajs.begin(), new_trajs.end());
    new_trajs.clear();

    std::list<std::list<TrajectoryGenerator*>::iterator> to_del;
    for (std::list<TrajectoryGenerator*>::iterator it = alive_trajs.begin();
	 it != alive_trajs.end(); ++it)
      if ((*it)->finished())
	to_del.push_back(it);

    for (std::list<TrajectoryGenerator*>::iterator del: to_del)
    {
      this->trajs_.push_back((*del)->get());
      delete (*del);
      alive_trajs.erase(del);

      new_trajs.push_back(this->traj_gen_facto_.get((frame + 1) * this->DT_));
      new_trajs.back()->init();
    }

    //std::cout << alive_trajs.size() << std::endl;
    ++frame;
  }

  for (TrajectoryGenerator* tgen: alive_trajs)
  {
    this->trajs_.push_back(tgen->get());
    delete tgen;
  }
  alive_trajs.clear();
}
