#ifndef TRAJECTORY_HH_
#define TRAJECTORY_HH_

# include <vector>
# include <iostream>

# include "point.hh"

template <size_t N>
using Trajectory = std::vector<TimedPoint<N>>;

template <size_t N>
using TrajectoryEnsemble = std::vector<Trajectory<N>>;

template <size_t N>
TimedPoint<N> from_point(double t, const Point<N>& p);

template <size_t N>
std::ostream& operator<< (std::ostream& os, const Trajectory<N>& tr);



//////////////////////// Implementation

template <size_t N>
std::ostream& operator<< (std::ostream& os,
			  const std::vector<std::array<double, N> >& tr)
{
  for (const Point<N>& p: tr)
    os << p << std::endl;
  return os;
}


#endif /// !TRAJECTORY_HH
