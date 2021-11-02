#ifndef POINT_HH_
#define POINT_HH_

# include <cmath>
# include <array>
# include <vector>
# include <iostream>

# include "constant.hh"

template <size_t N>
using Point = std::array<double, N>;

template <size_t N>
using Vec = std::array<double, N>;

template <size_t N>
using TimedPoint = std::array<double, N+1>;

template <size_t N>
using PointEnsemble = std::vector<Point<N> >;

enum Orientation {COLINEAR, CLOCKWISE, COUNTERCLOCKWISE};

bool colinear(const Point<2>& p1, const Point<2>& p2,
	      const Point<2>& p3);

Orientation orientation(const Point<2>& p1, const Point<2>& p2,
			const Point<2>& p3);

Vec<3> cross(const Vec<3>& v1, const Vec<3>& v2);

double round_to_precision(double v);

template <size_t N>
Point<N> null_point();

template <size_t N>
Point<N> zero();

template <size_t N>
Point<N> to_point(const TimedPoint<N>& tp);

template <size_t N>
bool operator== (const std::array<double, N>& v1,
		 const std::array<double, N>& v2);

template <size_t N>
std::array<double, N> operator- (const std::array<double, N>& v1,
				 const std::array<double, N>& v2);

template <size_t N>
std::array<double, N> operator+ (const std::array<double, N>& v1,
				 const std::array<double, N>& v2);

template <size_t N>
std::array<double, N> operator* (const std::array<double, N>& v,
				 double val);

template <size_t N>
std::array<double, N> operator/ (const std::array<double, N>& v,
				 double val);

template <size_t N>
double dot(const std::array<double, N>& v1, const std::array<double, N>& v2);

template <size_t N>
double norm(const Vec<N>& v);

template <size_t N>
double dist(const Point<N>& p1, const Point<N>& p2);


template <size_t N>
std::ostream& operator<< (std::ostream& os, const std::array<double, N>& pt);

template <size_t N>
Point<N>& round_to_precision(Point<N>& v);


/////////// implementation

template <size_t N>
Point<N> null_point()
{
  Point<N> res;
  for (size_t i = 0; i < N; ++i)
    res[i] = NAN;
  return res;
}

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
  for (size_t i = 0; i < N; ++i)
    res[i] = tp[i+1];
  return res;
}

template <size_t N>
bool operator== (const std::array<double, N>& v1,
		 const std::array<double, N>& v2)
{
  for (size_t i = 0; i < N; ++i)
    if (std::isnan(v1[i]) || std::isnan(v2[i]) ||
	std::abs(v1[i] - v2[i]) > EPSILON)
      return false;
  return true;
}

template <size_t N>
std::array<double, N> operator- (const std::array<double, N>& v1,
				 const std::array<double, N>& v2)
{
  std::array<double, N> res = v1;
  for (size_t i = 0; i < N; ++i)
    res[i] -= v2[i];
  return res;
}

template <size_t N>
std::array<double, N> operator+ (const std::array<double, N>& v1,
				 const std::array<double, N>& v2)
{
  std::array<double, N> res = v1;
  for (size_t i = 0; i < N; ++i)
    res[i] += v2[i];
  return res;
}

template <size_t N>
std::array<double, N> operator* (const std::array<double, N>& v,
				 double val)
{
  std::array<double, N> res = v;
  for (size_t i = 0; i < N; ++i)
    res[i] *= val;
  return res;
}

template <size_t N>
std::array<double, N> operator/ (const std::array<double, N>& v,
				 double val)
{
  std::array<double, N> res = v;
  for (size_t i = 0; i < N; ++i)
    res[i] /= val;
  return res;
}

template <size_t N>
double dot(const std::array<double, N>& v1, const std::array<double, N>& v2)
{
  double res = 0;
  for (size_t i = 0; i < N; ++i)
    res += v1[i] * v2[i];
  return res;
}

template <size_t N>
double norm(const Vec<N>& v)
{
  double res = 0;
  for (size_t i = 0; i < N; ++i)
    res += v[i] * v[i];
  return sqrt(res);
}

template <size_t N>
double dist(const Point<N>& p1, const Point<N>& p2)
{
  double res = 0;
  for (size_t i = 0; i < N; ++i)
    res += (p1[i] - p2[i]) * (p1[i] - p2[i]);
  return sqrt(res);
}


template <size_t N>
std::ostream& operator<< (std::ostream& os, const std::array<double, N>& pt)
{
  os << pt[0];
  for (size_t i = 1; i < N; ++i)
    os << ", " << pt[i];
  return os;
}

template <size_t N>
Point<N>& round_to_precision(Point<N>& v)
{
  for (size_t i = 0; i < N; ++i)
    v[i] = round_to_precision(v[i]);
  return v;
}


#endif /// !POINT_HH
