#ifndef UTILS_HH_
# define UTILS_HH_

# include "point.hh"

const double PRECISION = 1e7;
const double EPSILON = 1e-7;
const double DELTA_REPL = 1e-10;

double round_to_precision(double v);
void round_to_precision(Point& v);

#endif /// !UTILS_HH
