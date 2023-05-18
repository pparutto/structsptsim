#include "collider.hh"

#include "segment.hh"

#include <iostream>
#include <sstream>
#include <cassert>

template <size_t N>
CollisionException<N>::
CollisionException(const std::vector<Segment<N> >& history,
		   const std::vector<bool>& intersect,
		   const std::vector<bool>& pts_eq,
		   const std::vector<Point<2> >& inter_p,
		   const std::vector<Segment<2> >& inter_s,
		   std::string what)
  : history_(history)
  , intersect_(intersect)
  , pts_eq_(pts_eq)
  , inter_p_(inter_p)
  , inter_s_(inter_s)
  , what_(what)
{
}

template <size_t N>
const char*
CollisionException<N>::what() const noexcept
{
  std::stringstream res;
  res << this->what_ << "; collision history:" << std::endl;
  for (const Segment<2>& seg: this->history_)
    res << seg.p1() << " " << seg.p2() << std::endl;
  for (bool inter: this->intersect_)
    res << inter << std::endl;
  for (bool inter: this->pts_eq_)
    res << inter << std::endl;
  for (Point<2> inter_p: this->inter_p_)
    res << inter_p << std::endl;
  for (Segment<2> inter_s: this->inter_s_)
    res << inter_s << std::endl;

  return res.str().c_str();
}

template <size_t N>
Collider<N>::~Collider()
{
};

template <size_t N>
NoneCollider<N>::~NoneCollider()
{
}

template <size_t N>
bool
NoneCollider<N>::outside(const Point<N>& p) const
{
  (void) p;
  return false;
}

template <size_t N>
bool
NoneCollider<N>::collide(const Point<N>& p1, const Point<N>& p2,
			 Point<N>& res) const
{
  (void) p1;
  res = p2;
  return false;
}

template <size_t N>
void
NoneCollider<N>::who_am_I(std::ostream& os) const
{
  os << "NoneCollider()" << std::endl;
}

template <size_t N>
BoxCollider<N>::BoxCollider(const Box<N>& box)
  : box_(box)
{
}

template <size_t N>
BoxCollider<N>::~BoxCollider()
{
}

template <size_t N>
bool
BoxCollider<N>::outside(const Point<N>& p) const
{
  return !this->box_.inside(p);
}

template <size_t N>
bool
BoxCollider<N>::collide(const Point<N>& p1, const Point<N>& p2,
			Point<N>& res) const
{
  (void) p1;
  res = p2;

  Point<N> min = this->box_.min();
  Point<N> max = this->box_.max();
  bool collided = false;
  while (this->outside(res))
  {
    collided = true;
    for (size_t i = 0; i < N; ++i)
    {
      if (p2[i] > max[i])
	res[i] = max[i] - (p2[i] - max[i]);
      else if (p2[i] < min[i])
	res[i] = min[i] + (min[i] - p2[i]);
    }
  }

  return collided;
}

template <size_t N>
void
BoxCollider<N>::who_am_I(std::ostream& os) const
{
  os << "BoxCollider(" << this->box_ << ")" << std::endl;
}

CylinderCollider::CylinderCollider(const Cylinder& c)
  : c_(c)
{
}

CylinderCollider::~CylinderCollider()
{
}

bool
CylinderCollider::outside(const Point<3>& p) const
{
  return !this->c_.inside(p);
}

