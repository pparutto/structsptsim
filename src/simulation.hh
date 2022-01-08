#ifndef SIMULATION_HH_
#define SIMULATION_HH_

# include "io.hh"

# include "trajectory_generator.hh"
# include "simulation_end_condition.hh"

template <size_t N>
class SimulationEndCondition;

template <size_t N>
class Simulation
{
public:
  Simulation(TrajectoryGeneratorFactory<N>& traj_gen_facto,
	     double obj_radius);
  virtual ~Simulation();

  virtual void run() = 0;

  const TrajectoryEnsemble<N>& trajs() const { return this->trajs_; }
protected:
  TrajectoryGeneratorFactory<N>& traj_gen_facto_;
  TrajectoryEnsemble<N> trajs_;
  double obj_radius_;
};

template <size_t N>
class SimulationTrajectory: public Simulation<N>
{
public:
  SimulationTrajectory(TrajectoryGeneratorFactory<N>& traj_gen_facto,
		       SimulationEndCondition<N>& end_cond,
		       double obj_radius);
  virtual ~SimulationTrajectory();

  virtual void run();

protected:
  SimulationEndCondition<N>& end_cond_;
};

template <size_t N>
class SimulationDensity: public Simulation<N>
{
public:
  SimulationDensity(TrajectoryGeneratorFactory<N>& traj_gen_facto,
		    SimulationEndCondition<N>& end_cond,
		    unsigned ntrajs, double DT, unsigned tratio,
		    double obj_radius);
  virtual ~SimulationDensity();

  virtual void run();
protected:
  SimulationEndCondition<N>& end_cond_;
  unsigned ntrajs_;
  double DT_;
  unsigned tratio_;
};

class SimulationEmpirical: public Simulation<2>
{
public:
  SimulationEmpirical(TrajectoryGeneratorFactory<2>& traj_gen_facto,
		      const TrajectoryCharacsMap& traj_characs,
		      double DT, double obj_radius);
  virtual ~SimulationEmpirical();

  virtual void run();
protected:
  const std::map<int, std::vector<TrajectoryCharacs> >& traj_characs_;
  double DT_;
};


#endif /// !SIMULATION_HH
