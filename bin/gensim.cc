#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <sys/stat.h>
#include <sys/types.h>

#include <cassert>

#include "tclap/CmdLine.h"

#include "simulation.hh"
#include "simulation_end_condition.hh"
#include "io.hh"

#include "cum_distrib_function.hh"

#include "utils.hh"

#include <tiffio.h>
#include "raw_image_simulator.hh"

void generate_tif_stack(const TrajectoryEnsemble& trajs, double width,
			double height, unsigned length, double pxsize,
			double DT, const std::string& outfile)
{
  //Pierre: add the two fixed values as arguments
  unsigned short*** imgs =
    raw_image_simulator(length, width, height, pxsize, DT,
			1000.0, 0.2, trajs);

  TIFF* tif = TIFFOpen(outfile.c_str(), "w");
  for (unsigned k = 0; k < length; ++k)
  {
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    for (unsigned i = 0; i < height; ++i)
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

void create_dir_if_not_exist(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR))
    {
      mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      std::cout << "Created directory: " << path << std::endl;
    }
}

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

    TCLAP::ValueArg<std::string> start_box_arg
      ("", "start-box", "", false, "",
       "Box to start trajectories (format: llx,lly;trx,try)");
    cmd.add(start_box_arg);

    TCLAP::ValueArg<std::string> stop_box_arg
      ("", "stop-box", "", false, "",
       "Box to end trajectories (format: llx,lly;trx,try)");
    cmd.add(stop_box_arg);

    TCLAP::ValueArg<double> exp_l_arg
      ("", "npts-exp", "", false, NAN, "Exponential λ traj. length");
    cmd.add(exp_l_arg);

    TCLAP::ValueArg<std::string> empirical_tr_arg
      ("", "empirical-trajs", "", false, "", "Empirical trajectories");
    cmd.add(empirical_tr_arg);

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

    TCLAP::ValueArg<std::string> poly_arg
      ("", "poly", "", false, "", "Path to polygon file");
    cmd.add(poly_arg);

    TCLAP::ValueArg<double> dt_arg
      ("", "dt", "", false, NAN, "Simulation timestep (s)");
    cmd.add(dt_arg);

    TCLAP::UnlabeledValueArg<double> DT_arg
      ("DT", "acquisition timestep (s)", true, NAN, "DT");
    cmd.add(DT_arg);

    TCLAP::UnlabeledValueArg<std::string> outdir_arg
      ("outdir", "output directory", true, "", "outdir");
    cmd.add(outdir_arg);

    cmd.parse(argc, argv);

    if (!(Ntrajs_arg.isSet() ^ Nframes_arg.isSet() ^ empirical_tr_arg.isSet()))
    {
      std::cerr << "ERROR: Either ntrajs, nframes or empirical-trajs must be set"
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

    if (!(Npts_arg.isSet() ^ exp_l_arg.isSet() ^ empirical_tr_arg.isSet() ^
	  stop_box_arg.isSet()))
    {
      std::cerr << "ERROR: either npts-fixed, npts-exp, empirical-trajs or stop-box must be set"
		<< std::endl;
      return 0;
    }

    if (D_arg.isSet() && !dt_arg.isSet())
    {
      std::cerr << "ERROR: dt must be set if motion is diffusive" << std::endl;
      return 0;
    }

    if (poly_arg.isSet())
    {
      p_opts.use_poly = true;
      p_opts.poly_path = poly_arg.getValue();
    }

    p_opts.export_poly_txt = export_poly_txt_arg.getValue();
    p_opts.export_poly_mat = export_poly_mat_arg.getValue();

    if (pxsize_arg.isSet())
    {
      p_opts.use_pxsize = true;
      p_opts.pxsize = pxsize_arg.getValue();
    }

    if (start_box_arg.isSet())
    {
      p_opts.use_start_reg = true;
      p_opts.start_reg = parse_box(start_box_arg.getValue());
    }

    if (stop_box_arg.isSet())
    {
      p_opts.tr_len_type = TrajLenType::REG;
      p_opts.stop_reg = parse_box(stop_box_arg.getValue());
    }
    else if (exp_l_arg.isSet())
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
      p_opts.tr_gen_type = TrajGenType::NTRAJS;
      p_opts.Ntrajs = Ntrajs_arg.getValue();
    }
    else if (Nframes_arg.isSet())
    {
      p_opts.tr_gen_type = TrajGenType::NFRAMES;
      p_opts.Nframes = Nframes_arg.getValue();
      p_opts.spot_dens = spot_dens_arg.getValue();
      p_opts.width = width_arg.getValue();
      p_opts.height = height_arg.getValue();
    }
    else if (empirical_tr_arg.isSet())
    {
      p_opts.tr_gen_type = TrajGenType::EMPIRICAL;
      p_opts.empirical_trajs_file = empirical_tr_arg.getValue();
      p_opts.empirical_trajs = load_characs(empirical_tr_arg.getValue());
    }

    if (ivel_cdf_arg.isSet())
    {
      p_opts.motion_type = MotionType::DISTRIB;
      p_opts.D = NAN;
      p_opts.cdf_path = ivel_cdf_arg.getValue();
    }
    else if (D_arg.isSet())
    {
      p_opts.motion_type = MotionType::BROWNIAN;
      p_opts.D = D_arg.getValue();
      p_opts.dt = dt_arg.getValue();
    }
    else
    {
      std::cerr << "ERROR: either motion-cdf or motion-D must be set"
		<< std::endl;
      return 0;
    }

    p_opts.DT = DT_arg.getValue();

    p_opts.outdir = outdir_arg.getValue();
  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << "ERROR: " << e.error() << " for arg "
	      << e.argId() << std::endl;
  }


  create_dir_if_not_exist(p_opts.outdir);
  //the above function will create only one sub-directory
  //if multiple are missing it will fail.
  {
    struct stat info;
    if (errno != EEXIST &&
	(stat(p_opts.outdir.c_str(), &info) != 0 || !S_ISDIR(info.st_mode)))
    {
      std::cerr << "ERROR: could not access output directory:"
		<< p_opts.outdir << std::endl;
    return 0;
    }
  }

  std::cout << std::setprecision(15);

  std::random_device rd;
  std::mt19937_64 mt(rd());

  std::cout << "Parsing polygon" << std::endl;

  Shape* poly = nullptr;
  if (p_opts.use_poly)
  {
    MultiplePolygon* polys =
      polys_from_inkscape_path(p_opts.poly_path);
    poly = polys;

    if (polys->empty())
    {
      std::cerr << "ERROR: no polygon loaded" << std::endl;
      return 0;
    }

    if (p_opts.use_pxsize)
      polys->apply_pxsize(p_opts.pxsize);

    if (p_opts.export_poly_txt)
    {
      int i = 0;
      for (const CompoundPolygon& poly: polys->polys())
      {
	save_poly_txt(poly, p_opts.outdir + "/poly_" + std::to_string(i) + ".txt");
	++i;
      }
    }

    if (p_opts.export_poly_mat)
    {
      save_polys_matlab(*polys, p_opts.outdir + "/polys.m");

      if (p_opts.use_start_reg)
	save_box_matlab(p_opts.start_reg, p_opts.outdir + "/start_box.m",
			"start_box");
      if (p_opts.tr_len_type == TrajLenType::REG)
	save_box_matlab(p_opts.stop_reg, p_opts.outdir + "/stop_box.m",
			"stop_box");
    }
  }
  else
  {
    double region_scale = 0.04;
    poly = new Box({region_scale * p_opts.width * p_opts.pxsize,
		    region_scale * p_opts.width * p_opts.pxsize},
      {(1 - region_scale) * p_opts.width * p_opts.pxsize,
       (1 - region_scale) * p_opts.height * p_opts.pxsize});
  }

  //FixedPointTrajectoryStartGenerator start_gen({10.0, 10.0});

  TrajectoryStartGenerator* start_gen = nullptr;
  if (p_opts.tr_gen_type == TrajGenType::EMPIRICAL)
  {
    std::cout << "Start point generator: Fixed" << std::endl;
    start_gen = new FixedPointTrajectoryStartGenerator({0.0, 0.0});
  }
  else if (p_opts.use_start_reg)
  {
    std::cout << "Start point generator: Box" << std::endl;
    start_gen = new RandomBoxInPolyTrajectoryStartGenerator(mt, *poly, p_opts.start_reg);
  }
  else if (p_opts.use_poly)
  {
    std::cout << "Start point generator: Poly" << std::endl;
    start_gen = new MultiplePolysRandomTrajectoryStartGenerator(mt, *dynamic_cast<MultiplePolygon*>(poly));
  }
  else
  {
    std::cout << "Start point generator: Poly bounding box" << std::endl;
    start_gen = new RandomBoxTrajectoryStartGenerator(mt, *dynamic_cast<Box*>(poly));
  }


  Motion* motion = nullptr;
  CumDistribFunction* ivel_cdf = nullptr;
  if (p_opts.motion_type == MotionType::BROWNIAN)
    motion = new BrownianMotion(mt, p_opts.D, p_opts.dt);
  else if (p_opts.motion_type == MotionType::DISTRIB)
  {
    ivel_cdf = new CumDistribFunction(mt, p_opts.cdf_path);
    motion = new EmpiricalMotion(mt, *ivel_cdf, p_opts.DT);
  }

  TrajectoryEndCondition* traj_end_cond = nullptr;
  if (p_opts.tr_len_type == EXP)
  {
    std::cout << "Trajectory end condition: exponential npts (lambda="
	      << (double) p_opts.pdist.lambda() << ")" << std::endl;
    traj_end_cond = new NumberPointsExpEndCondition(p_opts.pdist, mt);
  }
  else if (p_opts.tr_len_type == FIXED)
  {
    std::cout << "Trajectory end condition: npts="
	      << p_opts.Npts << std::endl;
    traj_end_cond = new NumberPointsEndCondition(p_opts.Npts);
  }
  else if (p_opts.tr_len_type == REG)
  {
    std::cout << "Trajectory end condition: enter region" << std::endl;
    traj_end_cond = new EnterRegionEndCondition(p_opts.stop_reg);
  }
  else if (p_opts.tr_gen_type == TrajGenType::EMPIRICAL)
  {
    std::cout << "Trajectory end condition: empirical" << std::endl;
    traj_end_cond = new NumberPointsEndCondition(0);
  }
  else
    assert(false);

  if (!p_opts.use_poly)
  {
    std::vector<TrajectoryEndCondition*> conds;
    conds.push_back(traj_end_cond);
    conds.push_back(new EscapeEndCondition(*poly));
    traj_end_cond = new CompoundEndCondition(conds);
  }

  TrajectoryEndConditionFactory traj_end_cond_facto(*traj_end_cond);

  TrajectoryRecorder* traj_rec = nullptr;
  if (p_opts.motion_type == MotionType::BROWNIAN)
    traj_rec =
      new SubsambleTrajectoryRecorder(0.0, p_opts.DT, p_opts.t_ratio());
  else if (p_opts.motion_type == MotionType::DISTRIB)
    traj_rec = new FullTrajectoryRecorder(0.0, p_opts.DT);

  TrajectoryRecorderFactory traj_rec_facto(*traj_rec);


  Collider* collider = nullptr;
  if (p_opts.use_poly)
    collider =
      new MultiplePolygonCollider(*dynamic_cast<MultiplePolygon*>(poly));
  else
    collider = new NoneCollider();

  TrajectoryGeneratorFactory traj_gen_facto(*start_gen, *motion,
					    traj_end_cond_facto, traj_rec_facto,
					    *collider);

  SimulationEndCondition* end_sim = nullptr;
  Simulation* sim = nullptr;
  if (p_opts.tr_gen_type == TrajGenType::NTRAJS)
  {
    end_sim = new NumberTrajectoriesSimulationEndCondition(p_opts.Ntrajs);
    sim = new SimulationTrajectory(traj_gen_facto, *end_sim);
  }
  else if (p_opts.tr_gen_type == TrajGenType::NFRAMES)
  {
    std::cout << "Density x fov area = " << p_opts.num_particles()
	      << " spots per frame" << std::endl;
    end_sim = new NumberFramesSimulationEndCondition((int) p_opts.Nframes);
    sim = new SimulationDensity(traj_gen_facto, *end_sim, p_opts.num_particles(),
				p_opts.DT, p_opts.t_ratio());
  }
  else if (p_opts.tr_gen_type == TrajGenType::EMPIRICAL)
  {
    sim = new SimulationEmpirical(traj_gen_facto,
				  p_opts.empirical_trajs, p_opts.DT);
  }
  else
    assert(false);

  std::cout << "Running simulation" << std::endl;
  sim->run();


  save_params_csv(p_opts.outdir + "/params.csv", p_opts);
  save_trajectories_csv(p_opts.outdir + "/trajs.csv", sim->trajs());

  delete traj_end_cond;

  if (p_opts.tr_gen_type == TrajGenType::NFRAMES)
  {
    std::cout << "Generating images" << std::endl;

    unsigned length = 0;
    if (p_opts.tr_gen_type == NFRAMES)
      length = p_opts.Nframes;
    else
    {
      for (const Trajectory& traj: sim->trajs())
	length = traj.size() > length ? traj.size() : length;
    }
    generate_tif_stack(sim->trajs(), p_opts.width, p_opts.height,
		       length, p_opts.pxsize, p_opts.DT,
		       p_opts.outdir + "/simulated_raw_data.tif");
  }

  delete poly;
  delete collider;
  delete start_gen;
  delete traj_rec;
  delete sim;
  delete end_sim;
  delete ivel_cdf;
  delete motion;

  std::cout << "DONE" << std::endl;
}
