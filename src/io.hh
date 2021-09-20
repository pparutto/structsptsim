#ifndef IO_HH_
#define IO_HH_

# include <random>
# include <map>
# include <string>

# include <fstream>
# include <sstream>

# include "shape.hh"
# include "trajectory.hh"

struct TrajectoryCharacs
{
  Point<2> p0;
  int npts;

  TrajectoryCharacs(Point<2> pos, int npts);
};
using TrajectoryCharacsMap = std::map<int, std::vector<TrajectoryCharacs> >;

enum MotionType {DISTRIB, BROWNIAN};
enum TrajLenType {FIXED, EXP, REG};
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

  bool use_start_reg;
  Box<2> start_reg;
  Box<2> stop_reg;

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

void save_params_csv(const std::string& fname,
		     const ProgramOptions opts);


Polygon poly_from_csv_path(const std::string& fname);
MultiplePolygon* polys_from_inkscape_path(const std::string& fname);

void save_box_matlab(const Box<2>& poly, const std::string& fname,
		     const std::string& fun_name);

void save_poly_matlab(const CompoundPolygon& poly, const std::string& fname);
void save_polys_matlab(const MultiplePolygon& poly,
		       const std::string& fname);

void save_poly_txt(const CompoundPolygon& poly, const std::string& fname);


TrajectoryCharacsMap load_characs(const std::string& fname);

Box<2> parse_box(const std::string& box_line);

template <size_t N>
void save_trajectories_csv(const std::string& fname,
			   const TrajectoryEnsemble<N>& trajs)
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  int cpt = 0;
  for (const Trajectory<N>& traj: trajs)
  {
    for (const TimedPoint<N>& p: traj)
    {
      f << cpt;
      for (size_t i = 0; i < N + 1; ++i)
	f << "," << p[i];
      f << std::endl;
    }
    ++cpt;
  }
  f.close();
}


#endif /// !IO_HH
