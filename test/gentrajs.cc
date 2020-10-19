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

  FixedPointTrajectoryStartGenerator start_gen({10, 10});

  NumberPointsEndCondition end_cond(5);
  TrajectoryEndConditionFactory traj_end_cond_facto(end_cond);

  BrownianMotion bm(mt, D, dt);

  FullTrajectoryRecorder traj_rec(0.0, dt);
  TrajectoryRecorderFactory traj_rec_facto(traj_rec);

  NoneCollider collider;

  TrajectoryGeneratorFactory traj_gen_facto(start_gen, bm, traj_end_cond_facto,
					    traj_rec_facto, collider);

  TrajectoryGenerator* traj_gen = traj_gen_facto.get(0.0);

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
}
