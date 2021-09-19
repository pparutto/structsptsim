#include "point.hh"

#include <cmath>
#include <cfloat>
#include <iostream>

#include "utils.hh"
#include "segment.hh"

bool
colinear(const Point<2>& p1, const Point<2>& p2, const Point<2>& p3)
{
  return orientation(p1, p2, p3) == COLINEAR;
}

Orientation
orientation(const Point<2>& p1, const Point<2>& p2, const Point<2>& p3)
{
  double v = ((p2[1] - p1[1]) * (p3[0] - p2[0]) -
	      (p2[0] - p1[0]) * (p3[1] - p2[1]));

  Segment<2> s = Segment<2>(p1, p2);
  if (std::abs(v) < EPSILON && s.distance(p3) < EPSILON)
    return COLINEAR;
  if (v > EPSILON)
    return CLOCKWISE;
  return COUNTERCLOCKWISE;
}

double
round_to_precision(double v)
{
  return std::round(v * PRECISION) / PRECISION;
}
