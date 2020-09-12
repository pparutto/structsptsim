#include <iostream>

#include <random>

#include "simulation.hh"
#include "simulation_end_condition.hh"
#include "io.hh"

#include "utils.hh"

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  std::random_device rd;
  std::mt19937_64 mt(rd());

  double dt = 0.0001;
  double DT = 0.2;
  double D = 3000;
  int max_npts = 20;
  int max_ntrajs = 100;

  Polygon poly = poly_from_inkscape_path("../resources/U.csv");

  //FixedPointTrajectoryStartGenerator start_gen({10.0, 10.0});
  RandomTrajectoryStartGenerator start_gen(mt, poly);

  BrownianMotion bm(mt, D, dt);
  NumberPointsEndCondition traj_end_cond(max_npts);
  FullTrajectoryRecorder traj_rec;
  NumberTrajectoriesSimulationEndCondition end_sim(max_ntrajs);

  //NoneCollider collider;
  PolygonCollider collider(poly);

  TrajectoryGenerator traj_gen(start_gen, bm, traj_end_cond, traj_rec,
			       collider, DT);

  Simulation sim(traj_gen, end_sim);

  sim.run();

  save_trajectories_csv("/tmp/trajs.csv", sim.trajs());

  std::cout << "DONE" << std::endl;
}
