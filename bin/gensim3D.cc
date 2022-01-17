#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <sys/stat.h>
#include <sys/types.h>

#include <cassert>

#include "tclap/CmdLine.h"

#include "simulation.hh"
#include "simulation_end_condition.hh"
#include "gensim_io.hh"

#include "cum_distrib_function.hh"

#include <tiffio.h>
#include "raw_image_simulator.hh"

#include "quadtree.hh"

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  std::cout << std::setprecision(15);

  std::random_device rd;
  std::mt19937_64 mt(rd());

  Cylinder c(Segment<3> ({2, 0, -50}, {2, 0, 50}), 0.5);
  (void) c;

  double DT = 0.01;
  CylinderCollider ccollider(c);
  //NoneCollider<3> ccollider;
  FixedPointTrajectoryStartGenerator<3> start({2, 0, 6});
  BrownianMotion<3> bm(mt, 1.0, DT);
  NumberPointsEndCondition<3>* end = new NumberPointsEndCondition<3>(10000);
  FullTrajectoryRecorder<3>* rec = new FullTrajectoryRecorder<3>(0, DT);

  TrajectoryGenerator<3> tgen(start, bm, end, rec, ccollider);


  Trajectory<3> traj = tgen.generate();

  //std::cout << traj;
}
