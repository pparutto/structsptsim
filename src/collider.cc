#include "collider.hh"

#include "segment.hh"

#include <iostream>
#include <sstream>
#include <cassert>

template <size_t N>
CollisionException<N>::
CollisionException(const std::vector<Segment<N> >& history,
		   std::string what)
  : history_(history)
  , what_(what)
{
}

template <size_t N>
const char*
CollisionException<N>::what() const noexcept
{
  std::cerr << this->what_ << "; collision history:" << std::endl;
  for (const Segment<2>& seg: this->history_)
    std::cerr << seg.p1() << " " << seg.p2() << std::endl;

  std::stringstream res;
  // res << this->what_ << "; collision history:" << std::endl;
  // for (const Segment<2>& seg: this->history_)
  //   res << seg.p1() << " " << seg.p2() << std::endl;
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
  assert(this->poly_.inside(p1));
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

  //segment cannot collide with the previously collided polygon segment
  while (this->poly_.intersect(s1, inter_p, inter_s) && !(prev_s == inter_s))
  {
    history.push_back(s1);
    collided = true;
    prev_s = inter_s;
    p = inter_p;
    tmp = p;
    res = Segment<2>::reflect(s1, inter_s, inter_p);
    //res = round_to_precision<2>(res);

    s1 = Segment<2>(p, res);
    ++cnt;
    if (cnt > 20)
      assert(false);
  }
  history.push_back(s1);

  // /!\ We need to discard this test at some point
  if (!this->poly_.inside(res))
    throw CollisionException<2>(history, "Collision result outside of polygon");

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
    if (coll.collide(p1, p2, res))
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

  //int cnt = 0;
  //std::vector<Segment<2> > history;
  //segment cannot collide with the previously collided polygon segment
  while (this->qt_->intersect(s1, inter_p, inter_s) && !(prev_s == inter_s))
  {
    //history.push_back(s1);
    //std::cout << qt->cnt() << std::endl;
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
      //std::cout << "YÀYYYYYYYYYYYYYYYYYYYYY " << t << std::endl;
      res = res + inter_s.normal() * (-2 * t);
      res = round_to_precision<2>(res);
      //assert(this->polys_.inside(res));
    }

    s1 = Segment<2>(p, res);
    //++cnt;
    //if (cnt > 20)
    //  assert(false);
  }
  //history.push_back(s1);

  //if (!this->polys_.inside(res))
  //  throw CollisionException<2>(history, "Collision result outside of polygon");

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
