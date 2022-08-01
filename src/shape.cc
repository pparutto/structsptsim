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

template <size_t N>
bool
Box<N>::inside(const Segment<N>& seg) const
{
  return this->inside(seg.p1()) && this->inside(seg.p2());
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

template <size_t N>
void
Box<N>::set_min_v(double v, unsigned idx)
{
  this->min_[idx] = v;
}

template <size_t N>
void
Box<N>::set_max_v(double v, unsigned idx)
{
  this->max_[idx] = v;
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


Cylinder::Cylinder(const Segment<3>& base, double r)
  : base_(base)
  , r_(r)
{
}

bool
Cylinder::inside(const Point<3>& p) const
{
  //std::cout << this->base_.distance(p) << " " <<  this->r_ << std::endl;
  return this->base_.distance(p) <= this->r_;
}

Box<3>
Cylinder::bounding_box() const
{
  // /!\ TODO
  assert(false);
  return Box<3>();
}

bool
Cylinder::intersect_side(const Segment<3>& s, Point<3>& inter_p) const
{
  double t = this->base_.orthogonal_project_t(s.p2());

  if (t >= 0 && t <= 1)
    return false;

  Point<3> p1 = s.p1();
  Point<3> p2 = s.p1() + s.normal() * 0.4;
  Point<3> p3 = s.p1() + s.normal() * 0.7;
  if (t > 1)
  {
    p1 = s.p2();
    p2 = s.p2() + s.normal() * 0.4;
    p3 = s.p2() + s.normal() * 0.7;
  }
  Plane p(p1, p2, p3);

  if (!p.intersect(s, inter_p) ||
      this->base_.distance(inter_p) > this->r_)
    return false;

  return true;
}


bool
Cylinder::intersect(const Segment<3>& s, Point<3>& inter_p) const
{
  if (this->inside(s.p2()))
    return false;

  Vec<3> v = s.vector();
  Point<3> p0 = s.p1();
  Point<3> p1 = this->base_.p1();
  Point<3> p2 = this->base_.p2();

  Vec<3> D1 = p0 - p1;
  Vec<3> D2 = p0 - p2;

  Vec<3> alpha = {(D1[1] - D2[1]) * v[2] + (D2[2] - D1[2]) * v[1],
		  (D1[2] - D2[2]) * v[0] + (D2[0] - D1[0]) * v[2],
		  (D1[0] - D2[0]) * v[1] + (D2[1] - D1[1]) * v[0]};
  Vec<3> beta = {D1[1] * D2[2] - D1[2] * D2[1],
		 D1[2] * D2[0] - D1[0] * D2[2],
		 D1[0] * D2[1] - D1[1] * D2[0]};

  double A = alpha[0] * alpha[0] + alpha[1] * alpha[1] + alpha[2] * alpha[2];
  double B = 2 * (alpha[0] * beta[0] + alpha[1] * beta[1] + alpha[2] * beta[2]);
  double C = beta[0] * beta[0] + beta[1] * beta[1] + beta[2] * beta[2] -
    norm(p1 - p2) * norm(p1 - p2) * this->r_ * this->r_;

  double delta = B * B - 4 * A * C;
  //std::cout << B << " " << A << " " << C << " " << delta << std::endl;
  if (delta < -EPSILON)
    return false;

  if (delta < EPSILON)
    inter_p = p0 + v * (-B / (2 * A));
  else
  {
    double s_delta = sqrt(delta);
    double t1 = (-B - s_delta) / (2 * A);
    double t2 = (-B + s_delta) / (2 * A);

    double t = NAN;
    if (t1 < -EPSILON || t1 > 1 + EPSILON)
      t = t2;
    else if (t2 < -EPSILON || t2 > 1 + EPSILON)
      t = t2;
    else
      t = t1 < t2 ? t1 : t2;

    //std::cout << t << std::endl;
    inter_p = p0 + v * t;

    //std::cout << this->base_.distance(s.p1()) << " " << this->base_.distance(s.p2()) << " " << this->base_.distance(inter_p) << std::endl;
      //<< " @ " << t1 << " " << t2 << " " << t << std::endl;
  }

  return true;
}

Vec<3> Cylinder::normal(const Point<3>& p) const
{
  Point<3> proj_p = this->base_.orthogonal_project(p);
  Vec<3> n = p - proj_p;
  return n / norm(n);
}



Plane::Plane(const Point<3>& p1, const Point<3>& p2, const Point<3>& p3)
  : p1_(p1)
  , p2_(p2)
  , p3_(p3)
{
}

bool
Plane::inside(const Point<3>& p) const
{
  (void) p;
  assert(false);
  return false;
}

Box<3>
Plane::bounding_box() const
{
  assert(false);
  return Box<3> ();
}


bool
Plane::intersect(const Segment<3>& s, Point<3>& inter_p) const
{
  Vec<3> p12 = this->p2_ - this->p1_;
  Vec<3> p13 = this->p3_ - this->p1_;
  Vec<3> v = s.vector();

  double det = -dot(v, cross(p12, p13));

  if (det >= -EPSILON && det <= EPSILON)
    return false;

  double t = dot(cross(p12, p13), s.p1() - this->p1_) / det;

  if (t < 0 || t > 1)
    return false;

  inter_p = s.p1() + v * t;
  return true;
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
Polygon::check_normals(const Polygon& poly, bool inner_normals)
{
  double DELTA_DISP = 0.005;
  for (size_t i = 0; i < poly.pts().size(); ++i)
  {
    int next = (i + 1) % poly.pts().size();

    Segment<2> seg(poly.pts()[i], poly.pts()[next]);
    Vec<2> norm = seg.normal();

    Point<2> p = seg.p1() + seg.vector() / 2 + norm * DELTA_DISP;
    round_to_precision<2>(p);
    if (poly.inside(p) != inner_normals)
      return false;
  }
  return true;
}

Polygon Polygon::reverse(const Polygon& poly)
{
  PointEnsemble<2> pe;
  for (int i = poly.pts().size() - 1; i >= 0; --i)
    pe.push_back(poly.pts()[i]);
  return Polygon(pe);
}

// bool
// Polygon::my_inside(const Point<2>& p, bool border_is_inside) const
// {
//   size_t n = this->pts_.size();
//   Segment seg_inf(p, {p[0], p[1] + 99999});
//   Segment<2> seg;
//   Point<2> inter_p;
//   int cnt = 0;
//   for (size_t i = 0; i < n; ++i)
//   {
//     int next = (i + 1) % n;
//     seg = Segment<2> (this->pts_[i], this->pts_[next]);

//     if (seg.distance(p) < EPSILON)
//       return border_is_inside;

//     if (colinear(seg.p1(), seg.p2(), p))
//     {
//       std::cout << "COLINEAR " << seg.p1() << " " << seg.p2() << " " << p << " " << triangle_area(seg.p1(), seg.p2(), p) <<  std::endl;
//       continue;
//     }

//     if (seg.intersect(seg_inf, inter_p))
//     {
//       if (dist(inter_p, this->pts_[i+1]) > EPSILON)
//       {
// 	std::cout << "[" << seg.p1()[0] << " " << seg.p2()[0] << "],[" << seg.p1()[1] << " " << seg.p2()[1] << "])" << std::endl;
// 	++cnt;
//       }
//     }
//   }

//   if ((cnt % 2) == 0)
//     std::cout << p << "  " << cnt << std::endl;
//   //std::cout << cnt << std::endl;
//   return (cnt % 2) == 1;

bool flt(double a, double b)
{
  return a - b < -EPSILON;
}

bool fle(double a, double b)
{
  return a - b < EPSILON;
}

bool fgt(double a, double b)
{
  return a - b > EPSILON;
}

bool fge(double a, double b)
{
  return a - b > -EPSILON;
}


bool
Polygon::my_inside(const Point<2>& p, bool border_is_inside) const
{
  size_t n = this->pts_.size();
  int crossings = 0;
  bool prev_left = false;
  for (size_t i = 1; i <= n; ++i)
  {
    if (flt(this->pts_[n-i][0], p[0]) || fgt(this->pts_[n-i][0], p[0]))
    {
      prev_left = flt(this->pts_[n-i][0], p[0]);
      break;
    }
  }

  for (size_t i = 0; i < n; i++)
  {
    Point<2> p1 = this->pts_[i];
    Point<2> p2 = this->pts_[(i + 1) % n];
    //plain crossing
    //    if ((fgt(p[0], p1[0]) && flt(p[0], p2[0])) ||
      //(flt(p[0], p1[0]) && fgt(p[0], p2[0])))
    if ((fgt(p[0], p1[0]) && flt(p[0], p2[0])) ||
	(flt(p[0], p1[0]) && fgt(p[0], p2[0])))
    {
      double t = (p[0] - p2[0]) / (p1[0] - p2[0]);
      double cy = t * p1[1] + (1-t) * p2[1];
      if (feq(p[1], cy, EPSILON))
	return border_is_inside;
      else if (fgt(p[1], cy))
      {
	//std::cout << "plot([" << p1[0] << "," << p2[0] << "],[" << p1[1] << "," << p2[1] << "], 'c', 'LineWidth', 2) " << (std::abs(p[0] - p2[0]) < -EPSILON) << std::endl;
	++crossings;
      }
    }
    else if (feq(p1[0], p[0], EPSILON) && (fge(p[1], p1[1]) || fgt(p[1], p2[1]))) // && p1[1] <= p[1] // || p2[1] <= p[1]))
    {
      if (feq(p1[1], p[1], EPSILON)) // p is p1
	return border_is_inside;
      if (feq(p2[0], p[0], EPSILON)) // p is colinear to p1,p2
      {
	if ((fle(p[1], p1[1]) && fge(p[1], p2[1])) ||
	    (fge(p[1], p1[1]) && fle(p[1], p2[1]))) //p is inside p1,p2
	  return border_is_inside;
      }

      if (fle(p1[1], p[1]) && ((prev_left && fgt(p2[0], p[0]))
			       || (!prev_left && flt(p2[0], p[0]))))
      {
	//std::cout << "plot([" << p1[0] << "," << p2[0] << "],[" << p1[1] << "," << p2[1] << "], 'm', 'LineWidth', 2)" << std::endl;
	++crossings;
      }

      // if (prev_left && p1[0] > p[0])
      // {
      // 	std::cout << "2 " << "plot([" << p1[0] << "," << p2[0] << "],[" << p1[1] << "," << p2[1] << "], 'm', 'LineWidth', 2)" << std::endl;
      // 	++crossings;
      // }
      // else if (p0[0] > p[0])
      // {
      // 	std::cout << "3 " << "plot([" << p1[0] << "," << p2[0] << "],[" << p1[1] << "," << p2[1] << "], 'c', 'LineWidth', 2)" << std::endl;
      // 	++crossings;
      // }
    }

    if (!feq(p2[0], p[0], EPSILON))
      prev_left = flt(p2[0], p[0]);
  }

  //std::cout << crossings << std::endl;
  return (crossings % 2) == 1;
}

// bool
// Polygon::my_inside(const Point<2>& p, bool border_is_inside) const
// {
//   size_t n = this->pts_.size();
//   double ax = 0.0;
//   double ay = this->pts_[n-2][0];
//   double bx = this->pts_[n-1][0] - p[0];
//   double by = this->pts_[n-1][1] - p[1];

//   bool lup = by > ay;
//   for (size_t i = 0; i < n; ++i)
//   {
//     ax = bx;
//     ay = by;
//     bx = this->pts_[i][0] - p[0];
//     by = this->pts_[i][1] - p[1];

//     if (feq(ay, by, EPSILON))
//       continue;
//     lup = by > ay;
//   }

//   int depth = 0;
//   for (size_t i = 0; i < n; ++i)
//   {
//     ax = bx;
//     ay = by;
//     bx = this->pts_[i][0] - p[0];
//     by = this->pts_[i][1] - p[1];

//     std::cout << this->pts_[i] << std::endl;

//     std::cout << "ax = " << ax << " ay = " << ay << " bx = " << bx << " by = " << by << std::endl;
//     if ((ay < -EPSILON && by < -EPSILON) ||
//   	(ay > EPSILON  && by > EPSILON) ||
//   	(ax < -EPSILON && bx < -EPSILON))
//     {
//       std::cout << "CONT 1" << std::endl;
//       continue;
//     }

//     if (feq(ay, by, EPSILON) && std::min(ax, bx) <= 0)
//     {
//       std::cout << "?????????" << std::endl;
//       return true;
//     }

//     if (feq(ay, by, EPSILON))
//       continue;

//     double lx = ax + (bx - ax) * (-ay) / (by - ay);
//     std::cout << "lx = " << lx << std::endl;
//     if (feq(lx, 0, EPSILON))
//     {
//       std::cout << "BORDER" << std::endl;
//       return border_is_inside;
//     }

//     if (lx > 0)
//     {
//       std::cout << "PLUS " << std::endl;
//       ++depth;
//     }
//     if (feq(ay, 0, EPSILON) && lup && by > ay)
//       --depth;
//     if (feq(ay, 0, EPSILON) && !lup && by < ay)
//     {
//       std::cout << "MINUS " << ay << " " << by << std::endl;
//       --depth;
//     }

//     lup = (by - ay) > EPSILON;
//   }
//   std::cout << "depth = " << depth << std::endl;

//   return depth % 2 == 1;
// }


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

  for (size_t i = 0; i < this->pts_.size(); ++i)
  {
    Segment<2> s2(this->pts_[i], this->pts_[(i + 1) % this->pts_.size()]);

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
  }

  return already;
}

std::vector<Segment<2> >
Polygon::segments() const
{
  std::vector<Segment<2> > res;
  for (size_t i = 0; i < this->pts_.size() - 1; ++i)
    res.push_back(Segment<2> (this->pts_[i], this->pts_[i+1]));
  res.push_back(Segment<2> (this->pts_[this->pts_.size()-1], this->pts_[0]));
  return res;
}

void Polygon::round_poly_pts()
{
  for (Point<2>& p: this->pts_)
    round_to_precision<2>(p);
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

std::vector<Segment<2> >
CompoundPolygon::segments() const
{
  std::vector<Segment<2> > res = this->base_.segments();
  for (const Polygon& diff: this->diffs_)
  {
    std::vector<Segment<2> > segs = diff.segments();
    res.insert(res.end(), segs.begin(), segs.end());
  }
  return res;
}

void
CompoundPolygon::round_poly_pts()
{
  this->base_.round_poly_pts();
  for (Polygon& diff: this->diffs_)
    diff.round_poly_pts();
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

std::vector<Segment<2> >
MultiplePolygon::segments() const
{
  std::vector<Segment<2> > res;
  for (const CompoundPolygon& poly: this->polys_)
  {
    const std::vector<Segment<2> > tmp = poly.segments();
    res.insert(res.end(), tmp.begin(), tmp.end());
  }
  return res;
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


void
MultiplePolygon::round_poly_pts()
{
  for (CompoundPolygon& poly: this->polys_)
    poly.round_poly_pts();
}

Box<2>
MultiplePolygon::bounding_box() const
{
  Box<2> res = this->polys_[0].bounding_box();
  for (unsigned i = 1; i < this->polys_.size(); ++i)
  {
    Box<2> b = this->polys_[i].bounding_box();
    if (b.min()[0] < res.min()[0])
      res.set_min_v(b.min()[0], 0);
    if (b.min()[1] < res.min()[1])
      res.set_min_v(b.min()[1], 1);
    if (b.max()[0] > res.max()[0])
      res.set_max_v(b.max()[0], 0);
    if (b.max()[1] > res.max()[1])
      res.set_max_v(b.max()[1], 1);
  }

  return res;
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
  return os << box.min() << ", " << box.max() << std::endl;
}

std::ostream&
operator<< (std::ostream& os, const Cylinder& c)
{
  return os << c.base() << ", " << c.r() << std::endl;
}

std::ostream&
operator<< (std::ostream& os, const Polygon& poly)
{
  for (const Point<2>& p: poly.pts())
    os << p << ";" << std::endl;
  os << poly.pts()[0] << ";" << std::endl;
  return os;
}

template class Shape<2>;
template class Shape<3>;

template class Box<2>;
template class Box<3>;

template std::ostream& operator<<<2> (std::ostream& os, const Box<2>& box);
