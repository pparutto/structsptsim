#include "io.hh"

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

#include "utils.hh"

void
save_trajectories_csv(const std::string& fname, const TrajectoryEnsemble& trajs)
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  int cpt = 0;
  for (const Trajectory& traj: trajs)
  {
    for (const TimedPoint& p: traj)
      f << cpt << "," << p[0] << "," << p[1] << "," << p[2] << std::endl;
    ++cpt;
  }
  f.close();
}

void
save_params_csv(const std::string& fname, const ProgramOptions opts)
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  if (opts.motion_type == MotionType::BROWNIAN)
  {
    f << "MotionType, Brownian" << std::endl;
    f << "D, " << std::to_string(opts.D) << std::endl;
    f << "dt, " << std::to_string(opts.dt) << std::endl;
  }
  else if (opts.motion_type == MotionType::DISTRIB)
  {
    f << "MotionType, Distribution" << std::endl;
    f << "DistribFile, " << opts.cdf_path << std::endl;
  }

  if (opts.tr_gen_type == TrajGenType::NTRAJS)
  {
    f << "TrajGenType, Trajs" << std::endl;
    f << "Ntrajs, " << std::to_string(opts.Ntrajs) << std::endl;
  }
  else if (opts.tr_gen_type == TrajGenType::NFRAMES)
  {
    f << "TrajGenType, Frames" << std::endl;
    f << "Nframes, " << std::to_string(opts.Nframes) << std::endl;
    f << "density, " << std::to_string(opts.spot_dens) << std::endl;
    f << "FOVWidth, " << std::to_string(opts.width) << std::endl;
    f << "FOVHeight, " << std::to_string(opts.height) << std::endl;
  }
  else
  {
    f << "TrajGenType, Empirical" << std::endl;
    f << "EmpiricalTrajsFile, " << opts.empirical_trajs_file << std::endl;
  }

  if (opts.tr_len_type == TrajLenType::EXP)
  {
    f << "TrajLengthType, Exponential" << std::endl;
    f << "NptsLambda, " + std::to_string(opts.pdist.lambda()) << std::endl;
  }
  else
  {
    f << "TrajLengthType, Fixed" << std::endl;
    f << "Npts, " << std::to_string(opts.Npts) << std::endl;
  }

  if (opts.use_pxsize)
    f << "pixelSize, " << opts.pxsize << std::endl;

  f << "DT, " << std::to_string(opts.DT) << std::endl;

  if (opts.use_poly)
  {
    f << "ConfinementType, Polygon" << std::endl;
    f << "polygonFile, " << opts.poly_path << std::endl;
  }
  else
    f << "ConfinementType, FreeSpace" << std::endl;

  f << "Outdir, " << opts.outdir << std::endl;

  f.close();
}

//format:
//x,y
//dx,dy
//....
Polygon
poly_from_csv_path(const std::string& fname)
{
  std::ifstream ifs(fname);

  if (!ifs.is_open())
  {
    std::cout << "ERROR file not found: " << fname << std::endl;
    return Polygon(std::vector<Point> ());
  }

  std::string line;
  std::string tmp;
  std::vector<Point> res;
  while (std::getline(ifs, line))
  {
    std::istringstream ss(line);
    std::getline(ss, tmp, ',');
    double a = std::stod(tmp);
    std::getline(ss, tmp, ',');
    double b = std::stod(tmp);

    if (res.empty())
      res.push_back({a, b});
    else
    {
      int N = res.size() - 1;
      res.push_back({res[N][0] + a, res[N][1] + b});
    }
  }

  return Polygon(res);
}

