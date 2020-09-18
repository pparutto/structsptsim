#include <iostream>
#include <iomanip>
#include <random>

#include "simulation.hh"
#include "simulation_end_condition.hh"
#include "io.hh"

#include "utils.hh"

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  std::cout << std::setprecision(15);

  std::random_device rd;
  std::mt19937_64 mt(rd());

  double dt = 0.0001;
  double DT = 0.004;
  double D = 9;
  int max_npts = 5;
  int max_ntrajs = 500;

  double poly_px_size = 0.0406250;

  //CompoundPolygon poly = poly_from_inkscape_path("../resources/ER_net1.path");
  CompoundPolygon poly = poly_from_inkscape_path("/mnt/data2/ER_net simu/190418_COS7_Halo-KDEL_YFP-Z_day1_JF646PC_SPT_9/190418_COS7_Halo-KDEL_YFP-Z_day1_JF646PC_SPT_9_G_med4_poly.path");

  poly.apply_pxsize(poly_px_size);

  save_poly_matlab(poly, "/tmp/load_polys.m");

  //FixedPointTrajectoryStartGenerator start_gen({10.0, 10.0});
  RandomTrajectoryStartGenerator start_gen(mt, poly);

  BrownianMotion bm(mt, D, dt);
  NumberPointsEndCondition traj_end_cond(max_npts);

  //FullTrajectoryRecorder traj_rec(DT);
  SubsambleTrajectoryRecorder traj_rec(DT, (unsigned) (DT / dt));

  NumberTrajectoriesSimulationEndCondition end_sim(max_ntrajs);

  //NoneCollider collider;
  PolygonCollider collider(poly);

  TrajectoryGenerator traj_gen(start_gen, bm, traj_end_cond, traj_rec,
			       collider);

  Simulation sim(traj_gen, end_sim);

  sim.run();

  std::string res_name = "trajs_D=" + std::to_string(D) +
    "_dt=" + std::to_string(dt) +
    "_DT=" + std::to_string(DT) +
    "_maxNpts=" + std::to_string(max_npts) +
    "_maxNtrajs=" + std::to_string(max_ntrajs) +
    ".csv";

  save_trajectories_csv("/tmp/" + res_name, sim.trajs());

  std::cout << "DONE" << std::endl;
}
