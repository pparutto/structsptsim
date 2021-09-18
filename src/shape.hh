#ifndef SHAPE_HH_
#define SHAPE_HH_

# include "point.hh"
# include "segment.hh"

class Box;

template <size_t N>
class Shape
{
public:
  virtual ~Shape();
  virtual bool inside(const Point<N>& p) const = 0;
  virtual PointEnsemble<N> boundary() const = 0;

  //to update to N dim box
  virtual Box bounding_box() const = 0;
};

class Box: public Shape<2>
{
public:
  Box();
  Box(const Point<2>& lower_left, const Point<2>& upper_right);
  Box(const Box& b) = default;

  virtual ~Box();

  virtual bool inside(const Point<2>& p) const override;
  virtual PointEnsemble<2> boundary() const override;
  virtual Box bounding_box() const override;

  const Point<2>& lower_left() const { return this->lower_left_; }
  const Point<2>& upper_right() const { return this->upper_right_; }
  double width() const { return this->width_; }
  double height() const { return this->height_; }
protected:
  Point<2> lower_left_;
  Point<2> upper_right_;
  double width_;
  double height_;
};

class Polygon: public Shape<2>
{
public:
  Polygon(const PointEnsemble<2>& pts);
  Polygon() = default;

  virtual ~Polygon();

  virtual bool inside(const Point<2>& p) const override;
  bool inside(const Polygon& poly) const;

  virtual PointEnsemble<2> boundary() const override;
  virtual Box bounding_box() const override;

  virtual void apply_pxsize(double pxsize);
  virtual Segment<2> intersect_with(const Segment<2>& s1) const;

  virtual double signed_area() const;

  virtual const PointEnsemble<2>& pts() const { return this->pts_; };

  void round_poly_pts();

  bool my_inside(const Point<2>& p, bool border_is_inside) const;
  bool my_inside2(const Point<2>& p, bool border_is_inside) const;
private:
  PointEnsemble<2> pts_;
};

class CompoundPolygon: public Polygon
{
public:
  CompoundPolygon(const Polygon& base, const std::vector<Polygon>& diffs);

  virtual ~CompoundPolygon();

  virtual bool inside(const Point<2>& p) const override;
  virtual PointEnsemble<2> boundary() const override;
  virtual Box bounding_box() const override;

  virtual void apply_pxsize(double pxsize) override;
  virtual Segment<2> intersect_with(const Segment<2>& s1) const override;

  virtual double signed_area() const override;

  virtual const PointEnsemble<2>& pts() const override;
  const Polygon& base() const { return this->base_; };
  const std::vector<Polygon>& diffs() const { return this->diffs_; };
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
  virtual PointEnsemble<2> boundary() const;
  virtual Box bounding_box() const;

  bool empty() const;
protected:
  std::vector<CompoundPolygon> polys_;
};

std::ostream& operator<< (std::ostream& os, const Box& box);
std::ostream& operator<< (std::ostream& os, const Polygon& poly);

#endif /// !SHAPE_HH
