#include "segment.hh"

#include <cfloat>
#include <cassert>
#include <iostream>

template <size_t N>
Segment<N>::Segment(const Point<N>& p1, const Point<N>& p2)
  : p1_(p1)
  , p2_(p2)
{
}

template <>
double
Segment<2>::distance(const Point<2>& p) const
{
  double d = dist<2>(this->p1_, this->p2_);
  return (std::abs((this->p2_[1] - this->p1_[1]) * p[0] -
		   (this->p2_[0] - this->p1_[0]) * p[1] +
		   this->p2_[0] * this->p1_[1] - this->p2_[1] * this->p1_[0]) /
	  d);
}

template <>
double
Segment<3>::distance(const Point<3>& p) const
{
  return norm(cross(p - this->p1_, p - this->p2_)) / norm(this->vector());
}

template <size_t N>
double
Segment<N>::distance(const Point<N>& p) const
{
  (void) p;
  assert(false);
  return -1;
}

template <>
bool
Segment<2>::on_segment(const Point<2>& p) const
{
  //std::cout << this->distance(p) << std::endl;

  if (!colinear(this->p1_, this->p2_, p))
  {
    if (this->distance(p) > EPSILON)
      return false;
  }
  //return false;

  return (p[0] <= std::max(this->p1_[0], this->p2_[0]) &&
	  p[0] >= std::min(this->p1_[0], this->p2_[0]) &&
	  p[1] <= std::max(this->p1_[1], this->p2_[1]) &&
	  p[1] >= std::min(this->p1_[1], this->p2_[1]));
}

template <size_t N>
bool
Segment<N>::on_segment(const Point<N>& p) const
{
  (void) p;
  return false;
}

template <size_t N>
Vec<N>
Segment<N>::vector() const
{
  return this->p2_ - this->p1_;
}

template <>
Vec<2>
Segment<2>::normal() const
{
  Vec<2> v = this->vector();
  double n = norm<2>(v);
  return {-v[1] / n, v[0] / n};
}

template <size_t N>
Vec<N>
Segment<N>::normal() const
{
  return Vec<N> ();
}

template <size_t N>
void
Segment<N>::invert()
{
  Point<N> tmp = this->p1_;
  this->p1_= this->p2_;
  this->p2_ = tmp;
}

template <size_t N>
bool
Segment<N>::operator== (const Segment<N>& s2) const
{
  return this->p1_ == s2.p1() && this->p2_ == s2.p2();
}

template <size_t N>
bool
Segment<N>::intersect(const Segment<N>& s, Point<N>& inter_p) const
{
  (void) s;
  (void) inter_p;
  assert(false);
  return false;
}

// https://stackoverflow.com/a/1968345
template <>
bool
Segment<2>::intersect(const Segment<2>& seg, Point<2>& inter_p) const
{
  Vec<2> v1 = this->vector();
  Vec<2> v2 = seg.vector();

  double s = (-v1[1] * (this->p1_[0] - seg.p1()[0]) + v1[0] *
  	      (this->p1_[1] - seg.p1()[1])) / (-v2[0] * v1[1] + v1[0] * v2[1]);
  double t = ( v2[0] * (this->p1_[1] - seg.p1()[1]) - v2[1] *
  	      (this->p1_[0] - seg.p1()[0])) / (-v2[0] * v1[1] + v1[0] * v2[1]);

  if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
  {
    inter_p = this->p1_ + v1 * t;// + norm * EPSILON * 0.5;
    //round_to_precision<2>(inter_p);
    return true;
  }

  return false;
}

// Point
// Segment::intersection_point(const Segment& s1, const Segment& s2)
// {
//   Vec n1 = s1.vector();
//   Vec n2 = s2.vector();

//   double s = (-n1[1] * (s1.p1()[0] - s2.p1()[0]) +
// 	      n1[1] * (s1.p1()[1] - s2.p1()[1])) /
//     (-n2[0] * n1[1] + n1[0] * n2[1]);
//   double t = (n2[0] * (s1.p1()[1] - s2.p1()[1]) -
// 	      n2[1] * (s1.p1()[0] - s2.p1()[0])) /
//     (-n2[0] * n1[1] + n1[0] * n2[1]);

//   assert(s >= 0 && s <= 1 && t >= 0 && t <= 1);
//   return {s1.p1()[0] + t * n1[0], s1.p1()[1] + t * n1[1]};
// }

template <size_t N>
Point<N>
Segment<N>::reflect(const Segment<N>& s1, const Segment<N>& s2,
		    const Point<N>& inter_p)
{
  (void) s1;
  (void) s2;
  (void) inter_p;
  assert(false);
  return Point<N> ();
}

template<>
Point<2>
Segment<2>::reflect(const Segment<2>& s1, const Segment<2>& s2,
		    const Point<2>& inter_p)
{
  Vec<2> N = s2.normal();

  double s = (N[0] * (s1.p2()[0] - inter_p[0]) +
	      N[1] * (s1.p2()[1] - inter_p[1]));

  return s1.p2() - N * (2 * s);
}

template <size_t N>
Segment<N> Segment<N>::null()
{
  return Segment<N>(null_point<N>(), null_point<N>());
}

template <size_t N>
void
Segment<N>::shift_coords(const Point<N>& shifts)
{
  this->p1_ = this->p1_ + shifts;
  this->p2_ = this->p2_ + shifts;
}

template <size_t N>
Point<N>
Segment<N>::orthogonal_project(const Point<N>& p) const
{
  Vec<N> v = this->vector();
  Vec<N> ap = p - this->p1_;
  double t = dot(ap, v) / dot(v, v);
  return this->p1_ + v * t;
}

template <size_t N>
double
Segment<N>::orthogonal_project_t(const Point<N>& p) const
{
  Vec<N> v = this->vector();
  Vec<N> ap = p - this->p1_;
  return dot(ap, v) / dot(v, v);
}

template class Segment<2>;
template class Segment<3>;
