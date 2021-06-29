#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <sys/stat.h>

#include <cassert>

#include <tiffio.h>

#include "tclap/CmdLine.h"

#include "simulation.hh"
#include "simulation_end_condition.hh"
#include "io.hh"

#include "cum_distrib_function.hh"

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

enum MotionType {EMPIRICAL, BROWNIAN};
enum TrajLenType {FIXED, EXP};
enum TrajGenType {NTRAJS, NFRAMES};

struct ProgramOptions
{
  std::string poly_path;
  bool export_poly_txt = false;
  bool export_poly_mat = false;
  bool use_poly_pxsize = false;
  double poly_pxsize = NAN;

  TrajLenType tr_len_type;
  unsigned Npts = 0;
  std::exponential_distribution<double> pdist;

  double dt = NAN;
  double DT = NAN;

  TrajGenType tr_gen_type;
  unsigned Ntrajs = 0;
  unsigned Nframes = 0;
  unsigned width = 128;
  unsigned height = 128;
  double spot_dens = NAN;
  std::string outdir;

  MotionType motion_type;
  double D = NAN;
  std::string cdf_path;

  unsigned t_ratio()
  {
    return (unsigned) (this->DT / this->dt);
  }

  unsigned num_particles()
  {
    return (unsigned) (this->width * this->height * this->spot_dens);
  }
};

