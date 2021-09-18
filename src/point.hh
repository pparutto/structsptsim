#ifndef POINT_HH_
#define POINT_HH_

# include <array>
# include <vector>

template <size_t N>
using Point = std::array<double, N>;

template <size_t N>
using Vec = std::array<double, N>;

template <size_t N>
using TimedPoint = std::array<double, N+1>;

template <size_t N>
using PointEnsemble = std::vector<Point<N>>;

enum Orientation {COLINEAR, CLOCKWISE, COUNTERCLOCKWISE};

template <size_t N>
Point<N> zero();

template <size_t N>
Point<N> to_point(const TimedPoint<N>& tp);

bool colinear(const Point<2>& p1, const Point<2>& p2,
	      const Point<2>& p3);

template<size_t N>
bool operator== (const Point<N>& p1, const Point<N>& p2);

template<size_t N>
Point<N> operator- (const Point<N>& p1, const Point<N>& p2);

template<size_t N>
Point<N> operator+ (const Point<N>& p1, const Point<N>& p2);

Orientation orientation(const Point<2>& p1, const Point<2>& p2,
			const Point<2>& p3);

template<size_t N>
double dot(const Point<N>& v1, const Point<N>& v2);

template<size_t N>
double norm(const Vec<N>& v);

template<size_t N>
double dist(const Point<N>& p1, const Point<N>& p2);

template<size_t N>
std::ostream& operator<< (std::ostream& os, const std::array<double, N>& pt);

#endif /// !POINT_HH
