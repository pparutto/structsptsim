#include "trajectory_start_generator.hh"

#include <iostream>

FixedPointTrajectoryStartGenerator::
FixedPointTrajectoryStartGenerator(const Point& pt)
  : pt_(pt)
{
}

Point
FixedPointTrajectoryStartGenerator::generate()
{
  return this->pt_;
}

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

RandomTrajectoryStartGenerator::
RandomTrajectoryStartGenerator(std::mt19937_64& ng, const Shape& shape)
  : ng_(ng)
  , shape_(shape)
  , rnd_box_(RandomBoxTrajectoryStartGenerator(ng, shape.bounding_box()))
{
}

Point
RandomTrajectoryStartGenerator::generate()
{
  Point p = this->rnd_box_.generate();
  while (!this->shape_.inside(p))
    p = this->rnd_box_.generate();

  return p;
}
