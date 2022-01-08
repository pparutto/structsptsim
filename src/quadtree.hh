#ifndef QUADTREE_HH_
#define QUADTREE_HH_

# include "point.hh"
# include "segment.hh"
# include "shape.hh"

class QuadTree
{
public:
  QuadTree(Box<2> area);
  ~QuadTree();
  void insert_segments(const std::vector<Segment<2> >& segs, int max_depth);

  bool intersect(const Segment<2>& seg, Point<2>& inter_p,
		 Segment<2>& inter_s) const;

  unsigned size() const;
protected:
  void insert(const Segment<2>& seg, int max_depth);
  bool my_intersect(const Segment<2>& seg, Point<2>& inter_p,
		    Segment<2>& inter_s, bool already) const;
    bool my_intersect(const Segment<2>& seg, Point<2>& inter_p,
		    Segment<2>& inter_s, bool already) const;

protected:
  Box<2> area_;
  std::vector<Segment<2> > segs_;
  QuadTree* ll_;
  QuadTree* ul_;
  QuadTree* lr_;
  QuadTree* ur_;
};

#endif /// !QUADTREE_HH
