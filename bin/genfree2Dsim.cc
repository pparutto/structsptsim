#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <sys/stat.h>

#include <tiffio.h>

#include "tclap/CmdLine.h"

#include "simulation.hh"
#include "io.hh"

#include "utils.hh"

#include "raw_image_simulator.hh"


void create_dir_if_not_exist(const std::string& path)
{
    struct stat info;

    if(stat(path.c_str(), &info) != 0 || ! (info.st_mode & S_IFDIR))
    {
      mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      std::cout << "Created directory: " << path << std::endl;
    }
}


int main(int argc, char** argv)
{
  unsigned width = 0;
  unsigned height = 0;
  unsigned length = 0;
  double pxsize = NAN;
  double poisson_lambda = NAN;
  std::poisson_distribution<int> pdist;
  double dt = NAN;
  double DT = NAN;
  double D = NAN;
  double density = NAN;

  std::string outdir;
  try
  {
    TCLAP::CmdLine cmd("./gensim", ' ', "1");

    TCLAP::UnlabeledValueArg<unsigned> width_arg
      ("width", "Width (px)", true, 0, "w");
    cmd.add(width_arg);

    TCLAP::UnlabeledValueArg<unsigned> height_arg
      ("height", "Height (px)", true, 0, "h");
    cmd.add(height_arg);

    TCLAP::UnlabeledValueArg<unsigned> length_arg
      ("length", "Length (frames)", true, 0, "N");
    cmd.add(length_arg);

    TCLAP::UnlabeledValueArg<double> pxsize_arg
      ("pxsize", "Pixel size (µm)", true, NAN, "pxsize");
    cmd.add(pxsize_arg);

    TCLAP::UnlabeledValueArg<double> poisson_arg
      ("λ", "Poissonian λ traj. length", true, NAN, "poissonLambda");
    cmd.add(poisson_arg);

    TCLAP::UnlabeledValueArg<double> dt_arg
      ("dt", "Simulation timestep (s)", true, NAN, "dt");
    cmd.add(dt_arg);

    TCLAP::UnlabeledValueArg<double> DT_arg
      ("DT", "acquisition timestep (s)", true, NAN, "DT");
    cmd.add(DT_arg);

    TCLAP::UnlabeledValueArg<double> D_arg
      ("D", "Diffusion coefficient (µm2/s)", true, NAN, "D");
    cmd.add(D_arg);

    TCLAP::UnlabeledValueArg<double> density_arg
      ("density", "Point density (1/µm²)", true, 0, "dens");
    cmd.add(density_arg);

    TCLAP::UnlabeledValueArg<std::string> outdir_arg
      ("outdir", "output directory", true, "", "outdir");
    cmd.add(outdir_arg);

    cmd.parse(argc, argv);

    width = width_arg.getValue();
    height = height_arg.getValue();
    length = length_arg.getValue();
    pxsize = pxsize_arg.getValue();
    poisson_lambda = poisson_arg.getValue();
    pdist = std::poisson_distribution<int>(poisson_lambda);
    dt = dt_arg.getValue();
    DT = DT_arg.getValue();
    D = D_arg.getValue();
    density = density_arg.getValue();
    outdir = outdir_arg.getValue();
  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << "error: " << e.error() << " for arg "
	      << e.argId() << std::endl;
  }


  create_dir_if_not_exist(outdir);

  //std::cout << std::setprecision(15);

  std::random_device rd;
  std::mt19937_64 mt(rd());

  double region_scale = 0.04;
  Box simulation_region({region_scale * width * pxsize,
			 region_scale * width * pxsize},
    {(1 - region_scale) * width * pxsize,
     (1 - region_scale) * height * pxsize});

  RandomBoxTrajectoryStartGenerator start_gen(mt, simulation_region);

  std::vector<TrajectoryEndCondition*> end_conds;
  end_conds.push_back(new NumberPointsPoissonianEndCondition(pdist, mt));
  //end_conds.push_back(new NumberPointsEndCondition(10));
  end_conds.push_back(new EscapeEndCondition(simulation_region));
  CompoundEndCondition traj_end_cond(end_conds);
  TrajectoryEndConditionFactory traj_end_cond_facto(traj_end_cond);

  BrownianMotion bm(mt, D, dt);

  unsigned t_ratio = (unsigned) (DT / dt);
  SubsambleTrajectoryRecorder traj_rec(0.0, DT, t_ratio);
  TrajectoryRecorderFactory traj_rec_facto(traj_rec);

  NumberFramesSimulationEndCondition end_sim((int) length);

  NoneCollider collider;

  TrajectoryGeneratorFactory traj_gen_facto(start_gen, bm, traj_end_cond_facto,
					    traj_rec_facto, collider);

  unsigned n_particles = (unsigned) width * height * density;
  std::cout << "Particles per frame: " << n_particles << std::endl;
  SimulationDensity sim(traj_gen_facto, end_sim, n_particles, DT, t_ratio);

  sim.run();

  save_trajectories_csv(outdir + "/trajs.csv", sim.trajs());

  std::ofstream f;
  f.open(outdir + "/parameters.csv");
  f << "width (px): " << width << std::endl;
  f << "height (px): " << height << std::endl;
  f << "length (frame): " << length << std::endl;
  f << "pxsize (µm): " << pxsize << std::endl;
  f << "Poisson lambda (frame): " << poisson_lambda << std::endl;
  f << "dt (s): " << dt << std::endl;
  f << "DT (s): " << DT << std::endl;
  f << "D (µm2/s): " << D << std::endl;
  f << "density (1/mu m2): " << density << std::endl;
  f << "region scale factor: " << region_scale << std::endl;
  f.close();

  unsigned short*** imgs = raw_image_simulator(length, width, height, pxsize,
					       DT, 1000.0, 0.2, sim.trajs());
  TIFF* tif = TIFFOpen((outdir + "/raw_data.tif").c_str(), "w");
  for (unsigned k = 0; k < length; ++k)
  {
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 128);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 128);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    //TIFFSetField(tif, TIFFTAG_TILELENGTH, pxsize);
    //TIFFSetField(tif, TIFFTAG_TILEWIDTH, pxsize);
    for (unsigned i = 0; i < width; ++i)
      TIFFWriteScanline(tif, imgs[k][i], i);
    TIFFWriteDirectory(tif);
  }
  TIFFClose(tif);

  for (unsigned k = 0; k < length; ++k)
  {
    for (unsigned i = 0; i < width; ++i)
      delete[] imgs[k][i];
    delete[] imgs[k];
  }
  delete[] imgs;

  std::cout << "DONE" << std::endl;
}
