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

RandomPolygonTrajectoryStartGenerator::
RandomPolygonTrajectoryStartGenerator(std::mt19937_64& ng,
				      const CompoundPolygon& poly)
  : ng_(ng)
  , poly_(poly)
  , rnd_box_(ng_, bp::bounding_box(poly))

{
}

Point
RandomPolygonTrajectoryStartGenerator::generate()
{
  Point p = this->rnd_box_.generate();
  while (!bp::inside(this->poly_, bp::construct<pPoint>(p[0], p[1])))
    p = this->rnd_box_.generate();

  return p;
}

MultiplePolysRandomTrajectoryStartGenerator::
MultiplePolysRandomTrajectoryStartGenerator(std::mt19937_64& ng,
					    const std::vector<CompoundPolygon>& polys)
  : ng_(ng)
  , norm_cum_areas_()
  , randu_(0.0, 1.0)
  , poly_gens_()
{
  double sum = 0.0;
  for (const CompoundPolygon& poly: polys)
  {
    this->poly_gens_.push_back(RandomPolygonTrajectoryStartGenerator(ng, poly));

    double ar = bp::area(poly);
    if (this->norm_cum_areas_.empty())
      this->norm_cum_areas_.push_back(ar);
    else
      this->norm_cum_areas_.push_back(this->norm_cum_areas_.back() + ar);
    sum += ar;
  }

  for (unsigned i = 0; i < this->norm_cum_areas_.size(); ++i)
    this->norm_cum_areas_[i] /= sum;
}

Point
MultiplePolysRandomTrajectoryStartGenerator::generate()
{
  double p = this->randu_(this->ng_);

  for (unsigned i = 0; i < this->norm_cum_areas_.size(); ++i)
    if (p <= this->norm_cum_areas_[i])
      return this->poly_gens_[i].generate();
  return {};
}
