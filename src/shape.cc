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

double orientation(const PointEnsemble& pts)
{
  return ((pts[1][0] - pts[0][0]) * (pts[2][1] - pts[0][1]) -
	  (pts[2][0] - pts[0][0]) * (pts[1][1] - pts[0][1])) < 0;
}

bool within(const Segment& s, const Point p)
{
  double mx = std::min(s.p1()[0], s.p2()[0]);
  double Mx = std::max(s.p1()[0], s.p2()[0]);
  double my = std::min(s.p1()[1], s.p2()[1]);
  double My = std::max(s.p1()[1], s.p2()[1]);
  return p[0] >= mx && p[0] <= Mx && p[1] >= my && p[1] <= My;
}

Polygon::Polygon(const PointEnsemble& pts)
  : pts_(pts)
{
}

bool
Polygon::my_inside(const Point& p, bool border_is_inside) const
{
  double INF = 10000.0;
  Point extreme = {p[0], INF};
  Segment s1(p, extreme);
  int NONE = 0;
  int INTERSECT = 1;
  int COLIN = 2;
  int TOUCH = 3;

  int prev = NONE;
  Segment s2(this->pts_[this->pts_.size()-1], this->pts_[0]);
  if (colinear(s2.p1(), s2.p2(), p) && !s2.on_segment(p))
    prev = COLIN;
  else if(!s2.on_segment(p) && Segment::intersect(s1, s2))
  {
    if (Segment::intersection_point(s1, s2) == this->pts_[0])
      prev = TOUCH;
    else
      prev = INTERSECT;
  }


  int count = 0;
  int i = 0;
  do
  {
    int next = (i + 1) % this->pts_.size();

    Segment s2(this->pts_[i], this->pts_[next]);

    if (colinear(s2.p1(), s2.p2(), p) && colinear(s2.p1(), s2.p2(), extreme) &&
	(within(s2, p) || within(s2, extreme) || within(s1, s2.p1())
	 || within(s1, s2.p2())))
    {
      if (s2.on_segment(p))
	return border_is_inside;

      if (prev != NONE)
	--count;

      prev = COLIN;
      ++count;
    }
    else if (s2.on_segment(p))
      return border_is_inside;
    else if (Segment::intersect(s1, s2))
    {
      if (prev != NONE && Segment::intersection_point(s1, s2) == this->pts_[i])
      {
	//here we need to verify that the line does not just "touch"
	//the tip of the two segment without crossing them
	if (prev == TOUCH &&
	    ((this->pts_[i-1][0] < s2.p1()[0] &&
	      this->pts_[next][0] < s2.p1()[0]) ||
	     (this->pts_[i-1][0] > s2.p1()[0] &&
	      this->pts_[next][0] > s2.p1()[0])))
	  count -= 2;
	else
	  --count;
      }

      if (Segment::intersection_point(s1, s2) == this->pts_[i] ||
	  Segment::intersection_point(s1, s2) == this->pts_[next])
	prev = TOUCH;
      else
	prev = INTERSECT;

      ++count;
    }
    else
      prev = NONE;

    i = next;
  }
  while (i != 0);

  //std::cout << p[0] << " " << p[1] << " " << count << std::endl;
  return count % 2 == 1;
}

bool
Polygon::inside(const Point& p) const
{
  return this->my_inside(p, true);
}

bool Polygon::inside2(const Point& p, bool border_is_inside) const
{
  return this->my_inside(p, border_is_inside);
}

bool
Polygon::inside(const Polygon& poly) const
{
  for (const Point& pt: this->pts_)
    if (!poly.inside(pt))
      return false;
  return true;
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

void
Polygon::apply_pxsize(double pxsize)
{
  for (unsigned i = 0; i < this->pts_.size(); ++i)
  {
    this->pts_[i][0] *= pxsize;
    this->pts_[i][1] *= pxsize;
  }
}

Segment
Polygon::intersect_with(const Segment& s1) const
{
  bool already = false;
  Segment seg({0, 0}, {0, 0});
  Point inter_p = {0, 0};
  int i = 0;
  do
  {
    int next = (i + 1) % this->pts_.size();

    Segment s2(this->pts_[i], this->pts_[next]);

    if (!s2.on_segment(s1.p1()) && Segment::intersect(s1, s2))
    {
      if (already)
      {
	Point pp = Segment::intersection_point(s1, s2);
	if (dist(s1.p1(), pp) < dist(s1.p1(), inter_p))
	{
	  seg = s2;
	  inter_p = pp;
	}
      }
      else
      {
	already = true;
	inter_p = Segment::intersection_point(s1, s2);
	seg = s2;
      }
    }

    i = next;
  }
  while (i != 0);

  if (!already)
    throw std::runtime_error("Segment did not collide with polygon");

  return seg;
}

double
Polygon::signed_area() const
{
  double res = 0;
  unsigned i = 0;
  do
  {
    unsigned next = (i + 1) % this->pts_.size();

    res += (this->pts_[i][0] * this->pts_[next][1] -
	    this->pts_[next][0] * this->pts_[i][1]);
    i = next;
  }
  while (i != 0);

  return res;
}


CompoundPolygon::
CompoundPolygon(const Polygon& base, const std::vector<Polygon>& diffs)
  : base_(base)
  , diffs_(diffs)
{
}

bool
CompoundPolygon::inside(const Point& p) const
{
  if (!this->base_.inside(p))
    return false;

  for (const Polygon& poly: this->diffs_)
    if (poly.inside2(p, false))
      return false;

  return true;
}

PointEnsemble
CompoundPolygon::boundary() const
{
  return this->base_.boundary();
}

Box
CompoundPolygon::bounding_box() const
{
  return this->base_.bounding_box();
}

const PointEnsemble&
CompoundPolygon::pts() const
{
  return this->base_.pts();
};

void
CompoundPolygon::apply_pxsize(double pxsize)
{
  this->base_.apply_pxsize(pxsize);
  for (unsigned i = 0; i < this->diffs_.size(); ++i)
    this->diffs_[i].apply_pxsize(pxsize);
}

Segment
CompoundPolygon::intersect_with(const Segment& s1) const
{
  bool in_diff = false;
  const Polygon* diff = nullptr;

  if (this->inside(s1.p1()) == this->inside(s1.p2()))
    throw std::runtime_error("Points are in same location");
  //assert(this->inside(s1.p1()) != this->inside(s1.p2()));

  for (const Polygon& poly: this->diffs_)
  {
    if (poly.inside2(s1.p2(), false))
    {
      in_diff = true;
      diff = &poly;
      break;
    }
  }

  Segment res({0, 0}, {0, 0});
  if (!in_diff)
    res = this->base_.intersect_with(s1);
  else
    res = diff->intersect_with(s1);
  //we always want the normal to point inward
  //i.e toward the starting point of s1

  if (dot(s1.vector(), res.normal()) > 0)
    res = res.invert();

  return res;

}

double
CompoundPolygon::signed_area() const
{
  double res = fabs(this->base_.signed_area());

  for (const Polygon& poly: diffs_)
    res -= fabs(poly.signed_area());
  return res;
}


std::ostream&
operator<< (std::ostream& os, const Box& box)
{
  Point ll = box.lower_left();
  Point tr = box.upper_right();
  os << ll[0] << " " << ll[1] << ";"
     << tr[0] << " " << tr[1] << std::endl;
  return os;
}

std::ostream&
operator<< (std::ostream& os, const Polygon& poly)
{
  for (const Point& p: poly.pts())
    os << p << ";" << std::endl;
  os << poly.pts()[0] << ";" << std::endl;
  return os;
}
