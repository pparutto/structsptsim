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

template <size_t N>
void
shift_coords(Trajectory<N>& tr, const Point<N>& shifts)
{
  for (unsigned i = 0; i < tr.size(); ++i)
  {
    //it is a TimedPoint
    for (unsigned k = 0; k < N; ++k)
      tr[i][k+1] += shifts[k];
  }
}

template <size_t N>
void
shift_coords(TrajectoryEnsemble<N>& te, const Point<N>& shifts)
{
  for (Trajectory<N>& tr: te)
    shift_coords(tr, shifts);
}

template <size_t N>
unsigned
count_pts(const TrajectoryEnsemble<N>& te)
{
  unsigned cnt = 0;
  for (const Trajectory<N>& tr: te)
    cnt += tr.size();
  return cnt;
}

template void shift_coords(Trajectory<2>& te, const Point<2>& shifts);
template void shift_coords(TrajectoryEnsemble<2>& te, const Point<2>& shifts);
template unsigned count_pts<2>(const TrajectoryEnsemble<2>& te);
