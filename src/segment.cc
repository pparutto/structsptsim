#include "segment.hh"

#include <cfloat>
#include <cassert>
#include <iostream>

Segment::Segment(const Point& p1, const Point& p2)
  : p1_(p1)
  , p2_(p2)
{
}

const Point&
Segment::p1() const
{
  return this->p1_;
}

const Point&
Segment::p2() const
{
  return this->p2_;
}

double
Segment::distance(const Point& p) const
{
  double d = dist(this->p1_, this->p2_);
  return (std::abs((this->p2_[1] - this->p1_[1]) * p[0] -
		   (this->p2_[0] - this->p1_[0]) * p[1] +
		   this->p2_[0] * this->p1_[1] - this->p2_[1] * this->p1_[0]) /
	  d);
}

bool
Segment::on_segment(const Point& p) const
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

Vec
Segment::vector() const
{
  return this->p2_ - this->p1_;
}

Vec
Segment::normal() const
{
  Vec v = this->vector();
  double n = norm(v);
  return {-v[1] / n, v[0] / n};
}

Segment
Segment::invert() const
{
  return Segment(this->p2_, this->p1_);
}

std::string
Segment::plot_str(const std::string& col) const
{
  return "plot([" + std::to_string(this->p1_[0]) + "," +
    std::to_string(this->p2_[0]) + "], [" +
    std::to_string(this->p1_[1]) + "," + std::to_string(this->p2_[1]) +
    "], 'LineWidth', 2, 'Color', '" + col + "');";
}

bool
Segment::intersect(const Segment& s1, const Segment& s2)
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


Point project_on_line(const Point& p, const Segment& s)
{
  // get dot product of e1, e2
  Vec e1 = s.vector();
  Vec e2 = Segment(p, s.p1()).vector();
  double valDp = dot(e1, e2);

  double l = norm(e1);
  return {s.p1()[0] + (valDp * e1[0]) / l,
	  s.p1()[1] + (valDp * e1[1]) / l};
}
//the two segments must already be intersecting
//check this with Segment::intersect
Point
Segment::intersection_point(const Segment& s1, const Segment& s2)
{
  Vec e = s1.vector();
  Vec f = s2.vector();
  Vec p = {-e[1], e[0]};

  double inter = f[0] * p[0] + f[1] * p[1];

  double h1 = ((s1.p1()[0] - s2.p1()[0]) * p[0] +
	       (s1.p1()[1] - s2.p1()[1]) * p[1]) / inter;

  Point res = {s2.p1()[0] + f[0] * h1, s2.p1()[1] + f[1] * h1};

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

Point
Segment::reflect(const Segment& s1, const Segment& s2)
{
  //reflect s1 on s2
  Point inter_p = Segment::intersection_point(s1, s2);
  Vec N = s2.normal();

  double s = (N[0] * (s1.p2()[0] - inter_p[0]) +
	      N[1] * (s1.p2()[1] - inter_p[1]));


  return {s1.p2()[0] - 2 * s * N[0], s1.p2()[1] - 2 * s * N[1]};
}

std::ostream&
operator<< (std::ostream& os, const Segment& seg)
{
  os << seg.p1()[0] << " " << seg.p1()[1] << "; "
     << seg.p2()[0] << " " << seg.p2()[1];
  return os;
}
