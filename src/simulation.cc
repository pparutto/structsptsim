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
  while (!this->end_cond_.evaluate(*this))
  {
    this->trajs_.push_back(this->traj_gen_.generate());
    this->traj_gen_.reset();
  }
}
