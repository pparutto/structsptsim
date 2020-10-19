#include "raw_image_simulator.hh"

#include <iostream>

#include <cmath>
#include <cassert>

void
gaussian_kernel(double*** res, double I0, double s, unsigned size, double pxsize, double mu[])
{
  assert(size % 2 == 1); //size must be odd
  for (unsigned i = 0; i < size; ++i)
    for (unsigned j = 0; j < size; ++j)
      (*res)[i][j] = 0;

  double sum = 0.0;
  double mid = (size - 1) / 2;

  for (unsigned i = 0; i < size; ++i)
  {
    double x = (i - mid) * pxsize;
    for (unsigned j = 0; j < size; ++j)
    {
      double y = (j - mid) * pxsize;
      double d = ((x - mu[0]) * (x - mu[0]) + (y - mu[1]) * (y - mu[1]));
      (*res)[i][j] = 1/(2 * M_PI * s * s) * exp(-d / (2 * s * s));
      sum += (*res)[i][j];
    }
  }

  for (unsigned i = 0; i < size; ++i)
    for (unsigned j = 0; j < size; ++j)
      (*res)[i][j] = ((*res)[i][j] / sum) * I0;
}

unsigned short***
raw_image_simulator(unsigned length, unsigned width, unsigned height,
		    double pxsize, double DT,
		    double I0, double sigma,
		    const TrajectoryEnsemble& trajs)
{
  unsigned short*** res = new unsigned short**[length];
  for (unsigned k = 0; k < length; ++k)
  {
    res[k] = new unsigned short*[width];
    for (unsigned i = 0; i < width; ++i)
    {
      res[k][i] = new unsigned short[height];
      for (unsigned j = 0; j < height; ++j)
	res[k][i][j] = 0;
    }
  }

  unsigned ker_half = 7;
  double** gker = new double*[2 * ker_half + 1];
  for (unsigned i = 0; i < 2 * ker_half + 1; ++i)
    gker[i] = new double[2 * ker_half + 1];


  for (const Trajectory& traj: trajs)
  {
    for (const TimedPoint& p: traj)
    {
      int frame = (int) round(p[0] / DT);
      //std::cout << p[0] << " " << DT << " " << frame << std::endl;

      if ((unsigned) frame >= length)
	continue;

      int x_px = (int) (p[1] / pxsize);
      int y_px = (int) (p[2] / pxsize);
      double mu0[] = {p[1] - x_px * pxsize, p[2] - y_px * pxsize};

      gaussian_kernel(&gker, I0, sigma, 2 * ker_half + 1, pxsize, mu0);
      // for (unsigned i = 0; i < 15; ++i)
      // {
      // 	for (unsigned j = 0; j < 15; ++j)
      // 	  std::cout << gker[i][j] << " ";
      // 	std::cout << std::endl;
      // }

      if (x_px < 0 || x_px > (int) width || y_px < 0 || y_px > (int) height)
	continue;

      for (int i = - (int) ker_half; i <= (int) ker_half; ++i)
      {
	int xx = x_px + i;
	if (xx < 0 || xx >= (int) width)
	  continue;
	for (int j = - (int) ker_half; j <= (int) ker_half; ++j)
	{
	  int yy = y_px + j;
	  if (yy < 0 || yy >= (int) height)
	    continue;

	  res[frame][xx][yy] += gker[i + ker_half][j + ker_half];
	}
      }
    }
  }

  for (unsigned i = 0; i < 2 * ker_half + 1; ++i)
    delete[] gker[i];
  delete[] gker;

  return res;
}
