#include "gensim_io.hh"

#include <cassert>

std::array<unsigned, 2>
ArgumentParserOptions::read_fov_size(const std::string& str)
{
  std::istringstream ss(str);
  std::string tmp;
  std::getline(ss, tmp, ',');
  unsigned width = std::stoi(tmp);
  std::getline(ss, tmp, ',');
  unsigned height = std::stoi(tmp);
  return {width, height};
}

Point<2>
ArgumentParserOptions::read_point(const std::string& str)
{
  std::istringstream ss(str);
  std::string tmp;
  std::getline(ss, tmp, ',');
  double x = std::stod(tmp);
  std::getline(ss, tmp, ',');
  double y = std::stod(tmp);
  return {x, y};
}

std::pair<std::array<double,2>, std::array<double,2> >
ArgumentParserOptions::read_HMM(const std::string& str)
{
  std::istringstream ss(str);
  std::string tmp;

  std::array<double,2> Ds;
  std::getline(ss, tmp, ',');
  Ds[0] = std::stod(tmp);
  std::getline(ss, tmp, ',');
  Ds[1] = std::stod(tmp);

  std::array<double,2> rates;
  std::getline(ss, tmp, ',');
  rates[0] = std::stod(tmp);
  std::getline(ss, tmp, ',');
  rates[1] = std::stod(tmp);

  return std::make_pair(Ds, rates);
}

ArgumentParserOptions::ArgumentParserOptions()
  : noimg_arg("", "noimg", "Do not generate output img", false)
  , logncoll_arg("", "logncoll", "log number of collisions", false)
  , seed_arg("", "seed", "", false, 0, "Seed for random number generator")
  , export_poly_txt_arg("", "export-poly-txt", "Export polygon geometry as text file", false)
  , export_poly_mat_arg("", "export-poly-mat", "Export polygon geometry as malab file", false)
  , pxsize_arg("", "pxsize", "", false, NAN, "Pixel size for polygon (µm)")
  , start_point_arg("", "start-point", "", false, "", "x,y Initial trajectories point")
  , start_box_arg("", "start-box", "", false, "", "Box to start trajectories (format: llx,lly;trx,try)")
  , stop_box_arg("", "stop-box", "", false, "", "Box to end trajectories (format: llx,lly;trx,try)")
  , exp_l_arg("", "npts-exp", "", false, NAN, "Exponential λ traj. length")
  , empirical_tr_arg("", "empirical-trajs", "", false, "", "Empirical trajectories")
  , Npts_arg("", "npts-fixed", "", false, 0, "Number of points")
  , Ntrajs_arg("", "ntrajs", "", false, 0, "Number of trajectories")
  , Nframes_arg("", "nframes", "", false, 0, "Number of frames")
  , fov_size_arg("", "fov-size", "", false, "", "Width,Height of field of view in pixels")
  , sim_reg_arg("", "sim-reg", "", false, "", "simulation square region (minx, miny, maxx, maxy) in pixels")
  , spot_dens_arg("", "spot-density", "", false, 0.0, "Spots / μm^2 / frame")
  , D_arg("", "motion-D", "", false, NAN, "Diffusion coefficient (μm²/s)")
  , HMM_arg("", "motion-HMM", "", false, "", "HMM motion: D1,...,DN;t12,t13,...t1N,t21,t23,...,t2,N,...,tN1,...,tN(N-1)")
  , ivel_cdf_arg("", "motion-cdf", "", false, "", "CSV file containing inst. vel. cdf")
  , poly_arg("", "poly", "", false, "", "Path to polygon file")
  , dt_arg("", "dt", "", false, NAN, "Simulation timestep (s)")
  , DT_arg("DT", "acquisition timestep (s)", true, NAN, "DT")
  , outdir_arg("outdir", "output directory", true, "", "outdir")
{
}

void
ArgumentParserOptions::add_arguments(TCLAP::CmdLine& cmd)
{
  cmd.add(this->noimg_arg);
  cmd.add(this->logncoll_arg);
  cmd.add(this->seed_arg);
  cmd.add(this->export_poly_txt_arg);
  cmd.add(this->export_poly_mat_arg);
  cmd.add(this->pxsize_arg);
  cmd.add(this->start_point_arg);
  cmd.add(this->start_box_arg);
  cmd.add(this->stop_box_arg);
  cmd.add(this->exp_l_arg);
  cmd.add(this->empirical_tr_arg);
  cmd.add(this->Npts_arg);
  cmd.add(this->Ntrajs_arg);
  cmd.add(this->Nframes_arg);
  cmd.add(this->fov_size_arg);
  cmd.add(this->sim_reg_arg);
  cmd.add(this->spot_dens_arg);
  cmd.add(this->D_arg);
  cmd.add(this->HMM_arg);
  cmd.add(this->ivel_cdf_arg);
  cmd.add(this->poly_arg);
  cmd.add(this->dt_arg);
  cmd.add(this->DT_arg);
  cmd.add(this->outdir_arg);
}

