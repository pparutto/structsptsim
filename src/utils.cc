#include "utils.hh"

#include <iostream>
#include <fstream>
#include <sstream>

//format:
//x,y
//dx,dy
//....
Polygon
poly_from_inkscape_path(const std::string& fname)
{
  // Read from the text file
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
