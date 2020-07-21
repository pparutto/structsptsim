#ifndef IO_HH_
#define IO_HH_

# include <string>

# include "trajectory.hh"

void save_trajectories_csv(const std::string fname,
			   const TrajectoryEnsemble& trajs);

#endif /// !IO_HH
