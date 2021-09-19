#include "collider.hh"

#include "segment.hh"
#include "utils.hh"

#include <iostream>
#include <sstream>
#include <cassert>

template <size_t N>
CollisionException<N>::CollisionException (const Segment<N>& s,
					   std::string what)
  : s_(s)
  , what_(what)
{
}

template <size_t N>
const char*
CollisionException<N>::what() const noexcept
{
  std::stringstream res;
  res << "Segment " << this->s_.p1() << " -> " << this->s_.p2() << " "
      << this->what_;
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
Point<N>
NoneCollider<N>::collide(const Point<N>& p1, const Point<N>& p2) const
{
  (void) p1;
  return p2;
}

BoxCollider::BoxCollider(const Box& box)
  : box_(box)
{
}

BoxCollider::~BoxCollider()
{
}

bool
BoxCollider::outside(const Point<2>& p) const
{
  return !this->box_.inside(p);
}

Point<2>
BoxCollider::collide(const Point<2>& p1, const Point<2>& p2) const
{
  (void) p1;

  Point<2> res = p2;

  while (this->outside(res))
  {
    if (p2[0] > this->box_.upper_right()[0])
      res[0] = this->box_.upper_right()[0] -
	(p2[0] - this->box_.upper_right()[0]);
    else if (p2[0] < this->box_.lower_left()[0])
      res[0] = this->box_.lower_left()[0] +
	(this->box_.lower_left()[0] - p2[0]);

    if (p2[1] > this->box_.upper_right()[1])
      res[1] = this->box_.upper_right()[1] -
	(p2[1] - this->box_.upper_right()[1]);
    else if (p2[1] < this->box_.lower_left()[1])
      res[1] = this->box_.lower_left()[1] +
	(this->box_.lower_left()[1] - p2[1]);
  }

  return res;
}


PolygonCollider::PolygonCollider(const Polygon& poly)
  : poly_(poly)
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
Point<2>
PolygonCollider::collide(const Point<2>& p1, const Point<2>& p2) const
{
  Segment<2> s1(p1, p2);

  int cnt = 0;

  Point<2> p = p1;
  Point<2> pp = p2;
  do
  {
    //std::cout << p << " " << pp << ";" <<  std::endl;
    Segment<2> s2 = this->poly_.intersect_with(s1);

    pp = Segment<2>::reflect(s1, s2);
    p = Segment<2>::intersection_point(s1, s2);

    //Vec norm = s2.normal();
    //p = {p[0] + DELTA_REPL * norm[0], p[1] + DELTA_REPL * norm[1]};
    s1 = Segment<2>(p, pp);

    ++cnt;
    if (cnt > 20)
      assert(false);
  }
  while (this->outside(pp));
  //std::cout << "===============" << std::endl;

  return pp;
}

MultiplePolygonCollider::
MultiplePolygonCollider(const MultiplePolygon& polys)
  : colliders_()
{
  for (const CompoundPolygon& poly: polys.polys())
    this->colliders_.push_back(PolygonCollider(poly));
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

Point<2>
MultiplePolygonCollider::collide(const Point<2>& p1, const Point<2>& p2) const
{
  for (const PolygonCollider& coll: this->colliders_)
    if (!coll.outside(p1) && coll.outside(p2))
      return coll.collide(p1, p2);

  throw CollisionException(Segment<2>(p1, p2), "did not collide");
}


template class CollisionException<2>;
template class NoneCollider<2>;
