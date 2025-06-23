#ifndef GENSIM_IO_HH_
# define GENSIM_IO_HH_

#include "tclap/CmdLine.h"

# include "io.hh"

enum MotionType {DISTRIB, BROWNIAN, HMM};
enum TrajLenType {FIXED, EXP, REG};
enum TrajGenType {NTRAJS, NFRAMES, EMPIRICAL};

struct ProgramOptions
{
  bool noimg = false;

  unsigned seed = 0;

  bool use_poly = false;
  std::string poly_path;

  bool export_poly_txt = false;
  bool export_poly_mat = false;

  bool use_pxsize = false;
  double pxsize = NAN;

  TrajLenType tr_len_type;
  unsigned Npts = 0;
  std::exponential_distribution<double> pdist;

  double dt = NAN;
  double DT = NAN;
  unsigned t_ratio = 0;

  TrajGenType tr_gen_type = TrajGenType::NTRAJS;

  unsigned Ntrajs = 0;
  unsigned Nframes = 0;
  double spot_dens = NAN;
  std::string empirical_trajs_file;
  TrajectoryCharacsMap empirical_trajs;

  bool use_fov = false;
  std::array<unsigned, 2> fov_size; //{width, height}

  bool use_sim_reg = false;
  Box<2> sim_reg; //{minx, maxx, miny, maxy}

  MotionType motion_type = MotionType::BROWNIAN;
  double D = NAN;
  std::string cdf_path;

  std::array<double,2> Ds;
  std::array<double,2> rates;
  
  bool use_start_reg = false;
  Box<2> start_reg;
  Box<2> stop_reg;

  bool use_start_point = false;
  Point<2> start_point;

  std::string outdir;

  unsigned num_particles() const;
  void save_csv(const std::string& fname) const;
};


struct ArgumentParserOptions
{
  TCLAP::SwitchArg noimg_arg;
  TCLAP::ValueArg<unsigned> seed_arg;
  TCLAP::SwitchArg export_poly_txt_arg;
  TCLAP::SwitchArg export_poly_mat_arg;
  TCLAP::ValueArg<double> pxsize_arg;
  TCLAP::ValueArg<std::string> start_point_arg;
  TCLAP::ValueArg<std::string> start_box_arg;
  TCLAP::ValueArg<std::string> stop_box_arg;
  TCLAP::ValueArg<double> exp_l_arg;
  TCLAP::ValueArg<std::string> empirical_tr_arg;
  TCLAP::ValueArg<unsigned> Npts_arg;
  TCLAP::ValueArg<unsigned> Ntrajs_arg;
  TCLAP::ValueArg<unsigned> Nframes_arg;
  TCLAP::ValueArg<std::string> fov_size_arg;
  TCLAP::ValueArg<std::string> sim_reg_arg;
  TCLAP::ValueArg<double> spot_dens_arg;
  TCLAP::ValueArg<double> D_arg;
  TCLAP::ValueArg<std::string> HMM_arg;
  TCLAP::ValueArg<std::string> ivel_cdf_arg;
  TCLAP::ValueArg<std::string> poly_arg;
  TCLAP::ValueArg<double> dt_arg;
  TCLAP::UnlabeledValueArg<double> DT_arg;
  TCLAP::UnlabeledValueArg<std::string> outdir_arg;

  static std::array<unsigned, 2> read_fov_size(const std::string& str);
  static Point<2> read_point(const std::string& str);
  static std::pair<std::array<double,2>, std::array<double,2> >
  read_HMM(const std::string& str);
  
  ArgumentParserOptions();
  void add_arguments(TCLAP::CmdLine& cmd);
  void verify_command_line() const;
  void fill_program_options(ProgramOptions& p_opts);
};


#endif /// !GENSIM_IO_HH
