#include "trajectory_start_generator.hh"

RandomBoxTrajectoryStartGenerator::
RandomBoxTrajectoryStartGenerator(std::mt19937_64& ng, const Box& box)
  : ng_(ng)
  , box_(box)
  , randu_(0.0, 1.0)
{
}

Point
RandomBoxTrajectoryStartGenerator::generate()
{
  return {this->box_.lower_left()[0] + this->randu_(this->ng_) * this->box_.width(),
	  this->box_.lower_left()[1] + this->randu_(this->ng_) * this->box_.height()};
}
