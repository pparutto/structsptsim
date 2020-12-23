#include <iostream>
#include <iomanip>
#include <random>
#include <sys/stat.h>

#include <tiffio.h>

#include "tclap/CmdLine.h"

#include "simulation.hh"
#include "simulation_end_condition.hh"
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
  std::string poly_path;

  bool use_poly_pxsize = false;
  double poly_pxsize = NAN;
  bool use_lambda_dist = false;
  double poisson_lambda = NAN;
  std::poisson_distribution<int> pdist;
  double dt = NAN;
  double DT = NAN;
  double D = NAN;
  unsigned Ntrajs = 0;
  unsigned Npts = 0;
  std::string outdir;
  try
  {
    TCLAP::CmdLine cmd("./gensim", ' ', "1");

    TCLAP::ValueArg<double> na1
      ("", "pxsize", "", false, NAN, "Pixel size for polygon (µm)");
    cmd.add(na1);

    TCLAP::ValueArg<double> na2
      ("", "poisson", "", false, NAN, "Poissonian λ traj. length");
    cmd.add(na2);

    TCLAP::UnlabeledValueArg<std::string> poly_arg
      ("poly", "Polygon file", true, "", "polyFile");
    cmd.add(poly_arg);

    TCLAP::UnlabeledValueArg<double> dt_arg
      ("dt", "Simulation timestep (s)", true, NAN, "dt");
    cmd.add(dt_arg);

    TCLAP::UnlabeledValueArg<double> DT_arg
      ("DT", "acquisition timestep (s)", true, NAN, "DT");
    cmd.add(DT_arg);

    TCLAP::UnlabeledValueArg<double> D_arg
      ("D", "Diffusion coefficient (µm2/s)", true, NAN, "D");
    cmd.add(D_arg);

    TCLAP::UnlabeledValueArg<unsigned> Ntrajs_arg
      ("Ntrajs", "Number of trajectories", true, 0, "Nt");
    cmd.add(Ntrajs_arg);

    TCLAP::UnlabeledValueArg<unsigned> Npts_arg
      ("Npts", "Number of points", true, 0, "Np");
    cmd.add(Npts_arg);

    TCLAP::UnlabeledValueArg<std::string> outdir_arg
      ("outdir", "output directory", true, "", "outdir");
    cmd.add(outdir_arg);

    cmd.parse(argc, argv);

    poly_path = poly_arg.getValue();

    if (na1.isSet())
    {
      use_poly_pxsize = true;
      poly_pxsize = na1.getValue();
    }
    if (na2.isSet())
    {
      use_lambda_dist = true;
      poisson_lambda = na2.getValue();
      pdist = std::poisson_distribution<int>(poisson_lambda);
    }

    dt = dt_arg.getValue();
    DT = DT_arg.getValue();
    D = D_arg.getValue();
    Ntrajs = Ntrajs_arg.getValue();
    Npts = Npts_arg.getValue();
    outdir = outdir_arg.getValue();
  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << "error: " << e.error() << " for arg "
	      << e.argId() << std::endl;
  }


  create_dir_if_not_exist(outdir);

  std::cout << std::setprecision(15);

  std::random_device rd;
  std::mt19937_64 mt(rd());

  std::vector<CompoundPolygon> polys =
    polys_from_inkscape_path(poly_path);

  if (use_poly_pxsize)
  {
    for (CompoundPolygon& poly: polys)
      poly.apply_pxsize(poly_pxsize);
  }
  //save_poly_matlab(poly, "/tmp/load_polys.m");


  //FixedPointTrajectoryStartGenerator start_gen({10.0, 10.0});
  MultiplePolysRandomTrajectoryStartGenerator start_gen(mt, polys);

  BrownianMotion bm(mt, D, dt);
  TrajectoryEndCondition* traj_end_cond = nullptr;
  if (use_lambda_dist)
    traj_end_cond = new NumberPointsPoissonianEndCondition(pdist, mt);
  else
    traj_end_cond = new NumberPointsEndCondition(Npts);
  TrajectoryEndConditionFactory traj_end_cond_facto(*traj_end_cond);

  //FullTrajectoryRecorder traj_rec(DT);
  SubsambleTrajectoryRecorder traj_rec(0.0, DT, (unsigned) (DT / dt));
  TrajectoryRecorderFactory traj_rec_facto(traj_rec);

  NumberTrajectoriesSimulationEndCondition end_sim(Ntrajs);

  //NoneCollider collider;
  //PolygonCollider collider(poly);
  MultiplePolygonCollider collider(polys);

  TrajectoryGeneratorFactory traj_gen_facto(start_gen, bm, traj_end_cond_facto,
					    traj_rec_facto, collider);

  SimulationTrajectory sim(traj_gen_facto, end_sim);

  sim.run();

  std::string res_name;
  if (use_lambda_dist)
    res_name = "trajs_D=" + std::to_string(D) +
      "_dt=" + std::to_string(dt) +
      "_DT=" + std::to_string(DT) +
      "_lambdaNpts=" + std::to_string(poisson_lambda) +
      "_ntrajs=" + std::to_string(Ntrajs) +
      ".csv";
  else
    res_name = "trajs_D=" + std::to_string(D) +
      "_dt=" + std::to_string(dt) +
      "_DT=" + std::to_string(DT) +
      "_npts=" + std::to_string(Npts) +
      "_ntrajs=" + std::to_string(Ntrajs) +
      ".csv";

  save_trajectories_csv(outdir + "/" + res_name, sim.trajs());

  delete traj_end_cond;
  std::cout << "DONE" << std::endl;

  unsigned length = 100;
  unsigned width = 128;
  unsigned height = 128;

  unsigned short*** imgs = raw_image_simulator(length, 128, width, height, DT, 1000.0, 0.4, sim.trajs());
  /*
  for (int i = 0; i < 128; ++i)
  {
    for (int j = 0; j < 128; ++j)
      std::cout << imgs[0][i][j] << " ";
    std::cout << std::endl;
  }
  */

  TIFF* tif = TIFFOpen("/tmp/foo.tif", "w");
  for (unsigned k = 0; k < 100; ++k)
  {
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 128);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 128);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    for (unsigned i = 0; i < 128; ++i)
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
}