bool
CylinderCollider::collide(const Point<3>& p1, const Point<3>& p2,
		     Point<3>& res) const
{
  //std::cout << "p1 = " << p1 << " " << this->c_.inside(p1) << std::endl;
  assert(this->c_.inside(p1));
  Segment<3> s1(p1, p2);
  Point<3> inter_p = null_point<3>();
  bool collided = false;

  // if (!this->c_.inside(p2))
  //   assert(this->c_.intersect(s1, inter_p));
  while (this->c_.intersect(s1, inter_p))
  {
    //std::cout << inter_p << " @@ " << this->c_.base().distance(inter_p) << std::endl;
    collided = true;
    Vec<3> N = this->c_.normal(inter_p);
    // double s = (N[0] * (s1.p2()[0] - inter_p[0]) +
    // 		N[1] * (s1.p2()[1] - inter_p[1]) +
    // 		N[2] * (s1.p2()[2] - inter_p[2]));
    // //std::cout << s << std::endl;
    // res = s1.p2() - N * (2 * s);

    Vec<3> v = this->c_.base().vector();
    double t0 = (dot(s1.p2(), v) - dot(inter_p, v)) / dot(v, v);

    res = (inter_p + v * t0) * 2 - s1.p2();

    std::cout << "N  " << norm(s1.vector()) << "   " << norm(s1.p1() - inter_p) + norm(inter_p - res) << std::endl;
    //std::cout << this->c_.base().distance(s1.p2()) << " " << this->c_.base().distance(inter_p) << " " << this->c_.base().distance(res) << std::endl;
    assert(this->c_.inside(res));
    res = round_to_precision<3>(res);

    Segment<3> norm_s = Segment<3> (inter_p, inter_p + N);
    double t = norm_s.orthogonal_project_t(res);
    if (t < 0)
    {
      std::cout << "YÀYYYYYYYYYYYYYYYYYYYYY " << t << std::endl;
      res = res - N * (-2 * t);
      res = round_to_precision<3>(res);
      assert(this->c_.inside(res));
    }

    std::cout << this->c_.base().distance(s1.p2()) << " " << this->c_.base().distance(res) << std::endl;
    s1 = Segment<3>(inter_p, res);
    assert(this->c_.inside(res));
  }

  res = s1.p2();
  std::cout << "res = " << res << " " << this->c_.inside(res) << std::endl;
  return collided;
}

void CylinderCollider::who_am_I(std::ostream& os) const
{
  os << "CylinderCollider(" << this->c_ << ")" << std::endl;
}



PolygonCollider::PolygonCollider(const Polygon& poly, const QuadTree* qt)
  : poly_(poly)
  , qt_(qt)
{
}

PolygonCollider::~PolygonCollider()
{
}

bool
PolygonCollider::outside(const Point<2>& p) const
{
  return !this->poly_.inside(p);
}


//There could be a problem if p1 is exactly on the polygon and p2 is
//outside without crossing any other boundaries because the
//intersect_with function skips points on segments.

//because when you replace the point at the intersection point then
//you get a collision with the segment.

//In this case, we would need to change the function to consider it
//only the first time
bool
PolygonCollider::collide(const Point<2>& p1, const Point<2>& p2,
			 Point<2>& res) const
{
  // /!\ We need to discard this test at some point
  //assert(this->poly_.inside(p1));
  Point<2> p = p1;
  res = p2;
  Segment<2> s1(p1, res);

  Point<2> tmp = null_point<2>();
  Point<2> inter_p = null_point<2>();
  Segment<2> inter_s = Segment<2>::null();
  Segment<2> prev_s = Segment<2>::null();
  bool collided = false;
  int cnt = 0;
  std::vector<Segment<2> > history;

  std::vector<bool> intersect;
  std::vector<bool> pts_eq;
  std::vector<Point<2> > inter_ps;
  std::vector<Segment<2> > inter_ss;
  history.push_back(s1);
  //segment cannot collide with the previously collided polygon segment
  while (this->poly_.intersect(s1, inter_p, inter_s) && !(prev_s == inter_s))
  {
    intersect.push_back(this->poly_.intersect(s1, inter_p, inter_s));
    pts_eq.push_back(prev_s == inter_s);
    inter_ps.push_back(inter_p);
    inter_ss.push_back(inter_s);
    collided = true;
    prev_s = inter_s;
    p = inter_p;
    tmp = p;
    res = Segment<2>::reflect(s1, inter_s, inter_p);
    //res = round_to_precision<2>(res);

    s1 = Segment<2>(p, res);
    history.push_back(s1);
    ++cnt;
    if (cnt > 20)
      assert(false);
  }

  // /!\ We need to discard this test at some point
  if (!this->poly_.inside(res))
    throw CollisionException<2>(history, intersect, pts_eq, inter_ps, inter_ss,
				"Collision result outside of polygon");

  return collided;
}

