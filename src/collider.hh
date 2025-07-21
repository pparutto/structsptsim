#ifndef COLLIDER_HH_
#define COLLIDER_HH_

# include "shape.hh"
# include "quadtree.hh"

template <size_t N>
class CollisionException: public std::exception
{
public:
  CollisionException (const std::vector<Segment<N> >& history,
		      const std::vector<bool>& intersect,
		      const std::vector<bool>& pts_eq,
		      const std::vector<Point<N> >& inter_p,
		      const std::vector<Segment<N> >& inter_s,
		      std::string what);
  CollisionException () = default;
  ~CollisionException() = default;
  virtual const char* what() const noexcept override;

  const std::vector<Segment<N> >& history() const { return this->history_; };
protected:
  std::vector<Segment<N> > history_;
  std::vector<bool> intersect_;
  std::vector<bool> pts_eq_;
  std::vector<Point<N> > inter_p_;
  std::vector<Segment<N> > inter_s_;
  std::string what_;
};

template <size_t N>
class Collider
{
public:
  virtual ~Collider();

  virtual bool outside(const Point<N>& p) const = 0;
  //return number of collisions
  virtual unsigned collide(const Point<N>& p1, const Point<N>& p2,
			   Point<N>& res) const = 0;
  virtual void who_am_I(std::ostream& os) const = 0;
};

template <size_t N>
class NoneCollider: public Collider<N>
{
public:
  NoneCollider() = default;
  virtual ~NoneCollider();

  virtual bool outside(const Point<N>& p) const override;
  virtual unsigned collide(const Point<N>& p1, const Point<N>& p2,
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
  virtual unsigned collide(const Point<N>& p1, const Point<N>& p2,
			   Point<N>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  Box<N> box_;
};

class CylinderCollider: public Collider<3>
{
public:
  CylinderCollider(const Cylinder& c);
  virtual ~CylinderCollider();

  virtual bool outside(const Point<3>& p) const override;
  virtual unsigned collide(const Point<3>& p1, const Point<3>& p2,
			   Point<3>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  Cylinder c_;
};


class PolygonCollider: public Collider<2>
{
public:
  PolygonCollider(const Polygon& poly, const QuadTree* qt);
  virtual ~PolygonCollider();

  virtual bool outside(const Point<2>& p) const override;
  virtual unsigned collide(const Point<2>& p1, const Point<2>& p2,
			   Point<2>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;

  const Polygon& poly() const { return this->poly_; };
protected:
  const Polygon& poly_;
  const QuadTree* qt_;
};

class PolygonCollider3D: public Collider<3>
{
public:
  PolygonCollider3D(const Polygon3D& poly);
  virtual ~PolygonCollider3D();

  virtual bool outside(const Point<3>& p) const override;
  virtual unsigned collide(const Point<3>& p1, const Point<3>& p2,
			   Point<3>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  const Polygon3D& poly_;
};

class MultiplePolygonCollider: public Collider<2>
{
public:
  MultiplePolygonCollider(const MultiplePolygon& polys, const QuadTree* qt);
  virtual ~MultiplePolygonCollider();

  virtual bool outside(const Point<2>& p) const override;
  virtual unsigned collide(const Point<2>& p1, const Point<2>& p2,
			   Point<2>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  std::vector<PolygonCollider> colliders_;
  const QuadTree* qt_;
};

class QuadTreeCollider: public Collider<2>
{
public:
  QuadTreeCollider(const MultiplePolygon& polys, const QuadTree* qt);
  virtual ~QuadTreeCollider();

  virtual bool outside(const Point<2>& p) const override;
  virtual unsigned collide(const Point<2>& p1, const Point<2>& p2,
			   Point<2>& res) const override;
  virtual void who_am_I(std::ostream& os) const override;
protected:
  const MultiplePolygon& polys_;
  const QuadTree* qt_;
};


#endif /// !COLLIDER_HH
