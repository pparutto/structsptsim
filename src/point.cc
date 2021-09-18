#include "point.hh"

#include <cmath>
#include <cfloat>
#include <iostream>

#include "utils.hh"
#include "segment.hh"

template <size_t N>
Point<N> zero()
{
  Point<N> res;
  for (size_t i = 0; i < N; ++i)
    res[i] = 0;
  return res;
}

template <size_t N>
Point<N> to_point(const TimedPoint<N>& tp)
{
  Point<N> res;
  for (int i = 0; i < N; ++i)
    res[i] = tp[i+1];
  return res;
}

template <size_t N>
bool
operator== (const Point<2>& p1, const Point<2>& p2)
{
  for (int i = 0; i < N; ++i)
    if (std::abs(p1[i] - p2[i]) > EPSILON)
      return false;
  return true;
}

template <size_t N>
Point<N> operator- (const Point<N>& p1, const Point<N>& p2)
{
  return p1;
  // Point<N> res;
  // for (int i = 0; i < N; ++i)
  //   res[i] = p1[i] - p2[i];
  // return res;
}

//template Point<2> operator-<2> (const Point<2>& p1, const Point<2>& p2);

template <size_t N>
Point<N> operator+ (const Point<N>& p1, const Point<N>& p2)
{
  Point<N> res = p1;
  for (int i = 0; i < N; ++i)
    res[i] += p2[i];
  return res;
}

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

template <size_t N>
double dot(const Vec<N>& v1, const Vec<N>& v2)
{
  double res = 0;
  for (int i = 0; i < N; ++i)
    res += v1[i] * v2[i];
  return res;
}

template <size_t N>
double norm(const Vec<N>& v)
{
  double res = 0;
  for (int i = 0; i < N; ++i)
    res += v[i] * v[i];
  return sqrt(res);
}

template <size_t N>
double dist(const Point<N>& p1, const Point<N>& p2)
{
  double res = 0;
  for (int i = 0; i < N; ++i)
    res += (p1[i] - p2[i]) * (p1[i] - p2[i]);
  return sqrt(res);
}

template <size_t N>
std::ostream&
operator<< (std::ostream& os, const std::array<double, N>& pt)
{
  os << pt[0];
  for (int i = 1; i < N; ++i)
    os << " " << pt[i];
  return os;
}
