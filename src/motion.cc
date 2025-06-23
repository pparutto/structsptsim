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

HMM2DMotion::HMM2DMotion(std::mt19937_64& ng, const std::array<double,2>& Ds,
			 const std::array<double,2>& rates, double dt)
  : ng_(ng)
  , randu_()
  , dt_(dt)
  , rates_(rates)
  , bms_()
  , cur_state_(0)
  , transi_()
{
  for (double D: Ds)
    this->bms_.push_back(BrownianMotion<2> (ng, D, dt));

  double kon = rates[0];
  double koff = rates[1];

  this->transi_[0][1] = kon / (kon + koff) * (1 - exp(-(kon + koff) * this->dt_));
  this->transi_[0][0] = 1 - this->transi_[0][1];
  this->transi_[1][0] = koff / (kon + koff) * (1 - exp(-(kon + koff) * this->dt_));
  this->transi_[1][1] = 1 - this->transi_[1][0];

  double pi1_ = this->transi_[1][0] / (this->transi_[0][1] + this->transi_[1][0]);
  double u = this->randu_(this->ng_);
  if (u >= pi1_)
    this->cur_state_ = 1;
}

Point<2>
HMM2DMotion::step_euler(const Point<2>& p)
{
  double u = this->randu_(this->ng_);

  if (this->cur_state_ == 0 && u <= this->transi_[0][1])
    this->cur_state_ = 1;
  else if (this->cur_state_ == 1 && u <= this->transi_[1][0])
    this->cur_state_ = 0;

  return this->bms_[this->cur_state_].step_euler(p);
}

bool
HMM2DMotion::subsample() const
{
  return true;
}

std::string
HMM2DMotion::to_str() const
{
  if (this->bms_.empty())
    return "";

  std::string res;
  res += "HMM Motion: ";
  
  res += "D1=" + std::to_string(this->bms_[0].D()) + " ";
  res += "D2=" + std::to_string(this->bms_[0].D()) + " ";
  res += "kon=" + std::to_string(this->rates_[0]) + " ";
  res += "koff=" + std::to_string(this->rates_[1]);

  return res;
}


template class BrownianMotion<2>;
template class BrownianMotion<3>;
