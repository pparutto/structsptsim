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

  Box sim_reg({9.0, 9.0}, {11.0, 11.0});
  double dt = 0.0001;
  //double DT = 0.02;
  double D = 0.05;
  int max_npts = 20;
  int max_ntrajs = 100;

  RandomBoxTrajectoryStartGenerator start_gen =
    RandomBoxTrajectoryStartGenerator(mt, sim_reg);
  BrownianMotion bm(mt, D, dt);
  NumberPointsEndCondition traj_end_cond(max_npts);
  FullTrajectoryRecorder traj_rec;
  NumberTrajectoriesSimulationEndCondition end_sim(max_ntrajs);

  TrajectoryGenerator traj_gen(start_gen, bm, traj_end_cond, traj_rec, dt);

  Simulation sim(traj_gen, end_sim);

  sim.run();

  save_trajectories_csv("/tmp/trajs.csv", sim.trajs());

  std::cout << "DONE" << std::endl;
}
