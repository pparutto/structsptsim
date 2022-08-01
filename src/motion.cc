#include "motion.hh"

# include <cmath>

template <size_t N>
BrownianMotion<N>::BrownianMotion(std::mt19937_64& ng, double D, double dt)
  : ng_(ng)
  , randn_()
  , D_(D)
  , dt_(dt)
  , s2Ddt_(sqrt(2 * D * dt))
{
}

template <size_t N>
Point<N>
BrownianMotion<N>::step_euler(const Point<N>& p)
{
  Point<N> res;
  for (size_t i = 0; i < N; ++i)
    res[i] = p[i] + this->s2Ddt_ * this->randn_(this->ng_);
  return round_to_precision<N>(res);
}

template <size_t N>
bool
BrownianMotion<N>::subsample() const
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

Point<2>
EmpiricalMotion::step_euler(const Point<2>& p)
{
  double r = this->ivel_cdf_.draw(this->randu_(this->ng_)) * this->dt_;
  double ang = this->randu_(this->ng_) * 2 * M_PI;
  Point<2> res({p[0] + r * cos(ang), p[1] + r * sin(ang)});
  return round_to_precision<2>(res);
}

bool
EmpiricalMotion::subsample() const
{
  return false;
}

template class BrownianMotion<2>;
template class BrownianMotion<3>;