void
PolygonCollider::who_am_I(std::ostream& os) const
{
  os << "PolygonCollider()" << std::endl;
}


MultiplePolygonCollider::
MultiplePolygonCollider(const MultiplePolygon& polys, const QuadTree* qt)
  : colliders_()
  , qt_(qt)
{
  for (const CompoundPolygon& poly: polys.polys())
    this->colliders_.push_back(PolygonCollider(poly, qt));
}

MultiplePolygonCollider::~MultiplePolygonCollider()
{
}

bool
MultiplePolygonCollider::outside(const Point<2>& p) const
{
  for (const PolygonCollider& coll: this->colliders_)
    if (!coll.outside(p))
      return false;
  return true;
}

bool
MultiplePolygonCollider::collide(const Point<2>& p1, const Point<2>& p2,
				 Point<2>& res) const
{
  //Segment<2> inter_s = Segment<2>::null();
  //return this->qt_->intersect(Segment<2>(p1, p2), res, inter_s);

  res = p2;
  for (const PolygonCollider& coll: this->colliders_)
    if (coll.poly().inside(p1) && coll.collide(p1, p2, res))
      return true;

  return false;
}

void
MultiplePolygonCollider::who_am_I(std::ostream& os) const
{
  os << "MultiplePolygonCollider()" << std::endl;
}



QuadTreeCollider::QuadTreeCollider(const MultiplePolygon& polys,
				   const QuadTree* qt)
  : polys_(polys)
  , qt_(qt)
{
}

QuadTreeCollider::~QuadTreeCollider()
{
}

bool
QuadTreeCollider::outside(const Point<2>& p) const
{
  return !this->polys_.inside(p);
}

bool
QuadTreeCollider::collide(const Point<2>& p1, const Point<2>& p2,
			  Point<2>& res) const
{
  //assert(this->polys_.inside(p1));

  Point<2> p = p1;
  res = p2;
  Segment<2> s1(p1, res);

  Point<2> tmp = null_point<2>();
  Point<2> inter_p = null_point<2>();
  Segment<2> inter_s = Segment<2>::null();
  Segment<2> prev_s = Segment<2>::null();
  bool collided = false;

  //segment cannot collide with the previously collided polygon segment
  while (this->qt_->intersect(s1, inter_p, inter_s) && !(prev_s == inter_s))
  {
    collided = true;
    prev_s = inter_s;
    p = inter_p;
    tmp = p;
    res = Segment<2>::reflect(s1, inter_s, inter_p);
    res = round_to_precision<2>(res);
    Segment<2> norm_s = Segment<2> (inter_s.p1(),
				    inter_s.p1() + inter_s.normal());
    double t = norm_s.orthogonal_project_t(res);
    if (t < 0)
    {
      res = res + inter_s.normal() * (-2 * t);
      res = round_to_precision<2>(res);
    }

    s1 = Segment<2>(p, res);
  }

  if (!this->polys_.inside(res))
    throw CollisionException<2>(std::vector<Segment<2> >(), std::vector<bool>(),
				std::vector<bool>(),
				std::vector<Point<2> >(),
				std::vector<Segment<2> >(),
				"Collision result outside of polygon");

  return collided;
}

void
QuadTreeCollider::who_am_I(std::ostream& os) const
{
  os << "QuadTreeCollider" << std::endl;
}



template class CollisionException<2>;
template class NoneCollider<2>;
template class BoxCollider<2>;

template class NoneCollider<3>;
