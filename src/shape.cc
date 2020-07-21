#include "shape.hh"

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
