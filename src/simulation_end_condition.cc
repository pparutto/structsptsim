#include "simulation_end_condition.hh"
#include "simulation.hh"

#include <iostream>

template <size_t N>
SimulationEndCondition<N>::~SimulationEndCondition()
{
}


template <size_t N>
NumberTrajectoriesSimulationEndCondition<N>::
NumberTrajectoriesSimulationEndCondition(unsigned max_ntrajs)
  : max_ntrajs_(max_ntrajs)
{
}

template <size_t N>
NumberTrajectoriesSimulationEndCondition<N>::
~NumberTrajectoriesSimulationEndCondition()
{
}

template <size_t N>
bool
NumberTrajectoriesSimulationEndCondition<N>::evaluate(const Simulation<N>& sim)
{
  return sim.trajs().size() >= this->max_ntrajs_;
}


template <size_t N>
NumberFramesSimulationEndCondition<N>::
NumberFramesSimulationEndCondition(unsigned max_frames)
  : cur_frame_(0)
  , max_frames_(max_frames)
{
}

template <size_t N>
NumberFramesSimulationEndCondition<N>::~NumberFramesSimulationEndCondition()
{
}

template <size_t N>
bool
NumberFramesSimulationEndCondition<N>::evaluate(const Simulation<N>& sim)
{
  (void) sim;
  if (this->cur_frame_ >= this->max_frames_)
    return true;
  ++this->cur_frame_;
  return false;
}
