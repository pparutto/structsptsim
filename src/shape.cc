#include "shape.hh"

#include <cmath>

#include <cassert>
#include <iostream>

#include "collider.hh"
#include "utils.hh"

Shape::~Shape()
{
}

Box::Box()
  : lower_left_({NAN, NAN})
  , upper_right_({NAN, NAN})
  , width_(NAN)
  , height_(NAN)
{
}

Box::Box(const Point& lower_left, const Point& upper_right)
  : lower_left_(lower_left)
  , upper_right_(upper_right)
  , width_(upper_right[0] - lower_left[0])
  , height_(upper_right[1] - lower_left[1])
{
}

Box::~Box()
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

Polygon::~Polygon()
{
}

bool
my_colinear(const Segment& s1, const Segment& s2)
{
  return colinear(s2.p1(), s2.p2(), s1.p1()) &&
    colinear(s2.p1(), s2.p2(), s1.p2()) &&
    (s2.p1()[1] > s1.p1()[1] || s2.p2()[1] > s1.p1()[1]);
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

  //we need to pick a starting point that is not on a chain, so the
  //easiest solution is to look for a point that is on a segment that
  //does not collide with the polygon
  unsigned start = 0;
  while (start < this->pts_.size())
  {
    unsigned next = (start + 1) % this->pts_.size();
    Segment s2(this->pts_[start], this->pts_[next]);
    if (!my_colinear(s1, s2) && !Segment::intersect(s1, s2))
      break;
    ++start;
  }
  start = start % this->pts_.size();

  int start_chain = -1;
  int count = 0;
  unsigned i = start;
  do
  {
    int next = (i + 1) % this->pts_.size();

    Segment s2(this->pts_[i], this->pts_[next]);

    //   (within(s2, p) || within(s2, extreme) || within(s1, s2.p1()) || within(s1, s2.p2())) << std::endl;
    if (my_colinear(s1, s2))
    {
      if (start_chain < 0)
	start_chain = i;

      if (s2.on_segment(p))
	return border_is_inside;

      if (prev == NONE)
	--count;

      prev = COLIN;
      //++count;
    }
    else if (s2.on_segment(p))
      return border_is_inside;
    else if (Segment::intersect(s1, s2))
    {
      if (start_chain < 0)
	start_chain = i;

      Point ip = Segment::intersection_point(s1, s2);
      if (prev != NONE && ip == this->pts_[i])
      {
	//here we need to verify that the line does not just "touch"
	//the tip of the two segment without crossing them

	// ((this->pts_[start_chain][0] < s2.p1()[0] &&
	// this->pts_[next][0] < s2.p1()[0]) ||
	// (this->pts_[start_chain][0] > s2.p1()[0] &&
	// this->pts_[next][0] > s2.p1()[0]))))
	if (prev == COLIN || prev == TOUCH)
	{
	  //here we need to test that all points from the chain are on
	  //the same side of the intersection line s2.
	  bool ok = true;
	  double diff = this->pts_[start_chain][0] - s1.p1()[0];
	  int k = (start_chain + 1) % this->pts_.size();
	  while (k <= next)
	  {
	    double cur_diff = this->pts_[k][0] - s1.p1()[0];

	    if ((diff <= EPSILON && cur_diff > EPSILON) ||
		(diff >= -EPSILON && cur_diff < -EPSILON))
	    {
	      ok = false;
	      break;
	    }
	    k = (k + 1) % this->pts_.size();
	  }

	  if (ok)
	    count -= 2;
	  else
	    --count;
	}
	else
	  --count;
      }

      if (ip == this->pts_[i] || ip == this->pts_[next])
	prev = TOUCH;
      else
	prev = INTERSECT;

      ++count;
    }
    else
    {
      start_chain = -1;
      prev = NONE;
    }

    i = next;
  }
  while (i != start);

  return count % 2 == 1;
}

bool
Polygon::inside(const Point& p) const
{
  return this->my_inside(p, true);
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
    throw CollisionException(s1, "did not collide");

  return seg;
}

void Polygon::round_poly_pts()
{
  for (unsigned i = 0; i < this->pts_.size(); ++i)
  {
    this->pts_[i][0] = round_to_precision(this->pts_[i][0]);
    this->pts_[i][1] = round_to_precision(this->pts_[i][1]);
  }
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

CompoundPolygon::~CompoundPolygon()
{
}

bool
CompoundPolygon::inside(const Point& p) const
{
  if (!this->base_.inside(p))
    return false;

  for (const Polygon& poly: this->diffs_)
    if (poly.my_inside(p, false))
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

  for (const Polygon& poly: this->diffs_)
  {
    if (poly.my_inside(s1.p2(), false))
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


MultiplePolygon::
MultiplePolygon(const std::vector<CompoundPolygon>& polys)
  : polys_(polys)
{
  std::cout << "Loaded " << polys.size() << " polygons" << std::endl;
}

MultiplePolygon::~MultiplePolygon()
{
}

void
MultiplePolygon::apply_pxsize(double pxsize)
{
  for (CompoundPolygon& poly: this->polys_)
    poly.apply_pxsize(pxsize);
}

const std::vector<CompoundPolygon>&
MultiplePolygon::polys() const
{
  return this->polys_;
}

bool
MultiplePolygon::inside(const Point& p) const
{
  for (const CompoundPolygon& poly: this->polys_)
    if (poly.inside(p))
      return true;
  return false;
}

PointEnsemble
MultiplePolygon::boundary() const
{
  return {};
}

Box
MultiplePolygon::bounding_box() const
{
  return Box({0, 0}, {0, 0});
}

bool
MultiplePolygon::empty() const
{
  return this->polys_.empty();
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
