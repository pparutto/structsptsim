#include "simulation_end_condition.hh"
#include "simulation.hh"

#include <iostream>

SimulationEndCondition::~SimulationEndCondition()
{
}


NumberTrajectoriesSimulationEndCondition::
NumberTrajectoriesSimulationEndCondition(unsigned max_ntrajs)
  : max_ntrajs_(max_ntrajs)
{
}

NumberTrajectoriesSimulationEndCondition::
~NumberTrajectoriesSimulationEndCondition()
{
}

bool
NumberTrajectoriesSimulationEndCondition::evaluate(const Simulation& sim)
{
  return sim.trajs().size() >= this->max_ntrajs_;
}


NumberFramesSimulationEndCondition::
NumberFramesSimulationEndCondition(unsigned max_frames)
  : cur_frame_(0)
  , max_frames_(max_frames)
{
}

NumberFramesSimulationEndCondition::~NumberFramesSimulationEndCondition()
{
}

bool
NumberFramesSimulationEndCondition::evaluate(const Simulation& sim)
{
  (void) sim;
  if (this->cur_frame_ >= this->max_frames_)
    return true;
  ++this->cur_frame_;
  return false;
}
