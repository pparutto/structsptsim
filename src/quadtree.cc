#include "quadtree.hh"

QuadTree::QuadTree(Box<2> area)
  : area_(area)
  , segs_()
  , ll_area_()
  , ll_(nullptr)
  , ul_area_()
  , ul_(nullptr)
  , lr_area_()
  , lr_(nullptr)
  , ur_area_()
  , ur_(nullptr)
{
  Point<2> min = this->area_.min();
  Point<2> max = this->area_.max();
  double w = max[0] - min[0];
  double h = max[1] - min[1];
  this->ll_area_ = Box<2> (min, {min[0] + w/2, min[1] + h/2});
  this->ul_area_ = Box<2> ({min[0], min[1] + h/2}, {min[0] + w/2, max[1]});
  this->lr_area_ = Box<2> ({min[0] + w/2, min[1]}, {max[0], min[1] + h/2});
  this->ur_area_ = Box<2> ({min[0] + w/2, min[1] + h/2}, max);
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
  return this->my_intersect(s1, inter_p, inter_s, false, false);
}

bool
QuadTree::my_intersect(const Segment<2>& s1, Point<2>& inter_p,
		       Segment<2>& inter_s, bool already, bool all) const
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

  if (!all)
  {
    bool in_ll = this->ll_area_.inside(s1);
    bool in_lr = this->lr_area_.inside(s1);
    bool in_ul = this->ul_area_.inside(s1);
    bool in_ur = this->ur_area_.inside(s1);

    if (in_ll + in_lr + in_ul + in_ur > 1 ||
	in_ll + in_lr + in_ul + in_ur == 0)
      all = true;
    else if (this->ll_ && in_ll)
      return already || this->ll_->my_intersect(s1, inter_p, inter_s,
						already, false);
    else if (this->lr_ && in_lr)
      return already || this->lr_->my_intersect(s1, inter_p, inter_s,
						already, false);
    else if (this->ul_ && in_ul)
      return already || this->ul_->my_intersect(s1, inter_p, inter_s,
						already, false);
    else if (this->ur_ && in_ur)
      return already || this->ur_->my_intersect(s1, inter_p, inter_s,
						already, false);
  }

  if (all)
  {
    if (this->ll_)
      already = already || this->ll_->my_intersect(s1, inter_p, inter_s,
						   already, true);
    if (this->lr_)
      already = already || this->lr_->my_intersect(s1, inter_p, inter_s,
						   already, true);
    if (this->ul_)
      already = already || this->ul_->my_intersect(s1, inter_p, inter_s,
						   already, true);
    if (this->ur_)
      already = already || this->ur_->my_intersect(s1, inter_p, inter_s,
						   already, true);
  }

  return already;
}

void
QuadTree::insert(const Segment<2>& seg, int max_depth)
{
  if (max_depth == 0)
  {
    this->segs_.push_back(seg);
    return;
  }

  if (this->ll_area_.inside(seg))
  {
    if (this->ll_ == nullptr)
      this->ll_ = new QuadTree(this->ll_area_);
    this->ll_->insert(seg, max_depth - 1);
  }
  else if (this->lr_area_.inside(seg))
  {
    if (this->lr_ == nullptr)
      this->lr_ = new QuadTree(this->lr_area_);
    this->lr_->insert(seg, max_depth - 1);
  }
  else if (this->ul_area_.inside(seg))
  {
    if (this->ul_ == nullptr)
      this->ul_ = new QuadTree(this->ul_area_);
    this->ul_->insert(seg, max_depth - 1);
  }
  else if (this->ur_area_.inside(seg))
  {
    if (this->ur_ == nullptr)
      this->ur_ = new QuadTree(this->ur_area_);
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

void
QuadTree::debug() const
{
  unsigned cnt = 0;
  this->my_debug(cnt);
}

void
QuadTree::my_debug(unsigned& cnt) const
{
  for (const Segment<2>& seg: this->segs_)
    std::cout << seg.p1() << ", " << seg.p2() << ", " << cnt << std::endl;

  if (this->ul_)
  {
    ++cnt;
    this->ul_->my_debug(cnt);
  }
  if (this->ur_)
  {
    ++cnt;
    this->ur_->my_debug(cnt);
  }
  if (this->ll_)
  {
    ++cnt;
    this->ll_->my_debug(cnt);
  }
  if (this->lr_)
  {
    ++cnt;
    this->lr_->my_debug(cnt);
  }
}

void
QuadTree::show() const
{
  std::cout << this->area_;
  if (this->ul_)
    this->ul_->show();
  if (this->ur_)
    this->ur_->show();
  if (this->ll_)
    this->ll_->show();
  if (this->lr_)
    this->lr_->show();
}
