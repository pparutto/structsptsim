#ifndef IO_HH_
#define IO_HH_

# include <string>

# include "shape.hh"
# include "trajectory.hh"

void save_trajectories_csv(const std::string fname,
			   const TrajectoryEnsemble& trajs);

Polygon poly_from_csv_path(const std::string& fname);
std::vector<CompoundPolygon> polys_from_inkscape_path(const std::string& fname);

void save_poly_matlab(const CompoundPolygon& poly, const std::string& fname);
void save_polys_matlab(const std::vector<CompoundPolygon>& polys,
		       const std::string& fname);

void save_poly_txt(const CompoundPolygon& poly, const std::string& fname);

#endif /// !IO_HH
