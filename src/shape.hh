#ifndef SHAPE_HH_
#define SHAPE_HH_

# include "point.hh"
# include "segment.hh"

class Box;

class Shape
{
public:
  virtual bool inside(const Point& p) const = 0;
  virtual PointEnsemble boundary() const = 0;

  virtual Box bounding_box() const = 0;
};

class Box: public Shape
{
public:
  Box(const Point& lower_left, const Point& upper_right);
  Box(const Box& b) = default;

  virtual bool inside(const Point& p) const override;
  virtual PointEnsemble boundary() const override;
  virtual Box bounding_box() const override;

  const Point& lower_left() const { return this->lower_left_; }
  const Point& upper_right() const { return this->upper_right_; }
  double width() const { return this->width_; }
  double height() const { return this->height_; }
protected:
  Point lower_left_;
  Point upper_right_;
  double width_;
  double height_;
};

class Polygon: public Shape
{
public:
  Polygon(const PointEnsemble& pts);
  Polygon() = default;

  virtual bool inside(const Point& p) const override;
  bool inside(const Polygon& poly) const;
  bool inside2(const Point& p, bool border_is_inside) const;
  virtual PointEnsemble boundary() const override;
  virtual Box bounding_box() const override;

  virtual void apply_pxsize(double pxsize);
  virtual Segment intersect_with(const Segment& s1) const;

  virtual double signed_area() const;

  virtual const PointEnsemble& pts() const { return this->pts_; };

  bool my_inside(const Point& p, bool border_is_inside) const;
private:
  PointEnsemble pts_;
};

class CompoundPolygon: public Polygon
{
public:
  CompoundPolygon(const Polygon& base, const std::vector<Polygon>& diffs);

  virtual bool inside(const Point& p) const override;
  virtual PointEnsemble boundary() const override;
  virtual Box bounding_box() const override;

  virtual void apply_pxsize(double pxsize) override;
  virtual Segment intersect_with(const Segment& s1) const override;

  double signed_area() const override;

  virtual const PointEnsemble& pts() const override;
  const Polygon& base() const { return this->base_; };
  const std::vector<Polygon>& diffs() const { return this->diffs_; };
protected:
  Polygon base_;
  std::vector<Polygon> diffs_;
};

std::ostream& operator<< (std::ostream& os, const Box& box);
std::ostream& operator<< (std::ostream& os, const Polygon& poly);

#endif /// !SHAPE_HH
