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

template <size_t N>
const Point<N>&
Segment<N>::p1() const
{
  return this->p1_;
}

template <size_t N>
const Point<N>&
Segment<N>::p2() const
{
  return this->p2_;
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

template <size_t N>
double
Segment<N>::distance(const Point<N>& p) const
{
  (void) p;
  return -1;
}

template <>
bool
Segment<2>::on_segment(const Point<2>& p) const
{
  //std::cout << this->distance(p) << std::endl;

  if (!colinear(this->p1_, this->p2_, p))
  {
    if (this->distance(p) > DBL_EPSILON)
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
Point<N> minus(const Point<N>& p1, const Point<N>& p2)
{
  Point<N> res = p1;
  for (size_t i = 0; i < N; ++i)
    res[i] -= p2[i];
  return res;
}

template <size_t N>
Vec<N>
Segment<N>::vector() const
{
  Point<N> res = minus(this->p2_, this->p1_); //this->p2_ - this->p1_;
  return (Vec<N>) res;//this->p2_ - this->p1_;
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
Segment<N>
Segment<N>::invert() const
{
  return Segment<N>(this->p2_, this->p1_);
}

template <size_t N>
std::string
Segment<N>::plot_str(const std::string& col) const
{
  return "plot([" + this->p1_ + "], [" +
    this->p2_ + "], 'LineWidth', 2, 'Color', '" + col + "');";
}

template <>
bool
Segment<2>::intersect(const Segment<2>& s1, const Segment<2>& s2)
{
  Orientation os[] = {orientation(s1.p1(), s1.p2(), s2.p1()),
		      orientation(s1.p1(), s1.p2(), s2.p2()),
		      orientation(s2.p1(), s2.p2(), s1.p1()),
		      orientation(s2.p1(), s2.p2(), s1.p2())};

  if (os[0] != os[1] && os[2] != os[3])
    return true;

  //Special cases
  if ((os[0] == COLINEAR && s1.on_segment(s2.p1())) ||
      (os[1] == COLINEAR && s1.on_segment(s2.p2())) ||
      (os[2] == COLINEAR && s2.on_segment(s1.p1())) ||
      (os[3] == COLINEAR && s2.on_segment(s1.p2())))
    return true;

  return false;
}

template <size_t N>
bool
Segment<N>::intersect(const Segment<N>& s1, const Segment<N>& s2)
{
  (void) s1;
  (void) s2;
  return false;
}


Point<2> project_on_line(const Point<2>& p, const Segment<2>& s)
{
  // get dot product of e1, e2
  Vec<2> e1 = s.vector();
  Vec<2> e2 = Segment<2>(p, s.p1()).vector();
  double valDp = dot<2>(e1, e2);

  double l = norm<2>(e1);
  return {s.p1()[0] + (valDp * e1[0]) / l,
	  s.p1()[1] + (valDp * e1[1]) / l};
}

template <size_t N>
Point<N>
Segment<N>::intersection_point(const Segment<N>& s1, const Segment<N>& s2)
{
  (void) s1;
  (void) s2;
  return Point<N> ();
}

//the two segments must already be intersecting
//check this with Segment::intersect
template <>
Point<2>
Segment<2>::intersection_point(const Segment<2>& s1, const Segment<2>& s2)
{
  Vec<2> e = s1.vector();
  Vec<2> f = s2.vector();
  Vec<2> p = {-e[1], e[0]};

  double inter = f[0] * p[0] + f[1] * p[1];

  double h1 = ((s1.p1()[0] - s2.p1()[0]) * p[0] +
	       (s1.p1()[1] - s2.p1()[1]) * p[1]) / inter;

  Point<2> res = {s2.p1()[0] + f[0] * h1, s2.p1()[1] + f[1] * h1};

  return res;
}

// Point
// Segment::intersection_point(const Segment& s1, const Segment& s2)
// {
//   Vec n1 = s1.vector();
//   Vec n2 = s2.vector();

//   double s = (-n1[1] * (s1.p1()[0] - s2.p1()[0]) +
// 	      n1[1] * (s1.p1()[1] - s2.p1()[1])) /
//     (-n2[0] * n1[1] + n1[1] * n2[1]);
//   double t = (n2[0] * (s1.p1()[1] - s2.p1()[1]) -
// 	      n2[1] * (s1.p1()[0] - s2.p1()[0])) /
//     (-n2[0] * n1[1] + n1[0] * n2[1]);

//   assert(s >= 0 && s <= 1 && t >= 0 && t <= 1);
//   return {s1.p1()[0] + t * n1[0], s1.p1()[1] + t * n1[1]};
// }

template <>
Point<2>
Segment<2>::reflect(const Segment<2>& s1, const Segment<2>& s2)
{
  //reflect s1 on s2
  Point<2> inter_p = Segment<2>::intersection_point(s1, s2);
  Vec<2> N = s2.normal();

  double s = (N[0] * (s1.p2()[0] - inter_p[0]) +
	      N[1] * (s1.p2()[1] - inter_p[1]));


  return {s1.p2()[0] - 2 * s * N[0], s1.p2()[1] - 2 * s * N[1]};
}

template <size_t N>
Point<N>
Segment<N>::reflect(const Segment<N>& s1, const Segment<N>& s2)
{
  (void) s1;
  (void) s2;
  return Point<N> ();
}

template <size_t N>
std::ostream&
operator<< (std::ostream& os, const Segment<N>& seg)
{
  os << seg.p1() << "; " << seg.p2();
  return os;
}
