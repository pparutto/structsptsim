#include "io.hh"

#include <iostream>
#include <fstream>

void
save_trajectories_csv(const std::string fname, const TrajectoryEnsemble& trajs)
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  int cpt = 0;
  for (const Trajectory& traj: trajs)
  {
    for (const TimedPoint& p: traj)
      f << cpt << "," << p[0] << "," << p[1] << "," << p[2] << std::endl;
    ++cpt;
  }
  f.close();
}
