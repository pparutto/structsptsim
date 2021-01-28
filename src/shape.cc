#include "shape.hh"

#include <cmath>

#include <cassert>
#include <iostream>

#include "utils.hh"

Box::Box(const Point& lower_left, const Point& upper_right)
  : lower_left_(lower_left)
  , upper_right_(upper_right)
  , width_(upper_right[0] - lower_left[0])
  , height_(upper_right[1] - lower_left[1])
{
}

bool
Box::inside(const Point& p) const
{
  return (p[0] >= this->lower_left_[0] && p[0] <= this->upper_right_[0] &&
	  p[1] >= this->lower_left_[1] && p[1] <= this->upper_right_[1]);
}

PointEnsemble
Box::boundary() const
{
  PointEnsemble res;
  res.push_back(this->lower_left_);
  res.push_back({this->lower_left_[0] + this->width_, this->lower_left_[1]});
  res.push_back(this->upper_right_);
  res.push_back({this->lower_left_[0], this->lower_left_[1] + this->height_});
  res.push_back(this->lower_left_);
  return res;
}

Box
Box::bounding_box() const
{
  return Box(this->lower_left_, this->upper_right_);
}

double orientation(const PointEnsemble& pts)
{
  return ((pts[1][0] - pts[0][0]) * (pts[2][1] - pts[0][1]) -
	  (pts[2][0] - pts[0][0]) * (pts[1][1] - pts[0][1])) < 0;
}

bool within(const Segment& s, const Point p)
{
  double mx = std::min(s.p1()[0], s.p2()[0]);
  double Mx = std::max(s.p1()[0], s.p2()[0]);
  double my = std::min(s.p1()[1], s.p2()[1]);
  double My = std::max(s.p1()[1], s.p2()[1]);
  return p[0] >= mx && p[0] <= Mx && p[1] >= my && p[1] <= My;
}


namespace boost::polygon
{
  bool
  inside(const Polygon& poly, const pPoint& p)
  {
    std::size_t i = 0;
    do
    {
      std::size_t next = (i + 1) % poly.size();
      pSegment ps2(poly.coords_[i], poly.coords_[i+1]);

      if (bp::euclidean_distance(ps2, p) < 1/PRECISION)
	return true;

      i = next;
    }
    while (i != 0);

    return bp::contains(poly, p);
  }

  bool
  inside(const CompoundPolygon& poly, const pPoint& p)
  {
    std::size_t i = 0;
    do
    {
      std::size_t next = (i + 1) % poly.size();
      pSegment ps2(poly.self_.coords_[i], poly.self_.coords_[i+1]);

      if (bp::euclidean_distance(ps2, p) < 1/PRECISION)
	return true;

      i = next;
    }
    while (i != 0);

    return bp::contains(poly, p);
  }

  Segment
  intersect_with(const Polygon& poly, const Segment& s1)
  {
    pPoint pp1 = bp::construct<pPoint> (s1.p1()[0], s1.p1()[1]);
    pPoint pp2 = bp::construct<pPoint> (s1.p2()[0], s1.p2()[1]);
    pSegment ps1(pp1, pp2);

    std::size_t next = 1;
    for (std::size_t i = 0; i <= poly.size(); ++i)
    {
      next = (i + 1) % poly.size();
      pSegment ps2(poly.coords_[i], poly.coords_[next]);

      //(bp::euclidean_distance(ps2, pp1) > 1/PRECISION &&
      if (bp::intersects(ps1, ps2))
	return Segment({poly.coords_[i].x(), poly.coords_[i].y()},
		       {poly.coords_[next].x(), poly.coords_[next].y()});
    }

    std::cout << "[" << pp1.x() << " " << pp2.x() << "], [" << pp1.y() << " " << pp2.y() << "], 'x')" << std::endl;
    std::cout << poly << std::endl << "========" << std::endl;
    throw std::runtime_error("Segment did not collide with polygon");
  }

  Segment intersect_with(const CompoundPolygon& poly,
			 const Segment& s1)
  {
    pPoint pp1 = bp::construct<pPoint> (s1.p1()[0], s1.p1()[1]);
    pPoint pp2 = bp::construct<pPoint> (s1.p2()[0], s1.p2()[1]);

    if (bp::inside(poly.self_, pp1) != bp::inside(poly.self_, pp2))
      return intersect_with(poly.self_, s1);

    for (CompoundPolygon::iterator_holes_type it = poly.begin_holes();
	 it != poly.end_holes(); ++it)
    {
      if (bp::contains(*it, pp1) != bp::contains(*it, pp2))
	return intersect_with(*it, s1);
    }

    std::cout << "[" << pp1.x() << " " << pp2.x() << "], [" << pp1.y() << " " << pp2.y() << "], 'x')" << std::endl;
    std::cout << "[" << poly.self_ << "];" << std::endl;
    std::cout << "sub_polys = {";
    for (CompoundPolygon::iterator_holes_type it = poly.begin_holes();
	 it != poly.end_holes(); ++it)
      std::cout << "[" << *it << "];" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << "========" << std::endl;
    throw std::runtime_error("Segment did not collide with compound polygon");
  }

  Box
  bounding_box(const Polygon& poly)
  {
      bp::rectangle_data<double> r;
      bp::extents(r, poly);
      return Box({bp::xl(r), bp::yl(r)}, {bp::xh(r), bp::yh(r)});
  }

  Box
  bounding_box(const CompoundPolygon& poly)
  {
    return bounding_box(poly.self_);
  }

  void
  apply_pxsize(Polygon& poly, double pxsize)
  {
    for (unsigned i = 0; i < poly.coords_.size(); ++i)
    {
      poly.coords_[i].x(poly.coords_[i].x() * pxsize);
      poly.coords_[i].y(poly.coords_[i].y() * pxsize);
    }
  }

  void
  apply_pxsize(CompoundPolygon& poly, double pxsize)
  {
    apply_pxsize(poly.self_, pxsize);
    for (Polygon& hole: poly.holes_)
      apply_pxsize(hole, pxsize);
  }
}

std::ostream&
operator<< (std::ostream& os, const Box& box)
{
  Point ll = box.lower_left();
  Point tr = box.upper_right();
  os << ll[0] << " " << ll[1] << ";"
     << tr[0] << " " << tr[1] << std::endl;
  return os;
}

std::ostream&
operator<< (std::ostream& os, const Polygon& poly)
{
  for (pPoint p: poly)
    os << p << ";";
  return os;
}

std::ostream&
operator<< (std::ostream& os, const pPoint& p)
{
  os << p.x() << " " << p.y();
  return os;
}
