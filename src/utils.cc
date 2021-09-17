#include "utils.hh"

#include <iostream>
#include <cmath>

double
round_to_precision(double v)
{
  return std::round(v * PRECISION) / PRECISION;
}

template <int N>
void
round_to_precision(Point<N>& v)
{
  for (int i = 0; i < N; ++i)
    v[i] = round_to_precision(v[i]);
}
