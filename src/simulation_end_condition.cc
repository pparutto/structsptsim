#include "simulation_end_condition.hh"
#include "simulation.hh"

NumberTrajectoriesSimulationEndCondition::
NumberTrajectoriesSimulationEndCondition(unsigned max_ntrajs)
  : max_ntrajs_(max_ntrajs)
{
}

bool
NumberTrajectoriesSimulationEndCondition::evaluate(const Simulation& sim)
{
  return sim.trajs().size() >= this->max_ntrajs_;
}
