#include "shape.hh"

#include <cmath>

#include <cassert>
#include <iostream>

Box::Box(const Point& lower_left, const Point& upper_right)
  : lower_left_(lower_left)
  , upper_right_(upper_right)
  , width_(upper_right[0] - lower_left[0])
  , height_(upper_right[1] - lower_left[1])
{
}

bool
Box::inside(const Point& p) const
{
  return (p[0] >= this->lower_left_[0] && p[0] <= this->upper_right_[0] &&
	  p[1] >= this->lower_left_[1] && p[1] <= this->upper_right_[1]);
}

PointEnsemble
Box::boundary() const
{
  PointEnsemble res;
  res.push_back(this->lower_left_);
  res.push_back({this->lower_left_[0] + this->width_, this->lower_left_[1]});
  res.push_back(this->upper_right_);
  res.push_back({this->lower_left_[0], this->lower_left_[1] + this->height_});
  res.push_back(this->lower_left_);
  return res;
}

Box
Box::bounding_box() const
{
  return Box(this->lower_left_, this->upper_right_);
}

Polygon::Polygon(const PointEnsemble& pts)
  : pts_(pts)
{
}

bool
Polygon::inside(const Point& p) const
{
  if (this->pts_.size() < 3)
    return false;

  double INF = 10000.0;
  Point extreme = {INF, p[1]};
  Segment s1(p, extreme);

  int count = 0;
  int i = 0;
  do
  {
    int next = (i + 1) % this->pts_.size();

    Segment s2(this->pts_[i], this->pts_[next]);
    if (Segment::intersect(s1, s2))
    {
      if (colinear(this->pts_[i], this->pts_[next], p))
	return s2.on_segment(p);
      ++count;
    }

    i = next;
  }
  while (i != 0);

  return count % 2 == 1;
}

PointEnsemble
Polygon::boundary() const
{
  return this->pts_;
}

Box
Polygon::bounding_box() const
{
  Point ll = {(double) NAN, (double) NAN};
  Point ur = {(double) NAN, (double) NAN};

  for (const Point& p: this->pts_)
  {
    ll[0] = std::isnan(ll[0]) || p[0] < ll[0]? p[0] : ll[0];
    ll[1] = std::isnan(ll[1]) || p[1] < ll[1]? p[1] : ll[1];
    ur[0] = std::isnan(ur[0]) || p[0] > ur[0]? p[0] : ur[0];
    ur[1] = std::isnan(ur[1]) || p[1] > ur[1]? p[1] : ur[1];
  }

  return Box(ll, ur);
}

Segment
Polygon::intersect_with(const Segment& s1) const
{
  bool already = false;
  Segment seg({0, 0}, {0, 0});
  int i = 0;
  do
  {
    int next = (i + 1) % this->pts_.size();

    Segment s2(this->pts_[i], this->pts_[next]);
    if (Segment::intersect(s1, s2))
    {
      assert(!already); //segment intersects poly more than once
      already = true;
      seg = s2;
    }

    i = next;
  }
  while (i != 0);

  assert(already);
  return seg;
}

std::ostream&
operator<< (std::ostream& os, const Polygon& poly)
{
  for (const Point& p: poly.pts())
    os << p << std::endl;
  return os;
}
