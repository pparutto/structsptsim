#include "utils.hh"

#include <iostream>
#include <cmath>

double
round_to_precision(double v)
{
  return std::round(v * PRECISION) / PRECISION;
}


void
round_to_precision(Point& v)
{
  v[0] = round_to_precision(v[0]);
  v[1] = round_to_precision(v[1]);
}
