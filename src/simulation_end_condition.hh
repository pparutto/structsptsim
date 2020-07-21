#ifndef SIMULATION_END_CONDITION_HH_
#define SIMULATION_END_CONDITION_HH_

class Simulation;

class SimulationEndCondition
{
public:
  virtual bool evaluate(const Simulation& sim) = 0;
};

class NumberTrajectoriesSimulationEndCondition: public SimulationEndCondition
{
public:
  NumberTrajectoriesSimulationEndCondition(unsigned max_ntrajs);

  virtual bool evaluate(const Simulation& sim) override;
protected:
  unsigned max_ntrajs_;
};


#endif /// !SIMULATION_END_CONDITION_HH
