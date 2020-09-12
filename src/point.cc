#include "point.hh"

#include <cmath>
#include <iostream>

#include "utils.hh"

bool
operator== (const Point& p1, const Point& p2)
{
  return (std::abs(p1[0] - p2[0]) < PRECISION &&
	  std::abs(p1[1] - p2[1]) < PRECISION);
}

Point operator- (const Point& p1, const Point& p2)
{
  return {p1[0] - p2[0], p1[1] - p2[1]};
}

bool
colinear(const Point& p1, const Point& p2, const Point& p3)
{
  return orientation(p1, p2, p3) == COLINEAR;
}

Orientation
orientation(const Point& p1, const Point& p2, const Point& p3)
{
  double v = ((p2[1] - p1[1]) * (p3[0] - p2[0]) -
	      (p2[0] - p1[0]) * (p3[1] - p2[1]));

  if (std::abs(v) < PRECISION)
    return COLINEAR;
  if (v > 0)
    return CLOCKWISE;
  return COUNTERCLOCKWISE;
}

double norm(const Vec& v)
{
  return sqrt(v[0]*v[0] + v[1]*v[1]);
}

std::ostream&
operator<< (std::ostream& os, const Point& pt)
{
  os << pt[0] << " " << pt[1];
  return os;
}
