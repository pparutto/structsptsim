#ifndef SEGMENT_HH_
#define SEGMENT_HH_

# include "point.hh"

template <size_t N>
class Segment
{
public:
  Segment(const Point<N>& p1, const Point<N>& p2);

  const Point<N>& p1() const;
  const Point<N>& p2() const;

  double distance(const Point<N>& p) const;
  bool on_segment(const Point<N>& p) const;
  Vec<N> vector() const;
  Vec<N> normal() const;
  void invert();

  //std::string plot_str(const std::string& col) const;

  static bool intersect(const Segment<N>& s1, const Segment<N>& s2);
  static Point<N> intersection_point(const Segment<N>& s1,
				     const Segment<N>& s2);
  static Point<N> reflect(const Segment<N>& s1, const Segment<N>& s2);
protected:
  Point<N> p1_;
  Point<N> p2_;
};

template <size_t N>
std::ostream& operator<< (std::ostream& os, const Segment<N>& seg);

#endif /// !SEGMENT_HH
