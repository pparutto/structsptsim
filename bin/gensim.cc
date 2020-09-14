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
  double DT = 1.0;
  double D = 10000;
  int max_npts = 1000;
  int max_ntrajs = 10;

  PointEnsemble pe;
  pe.push_back({5, 5});
  pe.push_back({10, 5});
  pe.push_back({10, 10});
  pe.push_back({5, 10});
  Polygon base_poly(pe);

  pe.clear();
  pe.push_back({6, 6});
  pe.push_back({9, 6});
  pe.push_back({9, 9});
  pe.push_back({6, 9});
  std::vector<Polygon> diff_polys;
  diff_polys.push_back(Polygon(pe));

  //CompoundPolygon poly(base_poly, diff_polys);
  CompoundPolygon poly = poly_from_inkscape_path("../resources/ER_net1.path");


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

  save_poly_matlab(poly, "/tmp/load_polys.m");
  save_trajectories_csv("/tmp/trajs.csv", sim.trajs());

  std::cout << "DONE" << std::endl;
}
