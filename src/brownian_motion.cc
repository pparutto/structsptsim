#include "brownian_motion.hh"

#include <cmath>

BrownianMotion::BrownianMotion(std::mt19937_64& ng, double D, double dt)
  : ng_(ng)
  , randn_(0, 1)
  , D_(D)
  , dt_(dt)
  , s2Ddt_(sqrt(2 * D * dt))
{
}

Point
BrownianMotion::step_euler(const Point& p)
{
  return Point {p[0] + this->s2Ddt_ * this->randn_(this->ng_),
		p[1] + this->s2Ddt_ * this->randn_(this->ng_)};
}
