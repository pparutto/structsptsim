#ifndef POINT_HH_
#define POINT_HH_

# include <array>
# include <vector>

template <int N>
using Point = std::array<double, N>;

template <int N>
using Vec = std::array<double, N>;

template <int N>
using TimedPoint = std::array<double, N+1>;

template <int N>
using PointEnsemble = std::vector<Point<N>>;

enum Orientation {COLINEAR, CLOCKWISE, COUNTERCLOCKWISE};

template <int N>
Point<N> to_point(const TimedPoint<N>& tp);

bool colinear(const Point<2>& p1, const Point<2>& p2,
	      const Point<2>& p3);

template<int N>
bool operator== (const Point<N>& p1, const Point<N>& p2);

template<int N>
Point<N> operator- (const Point<N>& p1, const Point<N>& p2);

template<int N>
Point<N> operator+ (const Point<N>& p1, const Point<N>& p2);

Orientation orientation(const Point<2>& p1, const Point<2>& p2,
			const Point<2>& p3);

template<int N>
double dot(const Point<N>& v1, const Point<N>& v2);

template<int N>
double norm(const Point<N>& v);

template<int N>
double dist(const Point<N>& p1, const Point<N>& p2);

template<int N>
std::ostream& operator<< (std::ostream& os, const std::array<double, N>& pt);

#endif /// !POINT_HH