void
ArgumentParserOptions::verify_command_line() const
{
  if (!(this->Ntrajs_arg.isSet() ^
	this->Nframes_arg.isSet() ^
	this->empirical_tr_arg.isSet()))
  {
    std::cerr << "ERROR: Either ntrajs, nframes or empirical-trajs must be set"
	      << std::endl;
    assert(false);
  }

  if (this->Nframes_arg.isSet() &&
      !(this->spot_dens_arg.isSet() && this->fov_size_arg.isSet()))
  {
    std::cerr << "ERROR: spot-density and fov-size must be set when"
	      << " selecting nframes"
	      << std::endl;
    assert(false);
  }

  if (!this->fov_size_arg.isSet() && !this->start_point_arg.isSet())
  {
    std::cerr << "ERROR: must specify either start-point or fov-size"
	      << std::endl;
    assert(false);
  }

  if (!(this->Npts_arg.isSet() ^ this->exp_l_arg.isSet() ^
	this->empirical_tr_arg.isSet() ^ this->stop_box_arg.isSet()))
  {
    std::cerr << "ERROR: either npts-fixed, npts-exp, empirical-trajs"
	      << " or stop-box must be set"
	      << std::endl;
    assert(false);
  }

  if (!(this->D_arg.isSet() ^ this->ivel_cdf_arg.isSet() ^ this->HMM_arg.isSet()))
  {
    std::cerr << "ERROR: either motion-D, motion-cdf or motion-HMM must be set"
	      << std::endl;
    assert(false);
  }

  if (this->D_arg.isSet() && !this->dt_arg.isSet())
  {
    std::cerr << "ERROR: dt must be set when using motion-D" << std::endl;
    assert(false);
  }

  if (this->sim_reg_arg.isSet() && this->start_box_arg.isSet())
  {
    std::cerr << "ERROR: sim_reg already sets start_box either re-code or remove the later" << std::endl;
    assert(false);
  }
}

