#include "trajectory.hh"

TimedPoint
from_point(double t, const Point& p)
{
  return {t, p[0], p[1]};
}
