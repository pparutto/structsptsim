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

  Box sim_box({9.9, 9.9}, {10.1, 10.1});
  double dt = 0.0001;
  double DT = 0.02;
  double D = 0.1;
  int max_npts = 2000;
  int max_ntrajs = 1;

  FixedPointTrajectoryStartGenerator start_gen =
    FixedPointTrajectoryStartGenerator({10.0, 10.0});
  BrownianMotion bm(mt, D, dt);
  NumberPointsEndCondition traj_end_cond(max_npts);
  FullTrajectoryRecorder traj_rec(DT);
  NumberTrajectoriesSimulationEndCondition end_sim(max_ntrajs);
  BoxCollider bcollider(sim_box);

  TrajectoryGenerator traj_gen(start_gen, bm, traj_end_cond, traj_rec,
			       bcollider);

  Simulation sim(traj_gen, end_sim);

  sim.run();

  save_trajectories_csv("/tmp/trajs.csv", sim.trajs());

  std::cout << "DONE" << std::endl;
}