bool is_letter(const std::string& s)
{
  if (s.size() > 1)
    return false;
  return (s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z');
}

Point read_point(const std::string& s)
{
  std::istringstream ss(s);
  std::string tmp;
  std::getline(ss, tmp, ',');
  double a = std::stod(tmp);
  std::getline(ss, tmp, ',');
  double b = std::stod(tmp);
  return {a, b};
}

//Inkscape polygon format: can contain letters M, h, v, z
//Each line is a polygon
//The first line is the base polygon and succesive lines are the diffs
MultiplePolygon*
polys_from_inkscape_path(const std::string& fname)
{
  std::ifstream ifs(fname);

  if (!ifs.is_open())
  {
    std::cout << "ERROR file not found: " << fname << std::endl;
    return nullptr;
  }


  std::vector<Polygon> polys;
  std::string line;
  while(std::getline(ifs, line))
  {
    Point pos = {0, 0};
    PointEnsemble cur_pe;

    std::string tmp;
    std::istringstream ss(line);
    std::getline(ss, tmp, ' ');

    while (tmp != "z" && tmp != "Z")
    {
      if (tmp == "m" || tmp == "l")
      {
	if (tmp == "m")
	  assert(cur_pe.empty());

	std::getline(ss, tmp, ' ');
	while (!is_letter(tmp))
	{
	  pos = pos + read_point(tmp);
	  cur_pe.push_back(pos);
	  std::getline(ss, tmp, ' ');
	}
      }
      else if (tmp == "M" || tmp == "L")
      {
	if (tmp == "M")
	  assert(cur_pe.empty());

	std::getline(ss, tmp, ' ');
	while (!is_letter(tmp))
	{
	  pos = read_point(tmp);
	  cur_pe.push_back(pos);
	  std::getline(ss, tmp, ' ');
	}
      }
      else if (tmp == "v")
      {
	std::getline(ss, tmp, ' ');
	while (!is_letter(tmp))
	{
	  pos = pos + (Point) {0, std::stod(tmp)};
	  cur_pe.push_back(pos);
	  std::getline(ss, tmp, ' ');
	}
      }
      else if (tmp == "V")
      {
	std::getline(ss, tmp, ' ');
	while (!is_letter(tmp))
	{
	  pos = {pos[0], std::stod(tmp)};
	  cur_pe.push_back(pos);
	  std::getline(ss, tmp, ' ');
	}
      }
      else if (tmp == "h")
      {
	std::getline(ss, tmp, ' ');
	while (!is_letter(tmp))
	{
	  pos = pos + (Point) {std::stod(tmp), 0};
	  cur_pe.push_back(pos);
	  std::getline(ss, tmp, ' ');
	}
      }
      else if (tmp == "H")
      {
	std::getline(ss, tmp, ' ');
	while (!is_letter(tmp))
	{
	  pos = {std::stod(tmp), pos[1]};
	  cur_pe.push_back(pos);
	  std::getline(ss, tmp, ' ');
	}
      }
      else
      {
	std::cout << "Unhandled SVG command: " << tmp << std::endl;
	assert(false);
      }
    }

    assert(tmp == "z" || tmp == "Z");
    assert(!cur_pe.empty());

    polys.push_back(Polygon(cur_pe));
  }

  //IMPORTANT TO PREVENT INTERSECTION ARTIFACTS
  std::cout << "Rounding polygon points to precision: 1/"
	    << PRECISION << std::endl;
  for (Polygon& poly: polys)
    poly.round_poly_pts();

  std::vector<CompoundPolygon> res;
  bool go = true;
  while (go)
  {
    go = false;
    for (unsigned i = 0; i < polys.size(); ++i)
    {
      std::vector<Polygon> overlap;
      std::vector<unsigned> idxs;
      idxs.push_back(i);
      for (unsigned j = 0; j < polys.size(); ++j)
      {
	if (i == j)
	  continue;

	bool ins = polys[j].inside(polys[i]);
	if (abs(polys[i].signed_area()) > abs(polys[j].signed_area()) &&
	    ins)
	{
	  std::cout << "Detected a diff polygon" << std::endl;
	  overlap.push_back(polys[j]);
	  idxs.push_back(j);
	}

	//sanity check for ER
	// if (abs(polys[i].signed_area()) > abs(polys[j].signed_area()) &&
	//     !ins)
	//   assert(false);
      }

      if (!overlap.empty())
      {
	go = true;
	res.push_back(CompoundPolygon(polys[i], overlap));
	for (int k = idxs.size() - 1; k >= 0; --k)
	  polys.erase(polys.begin() + idxs[k]);
	break;
      }
    }
  }

  for (unsigned i = 0; i < polys.size(); ++i)
    res.push_back(CompoundPolygon(polys[i], {}));

  return new MultiplePolygon(res);
}

void save_box_matlab(const Box& box, const std::string& fname,
		     const std::string& fun_name)
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  f << "function box = " << fun_name << "()" << std::endl;
  f << "  box = [" << box.lower_left()[0] << " " << box.lower_left()[1]
    << "; " << box.upper_right()[0] << " " << box.upper_right()[1] << "];"
    << std::endl;
  f << "end" << std::endl;
  f.close();
}

