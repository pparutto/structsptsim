#include "simulation.hh"
#include "simulation_end_condition.hh"

#include <iostream>

Simulation::Simulation(TrajectoryGenerator& traj_gen,
		       SimulationEndCondition& end_cond)
  : traj_gen_(traj_gen)
  , end_cond_(end_cond)
  , trajs_()
{
}

void
Simulation::run()
{
  int cpt = 0;
  while (!this->end_cond_.evaluate(*this))
  {
    std::cout << cpt << std::endl;
    this->trajs_.push_back(this->traj_gen_.generate());
    this->traj_gen_.reset();
    ++cpt;
  }
}
