#ifndef UTILS_HH_
# define UTILS_HH_

# include "shape.hh"

const double PRECISION = 1e-6;

Polygon poly_from_inkscape_path(const std::string& fname);

#endif /// !UTILS_HH
