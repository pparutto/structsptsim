#include "collider.hh"

#include "segment.hh"
#include "utils.hh"

#include <iostream>
#include <cassert>

bool
NoneCollider::outside(const Point& p) const
{
  (void) p;
  return false;
}

Point
NoneCollider::collide(const Point& p1, const Point& p2) const
{
  (void) p1;
  return p2;
}

BoxCollider::BoxCollider(const Box& box)
  : box_(box)
{
}

bool
BoxCollider::outside(const Point& p) const
{
  return !this->box_.inside(p);
}

Point
BoxCollider::collide(const Point& p1, const Point& p2) const
{
  (void) p1;

  Point res = p2;

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


PolygonCollider::PolygonCollider(const CompoundPolygon& poly)
  : poly_(poly)
{
}

bool
PolygonCollider::outside(const Point& p) const
{
  return !bp::inside(this->poly_, bp::construct<pPoint> (p[0], p[1]));
}


//There could be a problem if p1 is exactly on the polygon and p2 is
//outside without crossing any other boundaries because the
//intersect_with function skips points on segments.

//because when you replace the point at the intersection point then
//you get a collision with the segment.

//In this case, we would need to change the function to consider it
//only the first time
Point
PolygonCollider::collide(const Point& p1, const Point& p2) const
{
  Segment s1(p1, p2);

  Point p = p1;
  Point pp = p2;
  do
  {
    Segment s2 = bp::intersect_with(this->poly_, s1);

    pp = Segment::reflect(s1, s2);
    p = Segment::intersection_point(s1, s2);

    Vec norm = s2.normal();
    p = {p[0] + DELTA_REPL * norm[0], p[1] + DELTA_REPL * norm[1]};
    s1 = Segment(p, pp);
  }
  while (this->outside(pp));

  return pp;
}

MultiplePolygonCollider::
MultiplePolygonCollider(const std::vector<CompoundPolygon>& polys)
  : colliders_()
{
  for (const CompoundPolygon& poly: polys)
    this->colliders_.push_back(PolygonCollider(poly));
}

bool
MultiplePolygonCollider::outside(const Point& p) const
{
  for (const PolygonCollider& coll: this->colliders_)
    if (!coll.outside(p))
      return false;
  return true;
}

Point
MultiplePolygonCollider::collide(const Point& p1, const Point& p2) const
{
  for (const PolygonCollider& coll: this->colliders_)
    if (!coll.outside(p1) && coll.outside(p2))
      return coll.collide(p1, p2);
  return {};
}
