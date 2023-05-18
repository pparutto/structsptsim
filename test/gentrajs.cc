#include <iostream>
#include <random>

#include "simulation.hh"

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  double D = 2.0;
  double dt = 0.0001;

  std::random_device rd;
  std::mt19937_64 mt(rd());

  FixedPointTrajectoryStartGenerator<2> start_gen({10, 10});

  NumberPointsEndCondition<2> end_cond(5);
  TrajectoryEndConditionFactory<2> traj_end_cond_facto(end_cond);

  BrownianMotion<2> bm(mt, D, dt);

  FullTrajectoryRecorder<2> traj_rec(0.0, dt);
  TrajectoryRecorderFactory<2> traj_rec_facto(traj_rec);

  NoneCollider<2> collider;

  VoidLogger* log = new VoidLogger();

  TrajectoryGeneratorFactory<2>
    traj_gen_facto(start_gen, bm, traj_end_cond_facto, traj_rec_facto,
		   collider, log);

  TrajectoryGenerator<2>* traj_gen = traj_gen_facto.get(0.0);

  traj_gen->init();
  std::cout << traj_gen->get()[0] << std::endl;

  int cpt = 1;
  while (!traj_gen->finished())
  {
    traj_gen->generate_step();
    std::cout << traj_gen->get()[cpt] << std::endl;
    ++cpt;
  }

  delete traj_gen;
  delete log;
}
