#ifndef SIMULATION_HH_
#define SIMULATION_HH_

# include "trajectory_generator.hh"

class SimulationEndCondition;

class Simulation
{
public:
  Simulation(TrajectoryGenerator& traj_gen,
	     SimulationEndCondition& end_cond);

  void run();

  const TrajectoryEnsemble& trajs() const { return this->trajs_; }

protected:
  TrajectoryGenerator& traj_gen_;
  SimulationEndCondition& end_cond_;

  TrajectoryEnsemble trajs_;
};

#endif /// !SIMULATION_HH
