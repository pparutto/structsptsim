#include "cum_distrib_function.hh"

#include <cassert>

#include <fstream>
#include <sstream>

#include <random>

CumDistribFunction::CumDistribFunction(std::mt19937_64& ng,
				       const std::string& fname)
  : ng_(ng)
  , x_()
  , y_()
  , bin_w_(-1)
  , randu_(0.0, 1.0)
{
  std::ifstream ifs(fname);

  if (!ifs.is_open())
  {
    std::cout << "ERROR file not found: " << fname << std::endl;
    assert(0);
  }

  //x,y are the lowest value of the bin
  std::string line;
  std::string tmp;
  while (std::getline(ifs, line))
  {
    std::istringstream ss(line);
    std::getline(ss, tmp, ',');
    this->x_.push_back(std::stod(tmp));
    std::getline(ss, tmp);
    this->y_.push_back(std::stod(tmp));
  }

  this->bin_w_ = this->x_[1] - this->x_[0];
}

double
CumDistribFunction::draw(double p)
{
  double noise = this->randu_(this->ng_) * this->bin_w_ - this->bin_w_ / 2;

  for (unsigned i = 0; i < this->x_.size() - 1; ++i)
  {
    if (p >= this->y_[i] && p < this->y_[i+1])
      return this->x_[i] + noise;
  }

  return this->x_[this->x_.size() - 1] + noise;
}
