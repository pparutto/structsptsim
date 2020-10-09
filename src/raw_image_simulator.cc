#include "raw_image_simulator.hh"

#include <iostream>

#include <cmath>
#include <cassert>

double**
gaussian_kernel(double I0, double s, unsigned size, double pxsize)
{
  assert(size % 2 == 1); //size must be odd
  double** res = new double*[size];
  for (unsigned i = 0; i < size; ++i)
    res[i] = new double[size];

  double sum = 0.0;
  int mid = (size - 1) / 2;

  for (unsigned i = 0; i < size; ++i)
  {
    for (unsigned j = 0; j < size; ++j)
    {
      double d = ((i - mid) * (i - mid) + (j - mid) * (j - mid)) * pxsize;
      res[i][j] = 1/(2 * M_PI * s * s) * exp(-d / (2 * s * s));
      sum += res[i][j];
    }
  }

  for (unsigned i = 0; i < size; ++i)
    for (unsigned j = 0; j < size; ++j)
      res[i][j] = (res[i][j] / sum) * I0;

  return res;
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

  int ker_half = 7;
  double** gker = gaussian_kernel(I0, sigma, 2 * ker_half + 1, pxsize);

  for (const Trajectory& traj: trajs)
  {
    for (const TimedPoint& p: traj)
    {
      int frame = (int) (p[0] / DT);

      if ((unsigned) frame >= length)
	continue;

      int x_px = (int) (p[1] / pxsize);
      int y_px = (int) (p[2] / pxsize);

      if (x_px < 0 || x_px > (int) width || y_px < 0 || y_px > (int) height)
	continue;

      for (int i = -ker_half; i <= ker_half; ++i)
      {
	int xx = x_px + i;
	if (xx < 0 || xx >= (int) width)
	  continue;
	for (int j = -ker_half; j <= ker_half; ++j)
	{
	  int yy = y_px + j;
	  if (yy < 0 || yy >= (int) height)
	    continue;

	  res[frame][xx][yy] += gker[i + ker_half][j + ker_half];
	}
      }
    }
  }

  for (int i = 0; i < 2 * ker_half + 1; ++i)
    delete[] gker[i];
  delete[] gker;

  return res;
}
