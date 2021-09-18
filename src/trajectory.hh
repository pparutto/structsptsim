#ifndef TRAJECTORY_HH_
#define TRAJECTORY_HH_

# include <vector>

# include "point.hh"

template <size_t N>
using Trajectory = std::vector<TimedPoint<N>>;

template <size_t N>
using TrajectoryEnsemble = std::vector<Trajectory<N>>;

template <size_t N>
TimedPoint<N> from_point(double t, const Point<N>& p);

#endif /// !TRAJECTORY_HH
