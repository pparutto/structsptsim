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
    TCLAP::CmdLine cmd("./gensim3D", ' ', "1");
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

  p_opts.save_csv(p_opts.outdir + "/params.csv");

  std::cout << std::setprecision(15);

  std::random_device rd;
  std::mt19937_64 mt(rd());
}
