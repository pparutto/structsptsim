#include "shape.hh"

#include <cmath>

#include <cassert>
#include <iostream>
#include <algorithm>

#include "collider.hh"

template <size_t N>
Shape<N>::~Shape()
{
}

template <size_t N>
Box<N>::Box()
  : min_()
  , max_()
  , dims_()
{
}

template <size_t N>
Box<N>::Box(const Point<N>& min, const Point<N>& max)
  : min_(min)
  , max_(max)
  , dims_()
{
  for (size_t i = 0; i < N; ++i)
    this->dims_[i] = this->max_[i] - this->min_[i];
}

template <size_t N>
Box<N>::~Box()
{
}

template <size_t N>
bool
Box<N>::inside(const Point<N>& p) const
{
  for (size_t i = 0; i < N; ++i)
    if (p[i] < this->min_[i] || p[i] > this->max_[i])
      return false;
  return true;
}

// template <size_t N>
// PointEnsemble<2>
// Box::boundary() const
// {
//   PointEnsemble<2> res;
//   res.push_back(this->lower_left_);
//   res.push_back({this->lower_left_[0] + this->width_, this->lower_left_[1]});
//   res.push_back(this->upper_right_);
//   res.push_back({this->lower_left_[0], this->lower_left_[1] + this->height_});
//   res.push_back(this->lower_left_);
//   return res;
// }

template <size_t N>
Box<N>
Box<N>::bounding_box() const
{
  return Box(this->min_, this->max_);
}


Triangle3D::
Triangle3D(const Point<3>& p1, const Point<3>& p2, const Point<3>& p3)
  : p1_(p1)
  , p2_(p2)
  , p3_(p3)
  , normal_()
{
  this->normal_ = cross(p2 - p1, p3 - p1);
  this->normal_ = this->normal_ / norm(this->normal_);
}

Box<3>
Triangle3D::bounding_box() const
{
  Point<3> min;
  Point<3> max;

  for (size_t i = 0; i < 3; ++i)
  {
    min[i] = std::min({this->p1_[i], this->p2_[i], this->p3_[i]});
    max[i] = std::max({this->p1_[i], this->p2_[i], this->p3_[i]});
  }

  return Box<3> (min, max);
}

bool
Triangle3D::intersect(const Segment<3>& seg, Point<3>& inter_p) const
{
  Vec<3> ray = seg.vector();
  Vec<3> edge1 = this->p2_ - this->p1_;
  Vec<3> edge2 = this->p3_ - this->p1_;

  Vec<3> h = cross(ray, edge2);

  double a = dot(edge1, h);
  if (a > -EPSILON && a < EPSILON)
    return false;    // ray parallel to triangle.

  double f = 1.0 / a;
  Vec<3> s = seg.p1() - this->p1_;

  double u = f * dot(s, h);
  if (u < 0.0 || u > 1.0)
    return false;

  Vec<3> q = cross(s, edge1);
  double v = f * dot(ray, q);
  if (v < 0.0 || u + v > 1.0)
    return false;

  double t = f * dot(edge2, q);
  if (t > EPSILON) // ray intersection
  {
    inter_p = seg.p1() + ray * t;
    return true;
  }
  return false;
}


double orientation(const PointEnsemble<2>& pts)
{
  return ((pts[1][0] - pts[0][0]) * (pts[2][1] - pts[0][1]) -
	  (pts[2][0] - pts[0][0]) * (pts[1][1] - pts[0][1])) < 0;
}

bool within(const Segment<2>& s, const Point<2> p)
{
  double mx = std::min(s.p1()[0], s.p2()[0]);
  double Mx = std::max(s.p1()[0], s.p2()[0]);
  double my = std::min(s.p1()[1], s.p2()[1]);
  double My = std::max(s.p1()[1], s.p2()[1]);
  return p[0] >= mx && p[0] <= Mx && p[1] >= my && p[1] <= My;
}

Polygon::Polygon(const PointEnsemble<2>& pts)
  : pts_(pts)
{
}

Polygon::~Polygon()
{
}

bool
my_colinear(const Segment<2>& s1, const Segment<2>& s2)
{
  return colinear(s2.p1(), s2.p2(), s1.p1()) &&
    colinear(s2.p1(), s2.p2(), s1.p2()) &&
    (s2.p1()[1] > s1.p1()[1] || s2.p2()[1] > s1.p1()[1]);
}


bool feq(double a, double b, double tol)
{
  return std::abs(a - b) < tol;
}

bool
Polygon::my_inside(const Point<2>& p, bool border_is_inside) const
{
  int n = this->pts_.size();
  double ax = 0.0;
  double ay = this->pts_[n-2][0];
  double bx = this->pts_[n-1][0] - p[0];
  double by = this->pts_[n-1][1] - p[1];

  bool lup = by > ay;
  for (int i = 0; i < n; ++i)
  {
    ax = bx;
    ay = by;
    bx = this->pts_[i][0] - p[0];
    by = this->pts_[i][1] - p[1];

    if (feq(ay, by, EPSILON))
      continue;
    lup = by > ay;
  }

  int depth = 0;
  for (int i = 0; i < n; ++i)
  {
    ax = bx;
    ay = by;
    bx = this->pts_[i][0] - p[0];
    by = this->pts_[i][1] - p[1];

    if ((ay < 0 && by < 0) ||
	(ay > 0 && by > 0) ||
	(ax < 0 && bx < 0))
      continue;

    if (feq(ay, by, EPSILON) && std::min(ax, bx) <= 0)
      return true;

    if (feq(ay, by, EPSILON))
      continue;

    double lx = ax + (bx - ax) * (-ay) / (by - ay);
    if (feq(lx, 0, EPSILON))
      return border_is_inside;

    if (lx > 0)
      ++depth;
    if (feq(ay, 0, EPSILON) && lup && by > ay)
      --depth;
    if (feq(ay, 0, EPSILON) && !lup && by < ay)
      --depth;

    lup = by > ay;
  }


  return depth % 2 == 1;
}


