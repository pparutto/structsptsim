#ifndef SEGMENT_HH_
#define SEGMENT_HH_

# include "point.hh"

class Segment
{
public:
  Segment(const Point& p1, const Point& p2);

  const Point& p1() const;
  const Point& p2() const;

  double distance(const Point& p) const;
  bool on_segment(const Point& p) const;
  Vec vector() const;
  Vec normal() const;

  Segment invert() const;

  std::string plot_str(const std::string& col) const;

  static bool intersect(const Segment& s1, const Segment& s2);
  static Point intersection_point(const Segment& s1, const Segment& s2);
  static Point reflect(const Segment& s1, const Segment& s2);
protected:
  Point p1_;
  Point p2_;
};

std::ostream& operator<< (std::ostream& os, const Segment& seg);

#endif /// !SEGMENT_HH
