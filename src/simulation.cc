#include "simulation.hh"
#include "simulation_end_condition.hh"

#include <list>

#include <iostream>

namespace
{
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
}

Simulation::Simulation(TrajectoryGeneratorFactory& traj_gen_facto)
  : traj_gen_facto_(traj_gen_facto)
  , trajs_()
{
}

Simulation::~Simulation()
{
}

SimulationTrajectory::
SimulationTrajectory(TrajectoryGeneratorFactory& traj_gen_facto,
		     SimulationEndCondition& end_cond)
  : Simulation(traj_gen_facto)
  , end_cond_(end_cond)
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
    this->trajs_.push_back(tg->generate());

    delete tg;
    ++cpt;
  }
}


SimulationDensity::
SimulationDensity(TrajectoryGeneratorFactory& traj_gen_facto,
		  SimulationEndCondition& end_cond,
		  unsigned ntrajs,
		  double DT,
		  unsigned tratio)
  : Simulation(traj_gen_facto)
  , end_cond_(end_cond)
  , ntrajs_(ntrajs)
  , DT_(DT)
  , tratio_(tratio)
{
}

SimulationDensity::~SimulationDensity()
{
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

  for (TrajectoryGenerator* tgen: new_trajs)
    delete tgen;

  alive_trajs.clear();
}


SimulationEmpirical::
SimulationEmpirical(TrajectoryGeneratorFactory& traj_gen_facto,
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
  NumberPointsEndCondition& end_cond = dynamic_cast<NumberPointsEndCondition&>
    (this->traj_gen_facto_.traj_end_facto().template_condition());
  FixedPointTrajectoryStartGenerator& start = dynamic_cast<FixedPointTrajectoryStartGenerator&>
    (this->traj_gen_facto_.traj_start());

  for (TrajectoryCharacsMap::const_iterator it = this->traj_characs_.begin();
       it != this->traj_characs_.end(); ++it)
  {
    for (std::vector<TrajectoryCharacs>::const_iterator jt = it->second.begin();
	 jt != it->second.end(); ++jt)
    {
      start.update_start_point(jt->p0);
      end_cond.update_max_npts(jt->npts);
      //std::cout << it->first << std::endl;
      try
      {
	TrajectoryGenerator* tg =
	  this->traj_gen_facto_.get(it->first * this->DT_);

	this->trajs_.push_back(tg->generate());
	delete tg;
      }
      catch (std::runtime_error& e)
      {
	std::cerr << e.what() << std::endl;
      }
    }
  }
}
