#ifndef IO_HH_
# define IO_HH_

# include <random>
# include <map>
# include <string>

# include <fstream>
# include <sstream>
# include <iomanip>

# include "shape.hh"
# include "trajectory.hh"

struct TrajectoryCharacs;
using TrajectoryCharacsMap = std::map<int, std::vector<TrajectoryCharacs> >;
struct TrajectoryCharacs
{
  Point<2> p0;
  int npts;

  TrajectoryCharacs(Point<2> pos, int npts);

  static TrajectoryCharacsMap from_file(const std::string& fname);
};



Polygon poly_from_csv_path(const std::string& fname);
MultiplePolygon* polys_from_inkscape_path(const std::string& fname);

void save_box_matlab(const Box<2>& poly, const std::string& fname,
		     const std::string& fun_name);

void save_poly_matlab(const CompoundPolygon& poly, const std::string& fname);
void save_polys_matlab(const MultiplePolygon& poly,
		       const std::string& fname);

void save_poly_txt(const CompoundPolygon& poly, const std::string& fname);


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

  f << std::fixed;

  int cpt = 0;
  for (const Trajectory<N>& traj: trajs)
  {
    for (const TimedPoint<N>& p: traj)
    {
      f << cpt;
      for (size_t i = 0; i < N + 1; ++i)
      {
	if (i == 0)
	  f.precision(6);
	else
	  f.precision(5);

	f << "," << p[i];
      }
      f << std::endl;
    }
    ++cpt;
  }
  f.close();
}

template <size_t N>
TrajectoryEnsemble<N> read_trajectories_csv(const std::string& fname)
{
  std::ifstream ifs;
  ifs.open(fname);

  TrajectoryEnsemble<N> res;

  if (!ifs.is_open())
  {
    std::cerr << "ERROR: Could not open input file: " << fname << std::endl;
    return res;
  }

  int last_id = -1;
  std::string line;
  std::string tmp;
  while (std::getline(ifs, line))
  {
    std::istringstream ss(line);

    std::getline(ss, tmp, ',');
    int cur_id = std::stoi(tmp);

    std::getline(ss, tmp, ',');
    double t = std::stod(tmp);

    Point<N> pt;
    for (unsigned i = 0; i < N; ++i)
    {
      std::getline(ss, tmp, ',');
      pt[i] = std::stod(tmp);
    }

    if (last_id != cur_id)
      res.push_back(Trajectory<N>());
    res[res.size() - 1].push_back(to_timed_point(t, pt));

    last_id = cur_id;
  }

  ifs.close();

  return res;
}

#endif /// !IO_HH