void
save_poly_matlab(const CompoundPolygon& poly, const std::string& fname)
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  f << "function [base_poly, polys] = load_polys()" << std::endl;
  f << "base_poly = [" << std::endl;
  f << poly.base() << "];" << std::endl;

  std::vector<Polygon> polys = poly.diffs();
  f << "polys = {};" << std::endl;
  for (unsigned i = 0; i < polys.size(); ++i)
  {
    f << "polys{" << i + 1 << "} = [" << std::endl;
    f << poly.diffs()[i];
    f << "];" << std::endl;
  }
  f << "end" << std::endl;

  f.close();
}

void
save_polys_matlab(const MultiplePolygon& poly,
		  const std::string& fname)
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  f << "function [base_polys, diff_polys] = polys()" << std::endl;
  int cpt = 1;
  for (const CompoundPolygon& poly: poly.polys())
  {
    f << "base_polys{" << cpt << "} = [" << std::endl;
    f << poly.base() << "];" << std::endl;

    std::vector<Polygon> d_polys = poly.diffs();

    f << "diff_polys{" << cpt << "} = {};" << std::endl;
    for (unsigned i = 0; i < d_polys.size(); ++i)
    {
      f << "diff_polys{" << cpt << "}{" << i + 1 << "} = [" << std::endl;
      f << d_polys[i];
      f << "];" << std::endl;
    }

    ++cpt;
  }

  f << "end" << std::endl;
  f.close();
}

void
save_poly_txt(const CompoundPolygon& poly, const std::string& fname)
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  f << "BASE" << std::endl;
  f << poly.base() << std::endl;

  std::vector<Polygon> polys = poly.diffs();
  for (unsigned i = 0; i < polys.size(); ++i)
  {
    f << "Diff " << i + 1 << std::endl;
    f << poly.diffs()[i];
  }

  f.close();
}


TrajectoryCharacs::TrajectoryCharacs(Point pos, int npts)
  : p0(pos)
  , npts(npts)
{
}


TrajectoryCharacsMap
load_characs(const std::string& fname)
{
  TrajectoryCharacsMap res;

  std::ifstream ifs(fname);
  if (!ifs.is_open())
  {
    std::cout << "ERROR file not found: " << fname << std::endl;
    return res;
  }

  std::string line;
  std::string tmp;
  while (std::getline(ifs, line))
  {
    std::istringstream ss(line);
    std::getline(ss, tmp, ',');
    double px = std::stod(tmp);
    std::getline(ss, tmp, ',');
    double py = std::stod(tmp);
    std::getline(ss, tmp, ',');
    int frame = std::stoi(tmp);
    std::getline(ss, tmp);
    int npts = std::stoi(tmp);

    if (res.find(frame) == res.end())
      res.insert(std::pair<int, std::vector<TrajectoryCharacs> >
		 (frame, std::vector<TrajectoryCharacs> ()));
    res[frame].push_back(TrajectoryCharacs({px, py}, npts));
  }

  return res;
}

Box
parse_box(const std::string& box_line)
{
  std::string tmp;
  std::istringstream ss(box_line);

  std::getline(ss, tmp, ',');
  double ll_x = std::stod(tmp);
  std::getline(ss, tmp, ';');
  double ll_y = std::stod(tmp);
  std::getline(ss, tmp, ',');
  double tr_x = std::stod(tmp);
  std::getline(ss, tmp);
  double tr_y = std::stod(tmp);

  return Box({ll_x, ll_y}, {tr_x, tr_y});
}
