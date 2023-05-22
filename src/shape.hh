#ifndef SHAPE_HH_
#define SHAPE_HH_

# include "point.hh"
# include "segment.hh"

template <size_t N>
class Box;

template <size_t N>
class Shape
{
public:
  virtual ~Shape();
  virtual bool inside(const Point<N>& p) const = 0;
  virtual void shift_coords(const Point<N>& p) = 0;

  //to update to N dim box
  virtual Box<N> bounding_box() const = 0;
};

template <size_t N>
class Box: public Shape<N>
{
public:
  Box();
  Box(const Point<N>& min, const Point<N>& max);
  Box(const Box& b) = default;

  virtual ~Box();

  virtual bool inside(const Point<N>& p) const override;
  virtual bool inside(const Segment<N>& seg) const;
  virtual Box bounding_box() const override;
  virtual void shift_coords(const Point<N>& shifts);

  const Point<N>& min() const { return this->min_; }
  const Point<N>& max() const { return this->max_; }
  const Vec<N>& dims() const { return this->dims_; }

  void set_min_v(double v, unsigned idx);
  void set_max_v(double v, unsigned idx);
protected:
  Point<N> min_;
  Point<N> max_;
  Vec<N> dims_;
};

class Triangle3D: public Shape<3>
{
public:
  Triangle3D(const Point<3>& p1, const Point<3>& p2, const Point<3>& p3);
  Triangle3D() = default;

  //it is not a concave shape
  virtual bool inside(const Point<3>& p) const { (void) p; return false; };
  virtual Box<3> bounding_box() const;

  bool intersect(const Segment<3>& s, Point<3>& inter_p) const;

  virtual void shift_coords(const Point<3>& shifts);
  const Vec<3>& normal() const { return this->normal_; }
protected:
  Point<3> p1_;
  Point<3> p2_;
  Point<3> p3_;
  Vec<3> normal_;
};

class Cylinder: public Shape<3>
{
public:
  Cylinder() = default;
  Cylinder(const Segment<3>& base, double r);

  virtual bool inside(const Point<3>& p) const;
  virtual Box<3> bounding_box() const;

  bool intersect(const Segment<3>& s, Point<3>& inter_p) const;
  bool intersect_side(const Segment<3>& s, Point<3>& inter_p) const;

  Vec<3> normal(const Point<3>& p) const;

  virtual void shift_coords(const Point<3>& shifts);

  const Segment<3>& base() const { return this->base_; }
  double r() const { return this->r_; }
protected:
  Segment<3> base_;
  double r_;
};

class Plane: public Shape<3>
{
public:
  Plane() = default;
  Plane(const Point<3>& p1, const Point<3>& p2, const Point<3>& p3);

  virtual bool inside(const Point<3>& p) const;
  virtual Box<3> bounding_box() const;

  virtual void shift_coords(const Point<3>& shifts);

  bool intersect(const Segment<3>& s, Point<3>& inter_p) const;
protected:
  Point<3> p1_;
  Point<3> p2_;
  Point<3> p3_;
};



class Polygon: public Shape<2>
{
public:
  Polygon(const PointEnsemble<2>& pts);
  Polygon() = default;

  virtual ~Polygon();

  virtual bool inside(const Point<2>& p) const override;
  bool inside(const Polygon& poly) const;

  virtual Box<2> bounding_box() const override;

  virtual void apply_pxsize(double pxsize);
  virtual bool intersect(const Segment<2>& s1, Point<2>& inter_p,
			 Segment<2>& inter_seg) const;

  virtual double signed_area() const;

  virtual const PointEnsemble<2>& pts() const { return this->pts_; };

  virtual std::vector<Segment<2> > segments() const;

  virtual void round_poly_pts();
  virtual void shift_coords(const Point<2>& shifts);

  bool my_inside(const Point<2>& p, bool border_is_inside) const;

  static Polygon reverse(const Polygon& poly);
  static bool check_normals(const Polygon& poly, bool inner_normals);
private:
  PointEnsemble<2> pts_;
};

class Polygon3D: public Shape<3>
{
public:
  Polygon3D(const std::vector<Triangle3D>& triangles);
  Polygon3D() = default;

  virtual ~Polygon3D();

  virtual bool inside(const Point<3>& p) const override;

  virtual Box<3> bounding_box() const override;

  virtual bool intersect(const Segment<3>& seg,
			 Point<3>& inter_p, Triangle3D& inter_t) const;

  virtual const std::vector<Triangle3D>& triangles() const
  {
    return this->triangles_;
  };

private:
  std::vector<Triangle3D> triangles_;
};


class CompoundPolygon: public Polygon
{
public:
  CompoundPolygon(const Polygon& base, const std::vector<Polygon>& diffs);

  virtual ~CompoundPolygon();

  virtual bool inside(const Point<2>& p) const override;
  virtual Box<2> bounding_box() const override;

  virtual void apply_pxsize(double pxsize) override;
  virtual bool intersect(const Segment<2>& s,
			 Point<2>& inter_p, Segment<2>& inter_s) const override;

  virtual double signed_area() const override;

  virtual std::vector<Segment<2> > segments() const;

  virtual void round_poly_pts() override;
  virtual void shift_coords(const Point<2>& shifts);

  virtual const PointEnsemble<2>& pts() const override;
  const Polygon& base() const { return this->base_; };
  Polygon& base() { return this->base_; };
  void base(Polygon& p);
  const std::vector<Polygon>& diffs() const { return this->diffs_; };
  std::vector<Polygon>& diffs() { return this->diffs_; };
protected:
  Polygon base_;
  std::vector<Polygon> diffs_;
};

class MultiplePolygon: public Shape<2>
{
public:
  MultiplePolygon(const std::vector<CompoundPolygon>& polys);

  virtual ~MultiplePolygon();

  void apply_pxsize(double pxsize);

  const std::vector<CompoundPolygon>& polys() const;

  virtual bool inside(const Point<2>& p) const;
  virtual Box<2> bounding_box() const;

  std::vector<Segment<2> > segments() const;

  virtual void round_poly_pts();
  virtual void shift_coords(const Point<2>& shifts);

  bool empty() const;
protected:
  std::vector<CompoundPolygon> polys_;
};

template <size_t N>
std::ostream& operator<< (std::ostream& os, const Box<N>& box);


std::ostream& operator<< (std::ostream& os, const Cylinder& c);
std::ostream& operator<< (std::ostream& os, const Polygon& poly);

#endif /// !SHAPE_HH
