#ifndef TRAJECTORY_HH_
#define TRAJECTORY_HH_

# include <vector>

# include "point.hh"

using  Trajectory = std::vector<TimedPoint>;
using  TrajectoryEnsemble = std::vector<Trajectory>;

TimedPoint from_point(double t, const Point& p);

#endif /// !TRAJECTORY_HH
