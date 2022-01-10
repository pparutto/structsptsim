#include "quadtree.hh"

QuadTree::QuadTree(Box<2> area)
  : area_(area)
  , segs_()
  , ll_(nullptr)
  , ul_(nullptr)
  , lr_(nullptr)
  , ur_(nullptr)
{
}

QuadTree::~QuadTree()
{
  delete this->ll_;
  delete this->lr_;
  delete this->ul_;
  delete this->ur_;
}

void
QuadTree::insert_segments(const std::vector<Segment<2> >& segs,
			  int max_depth)
{
  for (const Segment<2>& seg: segs)
    this->insert(seg, max_depth);
}

bool
QuadTree::intersect(const Segment<2>& s1, Point<2>& inter_p,
		    Segment<2>& inter_s) const
{
  return this->my_intersect(s1, inter_p, inter_s, false);
}

bool
QuadTree::my_intersect(const Segment<2>& s1, Point<2>& inter_p,
		       Segment<2>& inter_s, bool already) const
{
  Point<2> tmp_inter_p;

  for (const Segment<2>& s2: this->segs_)
  {
    if (!s2.on_segment(s1.p1()) && s1.intersect(s2, tmp_inter_p))
    {
      if (already)
      {
	if (dist(s1.p1(), tmp_inter_p) < dist(s1.p1(), inter_p))
	{
	  inter_s = s2;
	  inter_p = tmp_inter_p;
	}
      }
      else
      {
	already = true;
	inter_p = tmp_inter_p;
	inter_s = s2;
      }
    }
  }

  bool in_ll = this->ll_ && this->ll_->area_.inside(s1);
  bool in_lr = this->lr_ && this->lr_->area_.inside(s1);
  bool in_ul = this->ul_ && this->ul_->area_.inside(s1);
  bool in_ur = this->ur_ && this->ur_->area_.inside(s1);

  if (in_ll)
    return already || this->ll_->my_intersect(s1, inter_p, inter_s, already);
  else if (in_lr)
    return already || this->lr_->my_intersect(s1, inter_p, inter_s, already);
  else if (in_ul)
    return already || this->ul_->my_intersect(s1, inter_p, inter_s, already);
  else if (in_ur)
    return already || this->ur_->my_intersect(s1, inter_p, inter_s, already);

  if (!in_ll && !in_lr && !in_ul && !in_ur)
  {
    if (this->ll_)
      already = already || this->ll_->my_intersect(s1, inter_p, inter_s, already);
    if (this->lr_)
      already = already || this->lr_->my_intersect(s1, inter_p, inter_s, already);
    if (this->ul_)
      already = already || this->ul_->my_intersect(s1, inter_p, inter_s, already);
    if (this->ur_)
      already = already || this->ur_->my_intersect(s1, inter_p, inter_s, already);
  }

  return already;
}

void
QuadTree::insert(const Segment<2>& seg, int max_depth)
{
  Point<2> min = this->area_.min();
  Point<2> max = this->area_.max();
  double w = max[0] - min[0];
  double h = max[1] - min[1];

  Box<2> ll_area(min, {min[0] + w/2, min[1] + h/2});
  Box<2> lr_area({min[0] + w/2, min[1]}, {max[0], min[1] + h/2});
  Box<2> ul_area({min[0], min[1] + h/2}, {min[0] + w/2, max[1]});
  Box<2> ur_area({min[0] + w/2, min[1] + h/2}, max);

  if (max_depth == 0)
  {
    this->segs_.push_back(seg);
    return;
  }

  if (ll_area.inside(seg))
  {
    if (this->ll_ == nullptr)
      this->ll_ = new QuadTree(ll_area);
    this->ll_->insert(seg, max_depth - 1);
  }
  else if (lr_area.inside(seg))
  {
    if (this->lr_ == nullptr)
      this->lr_ = new QuadTree(lr_area);
    this->lr_->insert(seg, max_depth - 1);
  }
  else if (ul_area.inside(seg))
  {
    if (this->ul_ == nullptr)
      this->ul_ = new QuadTree(ul_area);
    this->ul_->insert(seg, max_depth - 1);
  }
  else if (ur_area.inside(seg))
  {
    if (this->ur_ == nullptr)
      this->ur_ = new QuadTree(ur_area);
    this->ur_->insert(seg, max_depth - 1);
  }
  else
    this->segs_.push_back(seg);
}

unsigned
QuadTree::size() const
{
  unsigned res = this->segs_.size();
  if (this->ul_ != nullptr)
    res = res + this->ul_->size();
  if (this->ur_ != nullptr)
    res = res + this->ur_->size();
  if (this->ll_ != nullptr)
    res = res + this->ll_->size();
  if (this->lr_ != nullptr)
    res = res + this->lr_->size();
  return res;
}
