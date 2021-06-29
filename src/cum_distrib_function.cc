#include "cum_distrib_function.hh"

#include <cassert>

#include <fstream>
#include <sstream>


CumDistribFunction::CumDistribFunction(const std::string& fname)
  : x_()
  , y_()
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
}

double
CumDistribFunction::draw(double p) const
{
  for (unsigned i = 0; i < this->x_.size() - 1; ++i)
  {
    if (p >= this->y_[i] && p < this->y_[i+1])
      return this->x_[i];
  }
  return this->x_[this->x_.size() - 1];
}
