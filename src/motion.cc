#include "motion.hh"

# include <cmath>

BrownianMotion::BrownianMotion(std::mt19937_64& ng, double D, double dt)
  : ng_(ng)
  , randn_()
  , D_(D)
  , dt_(dt)
  , s2Ddt_(sqrt(2 * D * dt))
{
}

Point
BrownianMotion::step_euler(const Point& p)
{
  return {p[0] + this->s2Ddt_ * this->randn_(this->ng_),
	  p[1] + this->s2Ddt_ * this->randn_(this->ng_)};
}

bool
BrownianMotion::subsample() const
{
  return true;
}

EmpiricalMotion::
EmpiricalMotion(std::mt19937_64& ng, CumDistribFunction& ivel_cdf,
		double dt)
  : ng_(ng)
  , randu_()
  , ivel_cdf_(ivel_cdf)
  , dt_(dt)
{
}

Point
EmpiricalMotion::step_euler(const Point& p)
{
  //std::cout << "p = " << p[0] << " " << p[1] << std::endl;
  double r = this->ivel_cdf_.draw(this->randu_(this->ng_)) * this->dt_;
  double ang = this->randu_(this->ng_) * 2 * M_PI;
  //std::cout << "pp = " << p[0] + r * cos(ang) << " " << p[1] + r * sin(ang) << std::endl;
  return {p[0] + r * cos(ang), p[1] + r * sin(ang)};
}

bool
EmpiricalMotion::subsample() const
{
  return false;
}
