#ifndef SHAPE_HH_
#define SHAPE_HH_

# include <boost/polygon/polygon.hpp>

# include "point.hh"
# include "segment.hh"

namespace bp = boost::polygon;

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


typedef bp::polygon_data<double> Polygon;
typedef bp::polygon_traits<Polygon>::point_type pPoint;
typedef bp::segment_data<double> pSegment;
typedef bp::polygon_with_holes_data<double> CompoundPolygon;

namespace boost::polygon
{
  bool inside(const Polygon& poly, const pPoint& p);
  bool inside(const CompoundPolygon& poly, const pPoint& p);

  Segment intersect_with(const Polygon& poly, const Segment& s1);
  Segment intersect_with(const CompoundPolygon& poly, const Segment& s1);

  Box bounding_box(const Polygon& poly);
  Box bounding_box(const CompoundPolygon& poly);

  void apply_pxsize(Polygon& poly, double pxsize);
  void apply_pxsize(CompoundPolygon& poly, double pxsize);
}

std::ostream& operator<< (std::ostream& os, const Box& box);
std::ostream& operator<< (std::ostream& os, const Polygon& poly);
std::ostream& operator<< (std::ostream& os, const pPoint& poly);

#endif /// !SHAPE_HH
