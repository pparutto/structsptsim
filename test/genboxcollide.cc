#include <iostream>

#include <random>

#include "simulation.hh"
#include "simulation_end_condition.hh"
#include "io.hh"

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  std::random_device rd;
  std::mt19937_64 mt(rd());

  Box<2> sim_box({9.9, 9.9}, {10.1, 10.1});
  double dt = 0.0001;
  double DT = 0.02;
  double D = 0.1;
  int max_npts = 2000;
  int max_ntrajs = 1;

  FixedPointTrajectoryStartGenerator<2> start_gen =
    FixedPointTrajectoryStartGenerator<2>({10.0, 10.0});
  BrownianMotion<2> bm(mt, D, dt);
  NumberPointsEndCondition<2> traj_end_cond(max_npts);
  TrajectoryEndConditionFactory<2> traj_end_cond_facto(traj_end_cond);
  FullTrajectoryRecorder<2> traj_rec(0.0, DT);
  TrajectoryRecorderFactory<2> traj_rec_facto(traj_rec);
  NumberTrajectoriesSimulationEndCondition<2> end_sim(max_ntrajs);
  BoxCollider<2> bcollider(sim_box);

  VoidLogger* log = new VoidLogger();

  TrajectoryGeneratorFactory<2>
    traj_gen_facto(start_gen, bm, traj_end_cond_facto, traj_rec_facto,
		   bcollider, log);

  SimulationTrajectory<2> sim(traj_gen_facto, end_sim);

  sim.run();

  save_trajectories_csv<2>("/tmp/trajs.csv", sim.trajs());

  delete log;
  std::cout << "DONE" << std::endl;
}
