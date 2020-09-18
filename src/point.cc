#include "point.hh"

#include <cmath>
#include <cfloat>
#include <iostream>

#include "utils.hh"
#include "segment.hh"

bool
operator== (const Point& p1, const Point& p2)
{
  return (std::abs(p1[0] - p2[0]) < DBL_EPSILON &&
	  std::abs(p1[1] - p2[1]) < DBL_EPSILON);
}

Point operator- (const Point& p1, const Point& p2)
{
  return {p1[0] - p2[0], p1[1] - p2[1]};
}

Point operator+ (const Point& p1, const Point& p2)
{
  return {p1[0] + p2[0], p1[1] + p2[1]};
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

  Segment s = Segment(p1, p2);
  if (std::abs(v) < DBL_EPSILON && s.distance(p3) < DBL_EPSILON)
    return COLINEAR;
  if (v > DBL_EPSILON)
    return CLOCKWISE;
  return COUNTERCLOCKWISE;
}

double dot(const Vec& v1, const Vec& v2)
{
  return v1[0] * v2[0] + v1[1] * v2[1];
}

double norm(const Vec& v)
{
  return sqrt(v[0]*v[0] + v[1]*v[1]);
}

double dist(const Point& p1, const Point& p2)
{
  return sqrt((p1[0] - p2[0]) * (p1[0] - p2[0]) +
	      (p1[1] - p2[1]) * (p1[1] - p2[1]));
}

std::ostream&
operator<< (std::ostream& os, const Point& pt)
{
  os << pt[0] << " " << pt[1];
  return os;
}
