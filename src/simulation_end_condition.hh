#ifndef SIMULATION_END_CONDITION_HH_
#define SIMULATION_END_CONDITION_HH_

# include <cstdlib>
# include <iostream>

template <size_t N>
class Simulation;

template <size_t N>
class SimulationEndCondition
{
public:
  virtual ~SimulationEndCondition();
  virtual bool evaluate(const Simulation<N>& sim) = 0;

  virtual void who_am_I(std::ostream& os) const = 0;
};

template <size_t N>
class NumberTrajectoriesSimulationEndCondition: public SimulationEndCondition<N>
{
public:
  NumberTrajectoriesSimulationEndCondition(unsigned max_ntrajs);
  virtual ~NumberTrajectoriesSimulationEndCondition();

  virtual bool evaluate(const Simulation<N>& sim) override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  unsigned max_ntrajs_;
};

template <size_t N>
class NumberFramesSimulationEndCondition: public SimulationEndCondition<N>
{
public:
  NumberFramesSimulationEndCondition(unsigned max_frames);
  virtual ~NumberFramesSimulationEndCondition();

  virtual bool evaluate(const Simulation<N>& sim) override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  unsigned cur_frame_;
  unsigned max_frames_;
};


#endif /// !SIMULATION_END_CONDITION_HH
