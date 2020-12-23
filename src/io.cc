#include "io.hh"

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

void
save_trajectories_csv(const std::string fname, const TrajectoryEnsemble& trajs)
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
std::vector<CompoundPolygon>
polys_from_inkscape_path(const std::string& fname)
{
  std::ifstream ifs(fname);

  if (!ifs.is_open())
  {
    std::cout << "ERROR file not found: " << fname << std::endl;
    return std::vector<CompoundPolygon> ();
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
      for (unsigned j = 0; j < polys.size() && i != j; ++j)
      {
	if (abs(polys[i].signed_area()) > abs(polys[j].signed_area()) &&
	    polys[i].inside(polys[0]))
	{
	  overlap.push_back(polys[j]);
	  idxs.push_back(j);
	}
      }

      if (!overlap.empty())
      {
	go = true;
	res.push_back(CompoundPolygon(polys[0], overlap));
	for (int i = idxs.size() - 1; i >= 0; --i)
	  polys.erase(polys.begin() + i);
	break;
      }
    }
  }

  for (unsigned i = 0; i < polys.size(); ++i)
    res.push_back(CompoundPolygon(polys[i], {}));

  return res;
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
