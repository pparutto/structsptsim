#include "collider.hh"

#include "segment.hh"

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
};


PolygonCollider::PolygonCollider(const Polygon& poly)
  : poly_(poly)
{
}

bool
PolygonCollider::outside(const Point& p) const
{
  return !this->poly_.inside(p);
}

Point
PolygonCollider::collide(const Point& p1, const Point& p2) const
{
  Segment s1(p1, p2);

  do
  {
    Segment s2 = this->poly_.intersect_with(s1);
    (void) s2;
  }
  while (this->outside(p2));
  return {0, 0};
}
