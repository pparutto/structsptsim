#ifndef POINT_HH_
#define POINT_HH_

# include <array>
# include <vector>

using Point = std::array<double, 2>;
using Vec = std::array<double, 2>;
using TimedPoint = std::array<double, 3>;

using PointEnsemble = std::vector<Point>;

enum Orientation {COLINEAR, CLOCKWISE, COUNTERCLOCKWISE};

bool colinear(const Point& p1, const Point& p2, const Point& p3);
bool operator== (const Point& p1, const Point& p2);
Point operator- (const Point& p1, const Point& p2);
Point operator+ (const Point& p1, const Point& p2);

Orientation orientation(const Point& p1, const Point& p2, const Point& p3);

double dot(const Vec& v1, const Vec& v2);
double norm(const Vec& v);
double dist(const Point& p1, const Point& p2);

std::ostream& operator<< (std::ostream& os, const Point& pt);

#endif /// !POINT_HH
