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
#include "gensim_io.hh"

#include "cum_distrib_function.hh"

#include <tiffio.h>
#include "raw_image_simulator.hh"

#include "quadtree.hh"

void generate_tif_stack(const TrajectoryEnsemble<2>& trajs, unsigned width,
			unsigned height, unsigned length, double pxsize,
			double DT, const std::string& outfile)
{
  //Pierre: add the two fixed values as arguments
  unsigned short*** imgs =
    raw_image_simulator(length, width, height, pxsize, DT,
			10000.0, 0.1, trajs);

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
    for (unsigned i = 0; i < height; ++i)
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
    ArgumentParserOptions arg_parse_opts;

    arg_parse_opts.add_arguments(cmd);
    cmd.parse(argc, argv);

    arg_parse_opts.verify_command_line();
    arg_parse_opts.fill_program_options(p_opts);
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

  std::cout << "Seed: " << p_opts.seed << std::endl;
  std::mt19937_64 mt(p_opts.seed);

  std::cout << "Parsing polygon" << std::endl;

  Shape<2>* poly = nullptr;
  Box<2> sim_reg;
  Box<2> fov;
  Shape<2>* sim_reg_ptr = nullptr;
  QuadTree* qt = nullptr;
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

    std::cout << "Polygon(s) size: " << polys->segments().size() << std::endl;

    if (p_opts.use_pxsize)
    {
      polys->apply_pxsize(p_opts.pxsize);
      polys->round_poly_pts();
    }

    polys->shift_coords({100.0, 100.0});

    Box<2> bb = polys->bounding_box();
    if (p_opts.use_fov)
      bb = Box<2>({0, 0}, {(p_opts.fov_size[0] - 1) * p_opts.pxsize,
			   (p_opts.fov_size[1] - 1) * p_opts.pxsize});

    qt = new QuadTree(bb);
    qt->insert_segments(polys->segments(), 5); //make it a parameter

    std::cout << "QuadTree size: " << qt->size() << std::endl;
    std::cout << "QuadTree area: " << qt->area();
  }
  else if (p_opts.use_fov)
  {
    sim_reg = Box<2>({0, 0}, {p_opts.fov_size[0] - 1.0,
			      p_opts.fov_size[1] - 1.0});
    sim_reg.scale(p_opts.pxsize);
    sim_reg.shift_coords({100.0, 100.0});
    poly = new Box<2> (sim_reg);
  }

  //setup limit region for trajectories
  if (p_opts.use_sim_reg)
  {    
    sim_reg = p_opts.sim_reg;
    sim_reg.scale(p_opts.pxsize);
    sim_reg.shift_coords({100.0, 100.0});
    sim_reg_ptr = &sim_reg;
  }

  TrajectoryStartGenerator<2>* start_gen = nullptr;
  if (p_opts.use_start_point)
  {
    std::cout << "Start point generator: Fixed" << std::endl;
    start_gen = new FixedPointTrajectoryStartGenerator<2>(p_opts.start_point + Point<2>({100.0, 100.0}));
  }
  else if (p_opts.tr_gen_type == TrajGenType::EMPIRICAL)
  {
    std::cout << "Start point generator: Fixed" << std::endl;
    start_gen = new FixedPointTrajectoryStartGenerator<2>({100.0, 100.0});
  }
  else if (p_opts.use_start_reg)
  {
    p_opts.start_reg.scale(p_opts.pxsize);
    p_opts.start_reg.shift_coords({100.0, 100.0});
    std::cout << "Start point generator: Box " << p_opts.start_reg;
    start_gen = new RandomBoxInPolyTrajectoryStartGenerator(mt, *poly, p_opts.start_reg);
  }
  else if (p_opts.use_poly)
  {
    std::cout << "Start point generator: Poly" << std::endl;
    start_gen = new MultiplePolysRandomTrajectoryStartGenerator(mt, *dynamic_cast<MultiplePolygon*>(poly));
  }
  else if (p_opts.use_fov)
  {
    std::cout << "Start point generator: Poly bounding box" << std::endl;
    start_gen = new RandomBoxTrajectoryStartGenerator(mt, *dynamic_cast<Box<2>*>(poly));
  }
  else
    assert(false);

  Motion<2>* motion = nullptr;
  CumDistribFunction* ivel_cdf = nullptr;
  if (p_opts.motion_type == MotionType::BROWNIAN)
    motion = new BrownianMotion<2>(mt, p_opts.D, p_opts.dt);
  else if (p_opts.motion_type == MotionType::DISTRIB)
  {
    ivel_cdf = new CumDistribFunction(mt, p_opts.cdf_path);
    motion = new EmpiricalMotion(mt, *ivel_cdf, p_opts.DT);
  }
  else if (p_opts.motion_type == MotionType::HMM)
  {
    motion = new HMM2DMotion(mt, p_opts.Ds, p_opts.rates, p_opts.dt);
    std::cout << ((HMM2DMotion*) motion)->to_str() << std::endl;
  }

  TrajectoryEndCondition<2>* traj_end_cond = nullptr;
  if (p_opts.tr_len_type == EXP)
  {
    std::cout << "Trajectory end condition: exponential npts (lambda="
	      << (double) p_opts.pdist.lambda() << ")" << std::endl;
    traj_end_cond = new NumberPointsExpEndCondition<2>(p_opts.pdist, mt);
  }
  else if (p_opts.tr_len_type == FIXED)
  {
    std::cout << "Trajectory end condition: npts="
	      << p_opts.Npts << std::endl;
    traj_end_cond = new NumberPointsEndCondition<2>(p_opts.Npts);
  }
  else if (p_opts.tr_len_type == REG)
  {
    std::cout << "Trajectory end condition: enter region" << std::endl;
    p_opts.stop_reg.shift_coords({100.0, 100.0});
    traj_end_cond = new EnterRegionEndCondition<2>(p_opts.stop_reg);
  }
  else if (p_opts.tr_gen_type == TrajGenType::EMPIRICAL)
  {
    std::cout << "Trajectory end condition: empirical" << std::endl;
    traj_end_cond = new NumberPointsEndCondition<2>(0);
  }
  else
    assert(false);

  if (p_opts.use_fov && !p_opts.use_sim_reg)
  {
    fov = Box<2>({0, 0}, {p_opts.fov_size[0] - 1.0,
			  p_opts.fov_size[1] - 1.0});
    fov.scale(p_opts.pxsize);
    fov.shift_coords({100.0, 100.0});

    std::cout << "Trajectory end condition 2: exit region "
	      << fov;
    std::vector<TrajectoryEndCondition<2>*> conds;
    conds.push_back(traj_end_cond);
    conds.push_back(new EscapeEndCondition<2>(fov));
    traj_end_cond = new CompoundEndCondition<2>(conds);
  }

  if (p_opts.use_sim_reg)
  {
    std::cout << "Trajectory end condition 2: exit region "
	      << sim_reg;
    std::vector<TrajectoryEndCondition<2>*> conds;
    conds.push_back(traj_end_cond);
    conds.push_back(new EscapeEndCondition<2>(sim_reg));
    traj_end_cond = new CompoundEndCondition<2>(conds);
  }

  TrajectoryEndConditionFactory<2> traj_end_cond_facto(*traj_end_cond);
  
  TrajectoryRecorder<2>* traj_rec = nullptr;
  if (motion->subsample())
    traj_rec =
      new SubsampleTrajectoryRecorder<2>(0.0, p_opts.DT, p_opts.t_ratio);
  else
    traj_rec = new FullTrajectoryRecorder<2>(0.0, p_opts.DT);

  TrajectoryRecorderFactory<2> traj_rec_facto(*traj_rec);


  Collider<2>* collider = nullptr;
  if (p_opts.use_poly)
    collider =
      //new MultiplePolygonCollider(*dynamic_cast<MultiplePolygon*>(poly), qt);
      new QuadTreeCollider(*dynamic_cast<MultiplePolygon*>(poly), qt);
  else
    collider = new NoneCollider<2>();

  //BufferLogger* log = new BufferLogger();
  std::ofstream log_ofs(p_opts.outdir + "/errors");
  Logger* log = new DirectFileLogger(log_ofs);

  std::cout << "Collider: "; collider->who_am_I(std::cout);
  TrajectoryGeneratorFactory<2>
    traj_gen_facto(*start_gen, *motion, traj_end_cond_facto, traj_rec_facto,
		   *collider, sim_reg_ptr, log);

  SimulationEndCondition<2>* end_sim = nullptr;
  Simulation<2>* sim = nullptr;
  if (p_opts.tr_gen_type == TrajGenType::NTRAJS)
  {
    end_sim = new NumberTrajectoriesSimulationEndCondition<2>(p_opts.Ntrajs);
    sim = new SimulationTrajectory<2>(traj_gen_facto, *end_sim);
  }
  else if (p_opts.tr_gen_type == TrajGenType::NFRAMES)
  {
    std::cout << "Density x fov area = " << p_opts.num_particles()
	      << " spots per frame" << std::endl;
    end_sim = new NumberFramesSimulationEndCondition<2>((int) p_opts.Nframes);
    sim = new SimulationDensity(traj_gen_facto, *end_sim, p_opts.num_particles(),
				p_opts.DT, p_opts.t_ratio);
  }
  else if (p_opts.tr_gen_type == TrajGenType::EMPIRICAL)
  {
    sim = new SimulationEmpirical(traj_gen_facto,
				  p_opts.empirical_trajs, p_opts.DT);
  }
  else
    assert(false);

  p_opts.save_csv(p_opts.outdir + "/params.csv");

  std::cout << "Running simulation" << std::endl;
  sim->run();
  sim->shift_trajs_coords({-100.0, -100.0});

  save_trajectories_csv<2>(p_opts.outdir + "/trajs.csv", sim->trajs());

  if (p_opts.export_poly_mat)
  {
    MultiplePolygon* polys = dynamic_cast<MultiplePolygon*>(poly);
    polys->shift_coords({-100.0, -100.0});
    save_polys_matlab(*polys, p_opts.outdir + "/polys.m");

    if (p_opts.use_start_reg)
    {
      p_opts.start_reg.shift_coords({-100.0, -100.0});
      save_box_matlab(p_opts.start_reg, p_opts.outdir + "/start_box.m",
		      "start_box");
    }
    if (p_opts.tr_len_type == TrajLenType::REG)
    {
      p_opts.stop_reg.shift_coords({-100.0, -100.0});
      save_box_matlab(p_opts.stop_reg, p_opts.outdir + "/stop_box.m",
		      "stop_box");
    }
    polys->shift_coords({100.0, 100.0});
  }
  
  if (p_opts.export_poly_txt)
  {
    MultiplePolygon* polys = dynamic_cast<MultiplePolygon*>(poly);
    polys->shift_coords({-100.0, -100.0});
    int i = 0;
    for (const CompoundPolygon& poly: polys->polys())
     {
       save_poly_txt(poly, p_opts.outdir + "/poly_" + std::to_string(i) + ".txt");
       ++i;
     }
  }

  if (!p_opts.noimg && p_opts.tr_gen_type == TrajGenType::NFRAMES)
  {
    std::cout << "Generating images" << std::endl;

    unsigned length = 0;
    if (p_opts.tr_gen_type == NFRAMES)
      length = p_opts.Nframes;
    else
    {
      for (const Trajectory<2>& traj: sim->trajs())
	length = traj.size() > length ? traj.size() : length;
    }

    generate_tif_stack(sim->trajs(), p_opts.fov_size[0], p_opts.fov_size[1],
		       length, p_opts.pxsize, p_opts.DT,
		       p_opts.outdir + "/simulated_raw_data.tif");
  }

  BufferLogger* bl = dynamic_cast<BufferLogger*> (log);
  if (bl != NULL)
    bl->save_to_file(p_opts.outdir + "/errors");
  if (dynamic_cast<DirectFileLogger*> (log) != NULL)
    log_ofs.close();

  delete traj_end_cond;
  delete log;
  delete poly;
  delete qt;
  delete collider;
  delete start_gen;
  delete traj_rec;
  delete sim;
  delete end_sim;
  delete ivel_cdf;
  delete motion;

  std::cout << "DONE" << std::endl;
}
