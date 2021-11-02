#ifndef COLLIDER_HH_
#define COLLIDER_HH_

# include "shape.hh"

template <size_t N>
class CollisionException: public std::exception
{
public:
  CollisionException (const Segment<N>& s, std::string what);
  ~CollisionException() = default;
  virtual const char* what() const noexcept override;

  const Segment<N>& s() const { return this->s_; };
protected:
  const Segment<N> s_;
  const std::string what_;
};

template <size_t N>
class Collider
{
public:
  virtual ~Collider();

  virtual bool outside(const Point<N>& p) const = 0;
  virtual bool collide(const Point<N>& p1, const Point<N>& p2,
		       Point<N>& res) const = 0;
  virtual void who_am_I(std::ostream& os) const = 0;
};

template <size_t N>
class NoneCollider: public Collider<N>
{
public:
  virtual ~NoneCollider();
  virtual bool outside(const Point<N>& p) const override;
  virtual bool collide(const Point<N>& p1, const Point<N>& p2,
		       Point<N>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
};

template <size_t N>
class BoxCollider: public Collider<N>
{
public:
  BoxCollider(const Box<N>& box);
  virtual ~BoxCollider();

  virtual bool outside(const Point<N>& p) const override;
  virtual bool collide(const Point<N>& p1, const Point<N>& p2,
		       Point<N>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  Box<N> box_;
};

class PolygonCollider: public Collider<2>
{
public:
  PolygonCollider(const Polygon& poly);
  virtual ~PolygonCollider();

  virtual bool outside(const Point<2>& p) const override;
  virtual bool collide(const Point<2>& p1, const Point<2>& p2,
		       Point<2>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  const Polygon& poly_;
};

class PolygonCollider3D: public Collider<3>
{
public:
  PolygonCollider3D(const Polygon3D& poly);
  virtual ~PolygonCollider3D();

  virtual bool outside(const Point<3>& p) const override;
  virtual bool collide(const Point<3>& p1, const Point<3>& p2,
		       Point<3>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  const Polygon3D& poly_;
};

class MultiplePolygonCollider: public Collider<2>
{
public:
  MultiplePolygonCollider(const MultiplePolygon& polys);
  virtual ~MultiplePolygonCollider();

  virtual bool outside(const Point<2>& p) const override;
  virtual bool collide(const Point<2>& p1, const Point<2>& p2,
		       Point<2>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  std::vector<PolygonCollider> colliders_;
};


#endif /// !COLLIDER_HH
