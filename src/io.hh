#ifndef IO_HH_
#define IO_HH_

# include <random>
# include <map>
# include <string>

# include "shape.hh"
# include "trajectory.hh"

struct TrajectoryCharacs
{
  Point p0;
  int npts;

  TrajectoryCharacs(Point pos, int npts);
};
using TrajectoryCharacsMap = std::map<int, std::vector<TrajectoryCharacs> >;

enum MotionType {DISTRIB, BROWNIAN};
enum TrajLenType {FIXED, EXP};
enum TrajGenType {NTRAJS, NFRAMES, EMPIRICAL};

struct ProgramOptions
{
  bool use_poly = false;
  std::string poly_path;

  bool export_poly_txt = false;
  bool export_poly_mat = false;

  bool use_pxsize = false;
  double pxsize = NAN;

  TrajLenType tr_len_type;
  unsigned Npts = 0;
  std::exponential_distribution<double> pdist;

  double dt = NAN;
  double DT = NAN;

  TrajGenType tr_gen_type;

  unsigned Ntrajs = 0;
  unsigned Nframes = 0;
  double spot_dens = NAN;
  std::string empirical_trajs_file;
  TrajectoryCharacsMap empirical_trajs;

  unsigned width = 128;
  unsigned height = 128;

  MotionType motion_type;
  double D = NAN;
  std::string cdf_path;

  std::string outdir;

  unsigned t_ratio()
  {
    return (unsigned) (this->DT / this->dt);
  }

  unsigned num_particles()
  {
    return (unsigned) (this->width * this->height * this->spot_dens);
  }
};


void save_trajectories_csv(const std::string& fname,
			   const TrajectoryEnsemble& trajs);

void save_params_csv(const std::string& fname,
		     const ProgramOptions opts);


Polygon poly_from_csv_path(const std::string& fname);
MultiplePolygon* polys_from_inkscape_path(const std::string& fname);

void save_poly_matlab(const CompoundPolygon& poly, const std::string& fname);
void save_polys_matlab(const MultiplePolygon& poly,
		       const std::string& fname);

void save_poly_txt(const CompoundPolygon& poly, const std::string& fname);


TrajectoryCharacsMap load_characs(const std::string& fname);

#endif /// !IO_HH
