#ifndef RAW_IMAGE_SIMULATOR_HH_
#define RAW_IMAGE_SIMULATOR_HH_

#include "trajectory.hh"


unsigned short***
raw_image_simulator(unsigned length, unsigned width, unsigned height,
		    double pxsize, double DT, double I0, double sigma,
		    const TrajectoryEnsemble<2>& trajs);


#endif /// !RAW_IMAGE_SIMULATOR_HH_
