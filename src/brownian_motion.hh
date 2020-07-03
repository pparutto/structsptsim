#ifndef BROWNIAN_MOTION_HH_
#define BROWNIAN_MOTION_HH_

# include <random>

# include "point.hh"


class BrownianMotion
{
public:
  BrownianMotion(std::mt19937_64& ng, double D, double dt);

  Point step_euler(const Point& p);

protected:
  std::mt19937_64& ng_;
  std::normal_distribution<> randn_;
  double D_;
  double dt_;
  double s2Ddt_;
};


#endif /// !BROWNIAN_MOTION_HH