void
ArgumentParserOptions::fill_program_options(ProgramOptions& p_opts)
{
  if (this->noimg_arg.isSet())
    p_opts.noimg = true;

  if (this->logncoll_arg.isSet())
    p_opts.log_n_coll = true;

  if (this->seed_arg.isSet())
    p_opts.seed = seed_arg.getValue();
  else
  {
    std::random_device rd;
    p_opts.seed = rd();
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

  if (start_point_arg.isSet())
  {
    p_opts.use_start_point = true;
    p_opts.start_point = ArgumentParserOptions::read_point(start_point_arg.getValue());
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

  if (exp_l_arg.isSet())
  {
    p_opts.tr_len_type = EXP;
    p_opts.pdist =
      std::exponential_distribution<double>(1 / exp_l_arg.getValue());
  }

  if (Npts_arg.isSet())
  {
    p_opts.tr_len_type = FIXED;
    p_opts.Npts = Npts_arg.getValue();
  }

  if (Ntrajs_arg.isSet())
  {
    p_opts.tr_gen_type = TrajGenType::NTRAJS;
    p_opts.Ntrajs = Ntrajs_arg.getValue();
  }

  if (Nframes_arg.isSet())
  {
    p_opts.tr_gen_type = TrajGenType::NFRAMES;
    p_opts.Nframes = Nframes_arg.getValue();
    p_opts.spot_dens = spot_dens_arg.getValue();
  }

  if (fov_size_arg.isSet())
  {
    p_opts.use_fov = true;
    p_opts.fov_size = ArgumentParserOptions::read_fov_size(fov_size_arg.getValue());
  }

  if (sim_reg_arg.isSet())
  {
    p_opts.use_sim_reg = true;
    p_opts.sim_reg = parse_box(sim_reg_arg.getValue());
    p_opts.use_start_reg = true;
    p_opts.start_reg = parse_box(sim_reg_arg.getValue());
  }

  if (empirical_tr_arg.isSet())
  {
    p_opts.tr_gen_type = TrajGenType::EMPIRICAL;
    p_opts.empirical_trajs_file = empirical_tr_arg.getValue();
    p_opts.empirical_trajs = TrajectoryCharacs::from_file(empirical_tr_arg.getValue());
  }

  if (ivel_cdf_arg.isSet())
  {
    p_opts.motion_type = MotionType::DISTRIB;
    p_opts.D = NAN;
    p_opts.cdf_path = ivel_cdf_arg.getValue();
  }

  if (D_arg.isSet())
  {
    p_opts.motion_type = MotionType::BROWNIAN;
    p_opts.D = D_arg.getValue();
  }

  if (dt_arg.isSet())
    p_opts.dt = dt_arg.getValue();

  if (HMM_arg.isSet())
  {
    p_opts.motion_type = MotionType::HMM;
    std::pair<std::array<double,2>, std::array<double,2> > tmp =
      ArgumentParserOptions::read_HMM(HMM_arg.getValue());
    p_opts.Ds = tmp.first;
    p_opts.rates = tmp.second;
  }

  p_opts.DT = DT_arg.getValue();
  p_opts.t_ratio = (unsigned) round(p_opts.DT / p_opts.dt);

  p_opts.outdir = outdir_arg.getValue();
}

unsigned
ProgramOptions::num_particles() const
{
  if (this->use_sim_reg)
    return (unsigned) (this->sim_reg.dims()[0] * this->sim_reg.dims()[1] * this->spot_dens);
  else
  {
    return (unsigned) (this->fov_size[0] * this->fov_size[1] * pow(this->pxsize, 2) * this->spot_dens);
  }
}

void
ProgramOptions::save_csv(const std::string& fname) const
{
  std::ofstream f;
  f.open(fname);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fname << std::endl;
    return;
  }

  f << "seed, " << std::to_string(this->seed) << std::endl;

  if (this->motion_type == MotionType::BROWNIAN)
  {
    f << "MotionType, Brownian" << std::endl;
    f << "D, " << std::to_string(this->D) << std::endl;
    f << "dt, " << std::to_string(this->dt) << std::endl;
  }
  else if (this->motion_type == MotionType::DISTRIB)
  {
    f << "MotionType, Distribution" << std::endl;
    f << "DistribFile, " << this->cdf_path << std::endl;
  }
  else if (this->motion_type == MotionType::HMM)
  {
    f << "MotionType, HMM" << std::endl;
    f << "D1, " << std::to_string(this->Ds[0]) << std::endl;
    f << "D2, " << std::to_string(this->Ds[1]) << std::endl;
    f << "k_on, " << std::to_string(this->rates[0]) << std::endl;
    f << "k_off, " << std::to_string(this->rates[1]) << std::endl;
    f << "dt, " << std::to_string(this->dt) << std::endl;
  }

  if (this->tr_gen_type == TrajGenType::NTRAJS)
  {
    f << "TrajGenType, Trajs" << std::endl;
    f << "Ntrajs, " << std::to_string(this->Ntrajs) << std::endl;
  }
  else if (this->tr_gen_type == TrajGenType::NFRAMES)
  {
    f << "TrajGenType, Frames" << std::endl;
    f << "Nframes, " << std::to_string(this->Nframes) << std::endl;
    f << "density, " << std::to_string(this->spot_dens) << std::endl;
    f << "num spts per frame, " << std::to_string(this->num_particles()) << std::endl;
    f << "FOVWidth, " << std::to_string(this->fov_size[0]) << std::endl;
    f << "FOVHeight, " << std::to_string(this->fov_size[1]) << std::endl;
  }
  else
  {
    f << "TrajGenType, Empirical" << std::endl;
    f << "EmpiricalTrajsFile, " << this->empirical_trajs_file << std::endl;
  }

  if (this->use_sim_reg)
    f << "SimReg Pixels," << this->sim_reg;

  if (this->tr_len_type == TrajLenType::EXP)
  {
    f << "TrajLengthType, Exponential" << std::endl;
    f << "NptsLambda, " + std::to_string(this->pdist.lambda()) << std::endl;
  }
  else
  {
    f << "TrajLengthType, Fixed" << std::endl;
    f << "Npts, " << std::to_string(this->Npts) << std::endl;
  }

  if (this->use_pxsize)
    f << "pixelSize, " << this->pxsize << std::endl;

  f << "DT, " << std::to_string(this->DT) << std::endl;
  f << "DT/dt, " << this->t_ratio << std::endl;
  
  if (this->use_poly)
  {
    f << "ConfinementType, Polygon" << std::endl;
    f << "polygonFile, " << this->poly_path << std::endl;
  }
  else
    f << "ConfinementType, FreeSpace" << std::endl;

  if (this->use_start_point)
    f << "StartPoint, " << this->start_point[0] << ";" << this->start_point[1]
      << std::endl;


  f << "Outdir, " << this->outdir << std::endl;

  f.close();
}