bool
Polygon::inside(const Point<2>& p) const
{
  return this->my_inside(p, true);
}

bool
Polygon::inside(const Polygon& poly) const
{
  for (const Point<2>& pt: this->pts_)
    if (!poly.inside(pt))
      return false;

  return true;
}

// PointEnsemble<2>
// Polygon::boundary() const
// {
//   return this->pts_;
// }

Box<2>
Polygon::bounding_box() const
{
  Point<2> ll = {(double) NAN, (double) NAN};
  Point<2> ur = {(double) NAN, (double) NAN};

  for (const Point<2>& p: this->pts_)
  {
    ll[0] = std::isnan(ll[0]) || p[0] < ll[0]? p[0] : ll[0];
    ll[1] = std::isnan(ll[1]) || p[1] < ll[1]? p[1] : ll[1];
    ur[0] = std::isnan(ur[0]) || p[0] > ur[0]? p[0] : ur[0];
    ur[1] = std::isnan(ur[1]) || p[1] > ur[1]? p[1] : ur[1];
  }

  return Box<2>(ll, ur);
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

bool
Polygon::intersect(const Segment<2>& s1,
		   Point<2>& inter_p, Segment<2>& inter_s) const
{
  Point<2> tmp_inter_p;
  bool already = false;
  int i = 0;
  do
  {
    int next = (i + 1) % this->pts_.size();

    Segment<2> s2(this->pts_[i], this->pts_[next]);

    if (!s2.on_segment(s1.p1()) && s1.intersect(s2, tmp_inter_p))
    {
      //if s1 intersect with multiple segments, we keep the one with
      //the closest intersection point to the start.
      if (already)
      {
	if (dist(s1.p1(), tmp_inter_p) < dist(s1.p1(), inter_p))
	{
	  inter_s = s2;
	  inter_p = tmp_inter_p;
	}
      }
      else
      {
	already = true;
	inter_p = tmp_inter_p;
	inter_s = s2;
      }
    }

    i = next;
  }
  while (i != 0);

  return already;
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


Polygon3D::Polygon3D(const std::vector<Triangle3D>& triangles)
  : triangles_(triangles)
{
}


Polygon3D::~Polygon3D()
{
}

bool
Polygon3D::inside(const Point<3>& p) const
{
  (void) p;
  assert(false);
  return false;
}

Box<3>
Polygon3D::bounding_box() const
{
  return Box<3>();
}


bool
Polygon3D::intersect(const Segment<3>& seg,
		     Point<3>& inter_p, Triangle3D& inter_t) const
{
  Point<3> tmp_inter_p;
  bool already = false;
  for(const Triangle3D& tri: this->triangles_)
  {
    if (tri.intersect(seg, tmp_inter_p))
    {
      if (already)
      {
	//new triangle is closer than previously intersected one
	if (dist(seg.p1(), tmp_inter_p) < dist(seg.p1(), inter_p))
	{
	  inter_p = tmp_inter_p;
	  inter_t = tri;
	}
      }
      else
      {
	already = true;
	inter_p = tmp_inter_p;
	inter_t = tri;
      }
    }
  }

  return already;
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
CompoundPolygon::inside(const Point<2>& p) const
{
  if (!this->base_.inside(p))
    return false;

  for (const Polygon& poly: this->diffs_)
    if (poly.my_inside(p, false))
      return false;

  return true;
}

// PointEnsemble<2>
// CompoundPolygon::boundary() const
// {
//   return this->base_.boundary();
// }

Box<2>
CompoundPolygon::bounding_box() const
{
  return this->base_.bounding_box();
}

const PointEnsemble<2>&
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

bool
CompoundPolygon::intersect(const Segment<2>& seg,
			   Point<2>& inter_p, Segment<2>& inter_s) const
{
  if (this->base_.intersect(seg, inter_p, inter_s))
    return true;

  for (const Polygon& poly: this->diffs_)
    if (poly.intersect(seg, inter_p, inter_s))
	return true;

  return false;

  // WARNINGGGGGGGGGGGGGGGGGGGGGGGGGGGGGg
  // /!\ need to make sure the normals are correct when loading polygons
  //we always want the normal to point inward
  //i.e toward the starting point of s1
  //if (dot(s1.vector(), res.normal()) > 0)
  //  res.invert();
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
MultiplePolygon::inside(const Point<2>& p) const
{
  for (const CompoundPolygon& poly: this->polys_)
    if (poly.inside(p))
      return true;
  return false;
}

// PointEnsemble<2>
// MultiplePolygon::boundary() const
// {
//   return {};
// }

Box<2>
MultiplePolygon::bounding_box() const
{
  return Box<2>({0, 0}, {0, 0});
}

bool
MultiplePolygon::empty() const
{
  return this->polys_.empty();
}



template <size_t N>
std::ostream&
operator<< (std::ostream& os, const Box<N>& box)
{
  return os << box.min() << "; " << box.max() << std::endl;
}

std::ostream&
operator<< (std::ostream& os, const Polygon& poly)
{
  for (const Point<2>& p: poly.pts())
    os << p << ";" << std::endl;
  os << poly.pts()[0] << ";" << std::endl;
  return os;
}

template class Box<2>;
template class Box<3>;

template std::ostream& operator<<<2> (std::ostream& os, const Box<2>& box);
