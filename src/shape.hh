#ifndef SHAPE_HH_
#define SHAPE_HH_

# include "point.hh"
# include "segment.hh"

class Shape
{
public:
  virtual bool inside(const Point& p) const = 0;
  virtual PointEnsemble boundary() const = 0;
};

class Box: public Shape
{
public:
  Box(const Point& lower_left, const Point& upper_right);
  Box(const Box& b) = default;

  virtual bool inside(const Point& p) const override;
  virtual PointEnsemble boundary() const override;

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

  virtual bool inside(const Point& p) const override;
  virtual PointEnsemble boundary() const override;

  Segment intersect_with(const Segment& s1) const;
protected:
  PointEnsemble pts_;
};

#endif /// !SHAPE_HH
