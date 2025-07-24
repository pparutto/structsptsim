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
  //1000.0, 0.2

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
  bool runover = false;
  bool nokill = false;
  double tmax = NAN;
  double t0 = NAN;
  unsigned seed = 0;
  double pxsize = NAN;
  unsigned Nframes = 0;
  double dt = NAN;
  double DT = NAN;
  double D = NAN;
  double prod_rate = NAN;
  double kill_rate = NAN;
  std::exponential_distribution<double> kill_pdist;
  Point<2> fov_size = {NAN, NAN};
  Point<2> pa_loc = {NAN, NAN};
  double pa_radius = NAN;
  std::string poly_path;
  std::string outdir;

  try
  {
    TCLAP::CmdLine cmd("./genPAsim", ' ', "1");
    ArgumentParserOptions arg_parse_opts;

    TCLAP::SwitchArg runover_arg
      ("", "runover", "continue simulating trajs going after final frame", false);
    cmd.add(runover_arg);

    TCLAP::SwitchArg nokill_arg
      ("", "nokill", "do not kill trajectories", false);
    cmd.add(nokill_arg);

    TCLAP::ValueArg<double> t0_arg
      ("", "t0", "", false, 0.0, "max time");
    cmd.add(t0_arg);

    TCLAP::ValueArg<double> tmax_arg
      ("", "tmax", "", false, NAN, "max time");
    cmd.add(tmax_arg);

    TCLAP::ValueArg<unsigned> seed_arg
      ("", "seed", "", false, 0, "Seed for random number generator");
    cmd.add(seed_arg);

    TCLAP::UnlabeledValueArg<unsigned> Nframes_arg
      ("Nframes", "Number of simulation frames", true, 0, "Nframes");
    cmd.add(Nframes_arg);

    TCLAP::UnlabeledValueArg<double> pxsize_arg
      ("pxs", "Pixel size for polygon (µm)", true, NAN, "pxsize");
    cmd.add(pxsize_arg);

    TCLAP::UnlabeledValueArg<double> dt_arg
      ("dt", "Simulation timestep (s)", true, NAN, "dt");
    cmd.add(dt_arg);

    TCLAP::UnlabeledValueArg<double> DT_arg
      ("DT", "acquisition timestep (s)", true, NAN, "DT");
    cmd.add(DT_arg);

    TCLAP::UnlabeledValueArg<double> D_arg
      ("D", "Diffusion coefficient (µm2/s)", true, NAN, "D");
    cmd.add(D_arg);

    TCLAP::UnlabeledValueArg<double> prod_rate_arg
      ("Pr", "production rate (trajs/s)", true, NAN, "Pr");
    cmd.add(prod_rate_arg);

    TCLAP::UnlabeledValueArg<double> kill_rate_arg
      ("Kr", "Killing rate (1/s)", true, NAN, "Kr");
    cmd.add(kill_rate_arg);

    TCLAP::UnlabeledValueArg<std::string> fov_size_arg
      ("FOV", "fov-size", true, "", "Width,Height of field of view in pixels");
      cmd.add(fov_size_arg);

    TCLAP::UnlabeledValueArg<std::string> pa_loc_arg
      ("PAc", "PA-center", true, "", "x,y center of PA region (µm)");
    cmd.add(pa_loc_arg);

    TCLAP::UnlabeledValueArg<double> pa_radius_arg
      ("PAr", "Photoactivation region radius (µm)", true, NAN, "PAr");
    cmd.add(pa_radius_arg);

    TCLAP::UnlabeledValueArg<std::string> poly_path_arg
      ("poly", "Path to polygon file", true, "", "Poly");
    cmd.add(poly_path_arg);

    TCLAP::UnlabeledValueArg<std::string> outdir_arg
      ("outdir", "output directory", true, "", "outdir");
    cmd.add(outdir_arg);

    cmd.parse(argc, argv);


    if (seed_arg.isSet())
      seed = seed_arg.getValue();
    else
    {
      std::random_device rd;
      seed = rd();
    }

    if (nokill_arg.isSet() && !tmax_arg.isSet())
    {
      std::cerr << "--tmax must be set if --nokill is set" << std::endl;
      return 1;
    }

    runover = runover_arg.isSet();
    nokill = nokill_arg.isSet();
    t0 = t0_arg.getValue();
    if (tmax_arg.isSet())
      tmax = tmax_arg.getValue();
    Nframes = Nframes_arg.getValue();
    pxsize = pxsize_arg.getValue();
    dt = dt_arg.getValue();
    DT = DT_arg.getValue();
    D = D_arg.getValue();
    prod_rate = prod_rate_arg.getValue();
    kill_rate = kill_rate_arg.getValue();
    kill_pdist = std::exponential_distribution<double>(1 / kill_rate);
    fov_size = ArgumentParserOptions::read_point(fov_size_arg.getValue());
    pa_loc = ArgumentParserOptions::read_point(pa_loc_arg.getValue());
    pa_radius = pa_radius_arg.getValue();
    poly_path = poly_path_arg.getValue();
    outdir = outdir_arg.getValue();
  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << "ERROR: " << e.error() << " for arg "
	      << e.argId() << std::endl;
  }

  create_dir_if_not_exist(outdir);
  //the above function will create only one sub-directory
  //if multiple are missing it will fail.
  {
    struct stat info;
    if (errno != EEXIST &&
	(stat(outdir.c_str(), &info) != 0 || !S_ISDIR(info.st_mode)))
    {
      std::cerr << "ERROR: could not access output directory:"
		<< outdir << std::endl;
      return 0;
    }
  }

  //save_csv(outdir + "/params.csv");

  std::cout << std::setprecision(15);

  std::cout << "Seed: " << seed << std::endl;
  std::mt19937_64 mt(seed);

  std::cout << "Parsing polygon" << std::endl;
  MultiplePolygon* poly =
    polys_from_inkscape_path(poly_path);

  if (poly->empty())
  {
    std::cerr << "ERROR: no polygon loaded" << std::endl;
    return 0;
  }
  std::cout << "Polygon(s) size: " << poly->segments().size() << std::endl;

  poly->apply_pxsize(pxsize);
  poly->round_poly_pts();

  save_polys_matlab(*poly, outdir + "/polys.m");
  poly->shift_coords({100.0, 100.0});

  Box<2> bb = poly->bounding_box();
  QuadTree* qt = new QuadTree(bb);
  qt->insert_segments(poly->segments(), 13); //make it a parameter //5

  std::cout << "QuadTree size: " << qt->size() << std::endl;
  std::cout << "QuadTree area: " << qt->area();

  Box<2> start_reg({pa_loc[0] - pa_radius, pa_loc[1] - pa_radius},
		   {pa_loc[0] + pa_radius, pa_loc[1] + pa_radius});

  save_box_matlab(start_reg, outdir + "/start_box.m", "start_box");
  start_reg.shift_coords({100.0, 100.0});

  TrajectoryStartGenerator<2>* start_gen =
    new RandomBoxInPolyTrajectoryStartGenerator(mt, *poly, start_reg);

  std::vector<Motion<2>*> motions;
  motions.push_back(new BrownianMotion<2>(mt, D, dt));

  if (!nokill)
    std::cout << "Trajectory end condition: exponential npts (lambda="
	      << kill_rate << ")" << std::endl;
  else
    std::cout << "Trajectory end condition: nokill (tmax="
	      << tmax << ")" << std::endl;
  std::cout << "t0 = " << t0 << std::endl;
  std::cout << "D = " << D <<std::endl;

  std::vector<TrajectoryEndCondition<2>*> conds;
  if (!nokill)
    conds.push_back(new NumberPointsExpEndCondition<2>(kill_pdist, mt));
  else
    conds.push_back(new TimeEndCondition<2>(tmax));
  //Trajs longer than the number of frames will not be displayed anyway
  //conds.push_back(new NumberPointsEndCondition<2>(Nframes+1));

  std::cout << "Runover: " << std::to_string(runover) << std::endl;
  if (!runover)
    conds.push_back(new TimeEndCondition<2>(t0 + (Nframes+1)*DT));
  conds.push_back(new EscapeEndCondition<2>(*poly));
  TrajectoryEndCondition<2>* traj_end_cond = new CompoundEndCondition<2>(conds);

  TrajectoryEndConditionFactory<2> traj_end_cond_facto(*traj_end_cond);

  TrajectoryRecorder<2>* traj_rec =
    new SubsampleTrajectoryRecorder<2>(0.0, DT, (unsigned) (DT /dt));
  TrajectoryRecorderFactory<2> traj_rec_facto(*traj_rec);


  Collider<2>* collider =
    new QuadTreeCollider(*dynamic_cast<MultiplePolygon*>(poly), qt);
  std::cout << "Collider: "; collider->who_am_I(std::cout);

  //BufferLogger* log = new BufferLogger();
  std::ofstream log_ofs(outdir + "/errors");
  Logger* log = new DirectFileLogger(log_ofs);

  std::vector<double> motions_ps;
  TrajectoryGeneratorFactory<2>
    traj_gen_facto(*start_gen, motions, motions_ps, traj_end_cond_facto,
		   traj_rec_facto, *collider, nullptr, log, mt);

  unsigned mol_per_frame = (unsigned) ceil(prod_rate * DT);
  std::cout << "Molecules per frame: " << mol_per_frame << std::endl;

  std::ofstream f;
  f.open(outdir + "/params.csv");
  if (!f.is_open())
    std::cerr << "ERROR: Could not open output file: " << outdir + "/params.csv" << std::endl;
  else
  {
    f << "runover, " << std::to_string(runover) << std::endl;
    f << "t0, " << std::to_string(t0) << std::endl;
    f << "nokill, " << std::to_string(nokill) << std::endl;
    f << "tmax, " << std::to_string(tmax) << std::endl;
    f << "seed, " << std::to_string(seed) << std::endl;
    f << "Nframes," << std::to_string(Nframes) << std::endl;
    f << "pxsize, " << std::to_string(pxsize) << std::endl;
    f << "dt, " << std::to_string(dt) << std::endl;
    f << "DT, " << std::to_string(DT) << std::endl;
    f << "D, " << std::to_string(D) << std::endl;

    f << "prod_rate, " << std::to_string(prod_rate) << std::endl;
    f << "Mol_per_frame, " << std::to_string(mol_per_frame) << std::endl;
    f << "kill_rate, " << std::to_string(kill_rate) << std::endl;
    f << "fov_w, " << std::to_string(fov_size[0]) << std::endl;
    f << "fov_h, " << std::to_string(fov_size[1]) << std::endl;
    f << "pa_loc_x, " << std::to_string(pa_loc[0]) << std::endl;
    f << "pa_loc_y, " << std::to_string(pa_loc[1]) << std::endl;
    f << "pa_radius, " << std::to_string(pa_radius) << std::endl;
    f << "poly_path, " << poly_path << std::endl;
    f << "outdir, " << outdir << std::endl;
    f.close();
  }

  TrajectoryEnsemble<2> trajs;
  TrajectoryGenerator<2>* tg = nullptr;
  std::cout << "Running simulation" << std::endl;
  for (unsigned k = 0; k < Nframes; ++k)
  {
    for (unsigned l = 0; l < mol_per_frame; ++l)
    {
      std::cout << k << " " << l << std::endl;
      tg = traj_gen_facto.get(t0 + k * DT);
      trajs.push_back(tg->generate());
      delete tg;
    }
  }
  for (Trajectory<2>& tr: trajs)
    shift_coords(tr, Point<2>({-100.0, -100.0}));

  save_trajectories_csv<2>(outdir + "/trajs.csv", trajs);

  std::cout << "Generating images" << std::endl;
  generate_tif_stack(trajs, fov_size[1], fov_size[0], Nframes, pxsize,
		     DT, outdir + "/simulated_raw_data.tif");

  BufferLogger* bl = dynamic_cast<BufferLogger*> (log);
  if (bl != NULL)
    bl->save_to_file(outdir + "/errors");
  if (dynamic_cast<DirectFileLogger*> (log) != NULL)
    log_ofs.close();

  delete traj_end_cond;
  delete log;
  delete poly;
  delete qt;
  delete collider;
  delete start_gen;
  delete traj_rec;
  delete motions[0];

  std::cout << "DONE" << std::endl;
}
