#ifndef SEGMENT_HH_
#define SEGMENT_HH_

# include "point.hh"

template <size_t N>
class Segment
{
public:
  Segment(const Point<N>& p1, const Point<N>& p2);
  Segment() = default;

  double distance(const Point<N>& p) const;
  bool on_segment(const Point<N>& p) const;
  Vec<N> vector() const;
  Vec<N> normal() const;
  void invert();

  //std::string plot_str(const std::string& col) const;

  bool operator== (const Segment<N>& s2) const;

  bool intersect(const Segment<N>& s, Point<N>& inter_p) const;

  const Point<N>& p1() const { return this->p1_; };
  const Point<N>& p2() const { return this->p2_; };

  static Point<N> reflect(const Segment<N>& s1, const Segment<N>& s2,
			  const Point<N>& inter_p);
  static Segment<N> null();


  Point<N> orthogonal_project(const Point<N>& p) const;
  double orthogonal_project_t(const Point<N>& p) const;

protected:
  Point<N> p1_;
  Point<N> p2_;
};

template <size_t N>
std::ostream& operator<< (std::ostream& os, const Segment<N>& seg)
{
  return os << seg.p1() << "; " << seg.p2();
}


#endif /// !SEGMENT_HH
