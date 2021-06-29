#ifndef SIMULATION_HH_
#define SIMULATION_HH_

# include "trajectory_generator.hh"
# include "simulation_end_condition.hh"

class SimulationEndCondition;

class Simulation
{
public:
  Simulation(const TrajectoryGeneratorFactory& traj_gen_facto,
	     SimulationEndCondition& end_cond);
  virtual ~Simulation();

  virtual void run() = 0;

  const TrajectoryEnsemble& trajs() const { return this->trajs_; }
protected:
  const TrajectoryGeneratorFactory& traj_gen_facto_;
  SimulationEndCondition& end_cond_;

  TrajectoryEnsemble trajs_;
};

class SimulationTrajectory: public Simulation
{
public:
  SimulationTrajectory(const TrajectoryGeneratorFactory& traj_gen_facto,
		       SimulationEndCondition& end_cond);
  virtual ~SimulationTrajectory();

  virtual void run();
};

class SimulationDensity: public Simulation
{
public:
  SimulationDensity(const TrajectoryGeneratorFactory& traj_gen_facto,
		    SimulationEndCondition& end_cond,
		    unsigned ntrajs, double DT, unsigned tratio);
  virtual ~SimulationDensity();

  virtual void run();
protected:
  unsigned ntrajs_;
  double DT_;
  unsigned tratio_;
};


#endif /// !SIMULATION_HH
