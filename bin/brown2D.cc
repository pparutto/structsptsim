#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <sys/stat.h>

#include <tiffio.h>

#include "tclap/CmdLine.h"

#include "simulation.hh"
#include "io.hh"

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
  std::random_device rd;
  unsigned seed = rd();

  double dt = NAN;
  double DT = NAN;
  double D = NAN;
  unsigned Ntrajs = 0;
  unsigned Npts = 0;

  std::string outdir;
  try
  {
    TCLAP::CmdLine cmd("./gensim", ' ', "1");

    TCLAP::ValueArg<unsigned> seed_arg
      ("", "seed", "", false, 0, "Seed for random number generator");
    cmd.add(seed_arg);

    // PIERRE: at the moment fixed size by default
    // TCLAP::ValueArg<unsigned> fixed_size
    //   ("", "fixed-size", "", false, 0, "Trajectory size (number of points)");
    // cmd.add(seed_arg);

    // PIERRE: not handled at the moment
    // TCLAP::UnlabeledValueArg<double> exp_arg
    //   ("λ", "Exponential λ traj. length", true, NAN, "ExpLambda");
    // cmd.add(exp_arg);

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
      ("Npts", "Trajectory size (num points)", true, 0, "Np");
    cmd.add(Npts_arg);


    TCLAP::UnlabeledValueArg<std::string> outdir_arg
      ("outdir", "output directory", true, "", "outdir");
    cmd.add(outdir_arg);

    cmd.parse(argc, argv);

    if (seed_arg.isSet())
      seed = seed_arg.getValue();

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

  std::cout << "RNG seed: " << seed << std::endl;
  std::mt19937_64 mt(seed);

  FixedPointTrajectoryStartGenerator<2> start_gen({0.0, 0.0});

  NumberPointsEndCondition<2> end_cond(Npts);
  TrajectoryEndConditionFactory<2> traj_end_cond_facto(end_cond);

  BrownianMotion<2> bm(mt, D, dt);

  unsigned t_ratio = (unsigned) (DT / dt);
  SubsampleTrajectoryRecorder<2> traj_rec(0.0, DT, t_ratio);
  TrajectoryRecorderFactory<2> traj_rec_facto(traj_rec);

  NumberTrajectoriesSimulationEndCondition<2> end_sim(Ntrajs);

  NoneCollider<2> collider;

  BufferLogger* log = new BufferLogger();

  TrajectoryGeneratorFactory<2>
    traj_gen_facto(start_gen, bm, traj_end_cond_facto, traj_rec_facto,
		   collider, log);

  SimulationTrajectory<2> sim(traj_gen_facto, end_sim);

  sim.run();

  save_trajectories_csv<2>(outdir + "/trajs.csv", sim.trajs());

  std::ofstream f;
  f.open(outdir + "/params.csv");
  f << "seed, " << seed << std::endl;
  f << "dt (s), " << dt << std::endl;
  f << "DT (s), " << DT << std::endl;
  f << "D (µm2/s), " << D << std::endl;
  f << "Ntrajs, " << Ntrajs << std::endl;
  f << "Npts, " << Npts << std::endl;
  f.close();

  log->save_to_file(outdir + "errors");

  delete log;
  std::cout << "DONE" << std::endl;
}
