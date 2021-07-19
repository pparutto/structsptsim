#ifndef SIMULATION_HH_
#define SIMULATION_HH_

# include "io.hh"

# include "trajectory_generator.hh"
# include "simulation_end_condition.hh"

class SimulationEndCondition;

class Simulation
{
public:
  Simulation(TrajectoryGeneratorFactory& traj_gen_facto);
  virtual ~Simulation();

  virtual void run() = 0;

  const TrajectoryEnsemble& trajs() const { return this->trajs_; }
protected:
  TrajectoryGeneratorFactory& traj_gen_facto_;

  TrajectoryEnsemble trajs_;
};

class SimulationTrajectory: public Simulation
{
public:
  SimulationTrajectory(TrajectoryGeneratorFactory& traj_gen_facto,
		       SimulationEndCondition& end_cond);
  virtual ~SimulationTrajectory();

  virtual void run();

protected:
  SimulationEndCondition& end_cond_;
};

class SimulationDensity: public Simulation
{
public:
  SimulationDensity(TrajectoryGeneratorFactory& traj_gen_facto,
		    SimulationEndCondition& end_cond,
		    unsigned ntrajs, double DT, unsigned tratio);
  virtual ~SimulationDensity();

  virtual void run();
protected:
  SimulationEndCondition& end_cond_;
  unsigned ntrajs_;
  double DT_;
  unsigned tratio_;
};

class SimulationEmpirical: public Simulation
{
public:
  SimulationEmpirical(TrajectoryGeneratorFactory& traj_gen_facto,
		      const TrajectoryCharacsMap& traj_characs,
		      double DT);
  virtual ~SimulationEmpirical();

  virtual void run();
protected:
  const std::map<int, std::vector<TrajectoryCharacs> >& traj_characs_;
  double DT_;
};


#endif /// !SIMULATION_HH