int main(int argc, char** argv)
{
  ProgramOptions p_opts;
  try
  {
    TCLAP::CmdLine cmd("./gensim", ' ', "1");

    TCLAP::SwitchArg export_poly_txt_arg
      ("", "export-poly-txt", "Export polygon geometry as text file", false);
    cmd.add(export_poly_txt_arg);

    TCLAP::SwitchArg export_poly_mat_arg
      ("", "export-poly-mat", "Export polygon geometry as malab file", false);
    cmd.add(export_poly_mat_arg);

    TCLAP::ValueArg<double> pxsize_arg
      ("", "pxsize", "", false, NAN, "Pixel size for polygon (µm)");
    cmd.add(pxsize_arg);

    TCLAP::ValueArg<double> exp_l_arg
      ("", "npts-exp", "", false, NAN, "Exponential λ traj. length");
    cmd.add(exp_l_arg);

    TCLAP::ValueArg<unsigned> Npts_arg
      ("", "npts-fixed", "", false, 0, "Number of points");
    cmd.add(Npts_arg);

    TCLAP::ValueArg<unsigned> Ntrajs_arg
      ("", "ntrajs", "", false, 0, "Number of trajectories");
    cmd.add(Ntrajs_arg);

    TCLAP::ValueArg<unsigned> Nframes_arg
      ("", "nframes", "", false, 0, "Number of frames");
    cmd.add(Nframes_arg);

    TCLAP::ValueArg<unsigned> width_arg
      ("", "fov-width", "", false, 0, "Width of field of view");
    cmd.add(width_arg);

    TCLAP::ValueArg<unsigned> height_arg
      ("", "fov-height", "", false, 0, "Height of field of view");
    cmd.add(height_arg);

    TCLAP::ValueArg<double> spot_dens_arg
      ("", "spot-density", "", false, 0.0, "Spots / μm^2 / frame");
    cmd.add(spot_dens_arg);

    TCLAP::ValueArg<double> D_arg
      ("", "motion-D", "", false, NAN, "Diffusion coefficient (μm²/s)");
    cmd.add(D_arg);

    TCLAP::ValueArg<std::string> ivel_cdf_arg
      ("", "motion-cdf", "", false, "", "CSV file containing inst. vel. cdf");
    cmd.add(ivel_cdf_arg);

    TCLAP::UnlabeledValueArg<std::string> poly_arg
      ("poly", "Polygon file", true, "", "polyFile");
    cmd.add(poly_arg);

    TCLAP::UnlabeledValueArg<double> dt_arg
      ("dt", "Simulation timestep (s)", true, NAN, "dt");
    cmd.add(dt_arg);

    TCLAP::UnlabeledValueArg<double> DT_arg
      ("DT", "acquisition timestep (s)", true, NAN, "DT");
    cmd.add(DT_arg);

    TCLAP::UnlabeledValueArg<std::string> outdir_arg
      ("outdir", "output directory", true, "", "outdir");
    cmd.add(outdir_arg);

    cmd.parse(argc, argv);

    if (!(Ntrajs_arg.isSet() ^ Nframes_arg.isSet()))
    {
      std::cerr << "ERROR: Either ntrajs or nframes must be set"
		<< std::endl;
      return 0;
    }

    if (Nframes_arg.isSet() && (!spot_dens_arg.isSet() || !width_arg.isSet() ||
				!height_arg.isSet()))
    {
      std::cerr << "ERROR: spot-density, fov-width and fov-height, must be set when selecting nframes"
		<< std::endl;
      return 0;
    }

    if (!(Npts_arg.isSet() ^ exp_l_arg.isSet()))
    {
      std::cerr << "ERROR: either npts-fixed or npts-lambda must be set"
		<< std::endl;
      return 0;
    }

    p_opts.poly_path = poly_arg.getValue();

    p_opts.export_poly_txt = export_poly_txt_arg.getValue();
    p_opts.export_poly_mat = export_poly_mat_arg.getValue();

    if (pxsize_arg.isSet())
    {
      p_opts.use_poly_pxsize = true;
      p_opts.poly_pxsize = pxsize_arg.getValue();
    }

    if (exp_l_arg.isSet())
    {
      p_opts.tr_len_type = EXP;
      p_opts.pdist =
	std::exponential_distribution<double>(1 / exp_l_arg.getValue());
    }
    else
    {
      p_opts.tr_len_type = FIXED;
      p_opts.Npts = Npts_arg.getValue();
    }

    if (Ntrajs_arg.isSet())
    {
      p_opts.tr_gen_type = NTRAJS;
      p_opts.Ntrajs = Ntrajs_arg.getValue();
    }
    else
    {
      p_opts.tr_gen_type= NFRAMES;
      p_opts.Nframes = Nframes_arg.getValue();
      p_opts.spot_dens = spot_dens_arg.getValue();
      p_opts.width = width_arg.getValue();
      p_opts.height = height_arg.getValue();
    }

    if (ivel_cdf_arg.isSet())
   {
     p_opts.motion_type = MotionType::EMPIRICAL;
     p_opts.D = NAN;
     p_opts.cdf_path = ivel_cdf_arg.getValue();
   }
    else if (D_arg.isSet())
   {
     p_opts.motion_type = MotionType::BROWNIAN;
     p_opts.D = D_arg.getValue();
   }
   else
   {
     std::cerr << "ERROR: either motion-cdf or motion-D must be set"
	       << std::endl;
     return 0;
   }

    p_opts.dt = dt_arg.getValue();
    p_opts.DT = DT_arg.getValue();

    p_opts.outdir = outdir_arg.getValue();
  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << "ERROR: " << e.error() << " for arg "
	      << e.argId() << std::endl;
  }


  create_dir_if_not_exist(p_opts.outdir);

  std::cout << std::setprecision(15);

  std::random_device rd;
  std::mt19937_64 mt(rd());

  std::cout << "Parsing polygon" << std::endl;

  std::vector<CompoundPolygon> polys =
    polys_from_inkscape_path(p_opts.poly_path);

  std::cout << "Loaded " << polys.size() << " polygons" << std::endl;
  if (polys.size() == 0)
  {
    std::cerr << "ERROR: no polygon loaded" << std::endl;
    return 0;
  }

  if (p_opts.use_poly_pxsize)
  {
    for (CompoundPolygon& poly: polys)
      poly.apply_pxsize(p_opts.poly_pxsize);
  }

  if (p_opts.export_poly_txt)
  {
    int i = 0;
    for (CompoundPolygon& poly: polys)
    {
      save_poly_txt(poly, p_opts.outdir + "/poly_" + std::to_string(i) + ".txt");
      ++i;
    }
  }

  if (p_opts.export_poly_mat)
    save_polys_matlab(polys, p_opts.outdir + "/polys.m");


  //FixedPointTrajectoryStartGenerator start_gen({10.0, 10.0});
  MultiplePolysRandomTrajectoryStartGenerator start_gen(mt, polys);

  Motion* motion = nullptr;
  CumDistribFunction* ivel_cdf = nullptr;
  if (p_opts.motion_type == MotionType::BROWNIAN)
    motion = new BrownianMotion(mt, p_opts.D, p_opts.dt);
  else if (p_opts.motion_type == MotionType::EMPIRICAL)
  {
    ivel_cdf = new CumDistribFunction(p_opts.cdf_path);
    motion = new EmpiricalMotion(mt, *ivel_cdf, p_opts.DT);
  }

  TrajectoryEndCondition* traj_end_cond = nullptr;
  if (p_opts.tr_len_type == EXP)
    traj_end_cond = new NumberPointsExpEndCondition(p_opts.pdist, mt);
  else if (p_opts.tr_len_type == FIXED)
    traj_end_cond = new NumberPointsEndCondition(p_opts.Npts);
  TrajectoryEndConditionFactory traj_end_cond_facto(*traj_end_cond);

  //FullTrajectoryRecorder traj_rec(DT);
  SubsambleTrajectoryRecorder traj_rec(0.0, p_opts.DT, p_opts.t_ratio());
  TrajectoryRecorderFactory traj_rec_facto(traj_rec);


  //NoneCollider collider;
  //PolygonCollider collider(poly);
  MultiplePolygonCollider collider(polys);

  TrajectoryGeneratorFactory traj_gen_facto(start_gen, *motion,
					    traj_end_cond_facto, traj_rec_facto,
					    collider);

  SimulationEndCondition* end_sim = nullptr;
  Simulation* sim = nullptr;
  if (p_opts.tr_gen_type == NTRAJS)
  {
    end_sim = new NumberTrajectoriesSimulationEndCondition(p_opts.Ntrajs);
    sim = new SimulationTrajectory(traj_gen_facto, *end_sim);
  }
  else if (p_opts.tr_gen_type == NFRAMES)
  {
    std::cout << "Density x fov area = " << p_opts.num_particles()
	      << " spots per frame" << std::endl;
    end_sim = new NumberFramesSimulationEndCondition((int) p_opts.Nframes);
    sim = new SimulationDensity(traj_gen_facto, *end_sim, p_opts.num_particles(),
				p_opts.DT, p_opts.t_ratio());
  }
  else
    assert(false);

  std::cout << "Running simulation" << std::endl;
  sim->run();

  std::string res_name = "trajs";

  if (p_opts.motion_type == MotionType::BROWNIAN)
    res_name += "_D=" + std::to_string(p_opts.D);
  else
    res_name += "_empirical";

  res_name += "_dt=" + std::to_string(p_opts.dt) +
    "_DT=" + std::to_string(p_opts.DT);

  if (p_opts.tr_len_type == EXP)
    res_name += "_lambdaNpts=" + std::to_string(p_opts.pdist.lambda());
  else
    res_name += "_npts=" + std::to_string(p_opts.Npts);

  if (p_opts.tr_gen_type == NTRAJS)
    res_name += "_ntrajs=" + std::to_string(p_opts.Ntrajs);
  else
    res_name += "_nframes=" + std::to_string(p_opts.Ntrajs) +
      "_width=" + std::to_string(p_opts.width) +
      "_height=" + std::to_string(p_opts.height) +
      "_dens=" + std::to_string(p_opts.spot_dens);

  res_name += ".csv";

  save_trajectories_csv(p_opts.outdir + "/" + res_name, sim->trajs());

  delete traj_end_cond;

  std::cout << "Generating images" << std::endl;
  unsigned length = 0;
  if (p_opts.tr_gen_type == NFRAMES)
    length = p_opts.Nframes;
  else
  {
    for (const Trajectory& traj: sim->trajs())
      length = traj.size() > length ? traj.size() : length;
  }

  unsigned short*** imgs =
    raw_image_simulator(length, p_opts.width, p_opts.height, p_opts.poly_pxsize,
			p_opts.DT, 1000.0, 0.2, sim->trajs());

  TIFF* tif = TIFFOpen((p_opts.outdir +"/simulated_raw_data.tif").c_str(),
		       "w");
  for (unsigned k = 0; k < length; ++k)
  {
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, p_opts.width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, p_opts.height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    for (unsigned i = 0; i < p_opts.width; ++i)
      TIFFWriteScanline(tif, imgs[k][i], i);
    TIFFWriteDirectory(tif);
  }
  TIFFClose(tif);

  for (unsigned k = 0; k < length; ++k)
  {
    for (unsigned i = 0; i < p_opts.width; ++i)
      delete[] imgs[k][i];
    delete[] imgs[k];
  }
  delete[] imgs;

  delete sim;
  delete end_sim;
  delete ivel_cdf;
  delete motion;

  std::cout << "DONE" << std::endl;
}
