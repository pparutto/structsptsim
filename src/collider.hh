#ifndef COLLIDER_HH_
#define COLLIDER_HH_

# include "shape.hh"

class CollisionException: public std::exception
{
public:
  CollisionException (const Segment& s, std::string what);
  ~CollisionException() = default;
  virtual const char* what() const noexcept override;

protected:
  const Segment s_;
  const std::string what_;
};

class Collider
{
public:
  virtual ~Collider();

  virtual bool outside(const Point& p) const = 0;
  virtual Point collide(const Point& p1, const Point& p2) const = 0;
};

class NoneCollider: public Collider
{
public:
  virtual ~NoneCollider();
  virtual bool outside(const Point& p) const override;
  virtual Point collide(const Point& p1, const Point& p2) const override;
};

class BoxCollider: public Collider
{
public:
  BoxCollider(const Box& box);
  virtual ~BoxCollider();

  virtual bool outside(const Point& p) const override;
  virtual Point collide(const Point& p1, const Point& p2) const override;
protected:
  Box box_;
};

class PolygonCollider: public Collider
{
public:
  PolygonCollider(const Polygon& poly);
  virtual ~PolygonCollider();

  virtual bool outside(const Point& p) const override;
  virtual Point collide(const Point& p1, const Point& p2) const override;
protected:
  const Polygon& poly_;
};

class MultiplePolygonCollider: public Collider
{
public:
  MultiplePolygonCollider(const MultiplePolygon& polys);
  virtual ~MultiplePolygonCollider();

  virtual bool outside(const Point& p) const override;
  virtual Point collide(const Point& p1, const Point& p2) const override;
protected:
  std::vector<PolygonCollider> colliders_;
};


#endif /// !COLLIDER_HH
