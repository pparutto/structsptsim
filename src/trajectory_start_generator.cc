#include "trajectory_start_generator.hh"

#include <iostream>
#include <cassert>

template <size_t N>
TrajectoryStartGenerator<N>::~TrajectoryStartGenerator()
{
}

template <size_t N>
FixedPointTrajectoryStartGenerator<N>::
FixedPointTrajectoryStartGenerator(const Point<N>& pt)
  : pt_(pt)
{
}

template <size_t N>
FixedPointTrajectoryStartGenerator<N>::
~FixedPointTrajectoryStartGenerator()
{
}

template <size_t N>
void
FixedPointTrajectoryStartGenerator<N>::
update_start_point(const Point<N>& new_pt)
{
  this->pt_ = new_pt;
}

template <size_t N>
Point<N>
FixedPointTrajectoryStartGenerator<N>::generate()
{
  return this->pt_;
}

template <size_t N>
RandomBoxTrajectoryStartGenerator<N>::
RandomBoxTrajectoryStartGenerator(std::mt19937_64& ng, const Box<N>& box)
  : ng_(ng)
  , box_(box)
  , randu_(0.0, 1.0)
{
}

template <size_t N>
RandomBoxTrajectoryStartGenerator<N>::
~RandomBoxTrajectoryStartGenerator()
{
}

template <size_t N>
Point<N>
RandomBoxTrajectoryStartGenerator<N>::generate()
{
  Point<N> res;
  for (size_t i = 0; i < N; ++i)
    res[i] = this->box_.min()[i] +
      this->randu_(this->ng_) * this->box_.dims()[i];
  return res;
}

template <size_t N>
RandomTrajectoryStartGenerator<N>::
RandomTrajectoryStartGenerator(std::mt19937_64& ng, const Shape<N>& shape)
  : ng_(ng)
  , shape_(shape)
  , rnd_box_(RandomBoxTrajectoryStartGenerator(ng, shape.bounding_box()))
{
}

template <size_t N>
RandomTrajectoryStartGenerator<N>::
~RandomTrajectoryStartGenerator()
{
}

template <size_t N>
Point<N>
RandomTrajectoryStartGenerator<N>::generate()
{
  Point<N> p = this->rnd_box_.generate();
  while (!this->shape_.inside(p))
    p = this->rnd_box_.generate();

  return p;
}

RandomBoxInPolyTrajectoryStartGenerator::
RandomBoxInPolyTrajectoryStartGenerator(std::mt19937_64& ng,
					const Shape<2>& poly,
					const Box<2>& box)
  : ng_(ng)
  , poly_(poly)
  , rnd_box_(RandomBoxTrajectoryStartGenerator(ng, box))
{
}

RandomBoxInPolyTrajectoryStartGenerator::
~RandomBoxInPolyTrajectoryStartGenerator()
{
}

Point<2>
RandomBoxInPolyTrajectoryStartGenerator::generate()
{
  Point<2> p = this->rnd_box_.generate();
  unsigned cpt = 5000;
  while (!this->poly_.inside(p))
  {
    p = this->rnd_box_.generate();
    if (cpt > 100000)
      assert(false);
    ++cpt;
  }

  return p;
}

MultiplePolysRandomTrajectoryStartGenerator::
MultiplePolysRandomTrajectoryStartGenerator(std::mt19937_64& ng,
					    const MultiplePolygon& polys)
  : ng_(ng)
  , norm_cum_areas_()
  , randu_(0.0, 1.0)
  , poly_gens_()
{
  double sum = 0.0;
  for (const CompoundPolygon& poly: polys.polys())
  {
    this->poly_gens_.push_back(RandomTrajectoryStartGenerator(ng, poly));

    double ar = fabs(poly.signed_area());
    if (this->norm_cum_areas_.empty())
      this->norm_cum_areas_.push_back(ar);
    else
      this->norm_cum_areas_.push_back(this->norm_cum_areas_.back() + ar);
    sum += ar;
  }

  for (unsigned i = 0; i < this->norm_cum_areas_.size(); ++i)
    this->norm_cum_areas_[i] /= sum;
}

MultiplePolysRandomTrajectoryStartGenerator::
~MultiplePolysRandomTrajectoryStartGenerator()
{
}

Point<2>
MultiplePolysRandomTrajectoryStartGenerator::generate()
{
  double p = this->randu_(this->ng_);

  for (unsigned i = 0; i < this->norm_cum_areas_.size(); ++i)
    if (p <= this->norm_cum_areas_[i])
      return this->poly_gens_[i].generate();
  return {};
}

template class RandomBoxTrajectoryStartGenerator<2>;
template class FixedPointTrajectoryStartGenerator<2>;
template class RandomTrajectoryStartGenerator<2>;

template class FixedPointTrajectoryStartGenerator<3>;
