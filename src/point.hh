#ifndef POINT_HH_
#define POINT_HH_

# include <array>
# include <vector>

using Point = std::array<double, 2>;
using TimedPoint = std::array<double, 3>;

using PointEnsemble = std::vector<Point>;

#endif /// !POINT_HH
