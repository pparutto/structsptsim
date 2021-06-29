#include "raw_image_simulator.hh"

#include <iostream>

#include <cmath>
#include <cassert>

void
gaussian_kernel(double*** res, double I0, double s, unsigned size,
		double pxsize, int pos[], double mu[])
{
  assert(size % 2 == 1); //size must be odd
  for (unsigned i = 0; i < size; ++i)
    for (unsigned j = 0; j < size; ++j)
      (*res)[i][j] = 0;

  double sum = 0.0;
  double mid = (size - 1) / 2;

  for (unsigned i = 0; i < size; ++i)
  {
    double x = (pos[0] + i - mid) * pxsize;
    for (unsigned j = 0; j < size; ++j)
    {
      double y =  (pos[1] + j - mid) * pxsize;
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
		    double pxsize, double DT, double I0, double sigma,
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

      if ((unsigned) frame >= length)
	continue;

      int pos[] = {(int) round(p[1] / pxsize), (int) round(p[2] / pxsize)};
      double mu[] = {p[1], p[2]};

      gaussian_kernel(&gker, I0, sigma, 2 * ker_half + 1, pxsize, pos, mu);

      if (pos[0] < 0 || pos[0] > (int) width ||
	  pos[1] < 0 || pos[1] > (int) height)
	continue;

      for (int i = - (int) ker_half; i <= (int) ker_half; ++i)
      {
	int xx = pos[0] + i;
	if (xx < 0 || xx >= (int) width)
	  continue;
	for (int j = - (int) ker_half; j <= (int) ker_half; ++j)
	{
	  int yy = pos[1] + j;
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
