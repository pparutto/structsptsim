#include "trajectory.hh"

template <size_t N>
TimedPoint<N>
from_point(double t, const Point<N>& p)
{
  TimedPoint<N> res;
  res[0] = t;
  for (size_t i = 0; i < N; ++i)
    res[i+1] = p[i];
  return res;
}
