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
		 Segment<2>& inter_s);// const;

  unsigned size() const;
  unsigned cnt() const { return this->cnt_; }
  const Box<2>& area() const { return this->area_; }

  void debug() const;
  void show() const;
protected:
  void insert(const Segment<2>& seg, int max_depth);
  bool my_intersect(const Segment<2>& seg, Point<2>& inter_p,
		    Segment<2>& inter_s, bool already, bool all);// const;
  void my_debug(unsigned& cnt) const;
protected:
  Box<2> area_;
  std::vector<Segment<2> > segs_;
  Box<2> ll_area_;
  QuadTree* ll_;
  Box<2> ul_area_;
  QuadTree* ul_;
  Box<2> lr_area_;
  QuadTree* lr_;
  Box<2> ur_area_;
  QuadTree* ur_;
  unsigned cnt_;
};

#endif /// !QUADTREE_HH
