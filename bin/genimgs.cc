#include <iostream>

#include <tiffio.h>

#include "tclap/CmdLine.h"

#include "trajectory.hh"
#include "io.hh"
#include "raw_image_simulator.hh"


//DUPLICATED from gensim, find a way to put that in common without having to add
//libtiff as a dependency of the simulation library
void generate_tif_stack(const TrajectoryEnsemble<2>& trajs, double I0,
			double sigma, unsigned width, unsigned height,
			unsigned length, double pxsize, double DT,
			const std::string& outfile)
{
  unsigned short*** imgs =
    raw_image_simulator(length, width, height, pxsize, DT, I0, sigma, trajs);
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
    for (unsigned i = 0; i < width; ++i)
      delete[] imgs[k][i];
    delete[] imgs[k];
  }
  delete[] imgs;
}


int main(int argc, char** argv)
{

  double I0 = NAN;
  double sigma = NAN;

  double fovw = NAN;
  double fovh = NAN;
  double pxsize = NAN;

  unsigned nframes = 0;
  double DT = NAN;

  std::string trajs_f = "";
  std::string outdir = "";

  try
  {
    TCLAP::CmdLine cmd("./genimgs", ' ', "1");

    TCLAP::UnlabeledValueArg<double> I0_arg
      ("I0", "Gaussian peak intensity", true, NAN, "I0");
    cmd.add(I0_arg);

    TCLAP::UnlabeledValueArg<double> sigma_arg
      ("sigma", "Gaussian width (µm)", true, NAN, "sigma");
    cmd.add(sigma_arg);

    TCLAP::UnlabeledValueArg<unsigned> fovw_arg
      ("fovw", "Width of field of view (pxs)", true, 0, "FOV width");
    cmd.add(fovw_arg);

    TCLAP::UnlabeledValueArg<unsigned> fovh_arg
      ("fovh", "Height of field of view (pxs)", true, 0, "FOV height");
    cmd.add(fovh_arg);

    TCLAP::UnlabeledValueArg<double> pxsize_arg
      ("pxsize", "Pixel size (µm)", true, NAN, "pixel size");
    cmd.add(pxsize_arg);

    TCLAP::UnlabeledValueArg<unsigned> nframes_arg
      ("nframes", "Number of frames", true, 0, "N frames");
    cmd.add(nframes_arg);

    TCLAP::UnlabeledValueArg<double> DT_arg
      ("DT", "Acquisition time (s)", true, NAN, "Acquisition time");
    cmd.add(DT_arg);

    TCLAP::UnlabeledValueArg<std::string> trajs_arg
      ("trajs", "Path to csv trajs file", true, "", "trajectory file");
    cmd.add(trajs_arg);

    TCLAP::UnlabeledValueArg<std::string> outdir_arg
      ("outdir", "Output directory", true, "", "outdir");
    cmd.add(outdir_arg);

    cmd.parse(argc, argv);

    I0 = I0_arg.getValue();
    sigma = sigma_arg.getValue();
    fovw = fovw_arg.getValue();
    fovh = fovh_arg.getValue();
    pxsize = pxsize_arg.getValue();
    nframes = nframes_arg.getValue();
    DT = DT_arg.getValue();
    trajs_f = trajs_arg.getValue();
    outdir = outdir_arg.getValue();
  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << "ERROR: " << e.error() << " for arg "
	      << e.argId() << std::endl;
  }

  TrajectoryEnsemble<2> trajs = read_trajectories_csv<2>(trajs_f);
  std::cout << "Loaded " << trajs.size() << " trajectories ("
	    << count_pts<2>(trajs) << " points)" << std::endl;

  std::cout << "Generating images" << std::endl;

  std::ofstream f;
  f.open(outdir + "/genimgs_params.csv");
  f << "I0, " << I0 << std::endl;
  f << "sigma (µm), " << sigma << std::endl;
  f << "FOVw (µm), " << fovw << std::endl;
  f << "FOVh (µm), " << fovh << std::endl;
  f << "pxsize (µm), " << pxsize << std::endl;
  f << "Nframes, " << nframes << std::endl;
  f << "DT (s), " << DT << std::endl;
  f << "Trajs file, " << trajs_f << std::endl;
  f << "outdir, " << outdir << std::endl;
  f.close();

  generate_tif_stack(trajs, I0, sigma, fovw, fovh, nframes, pxsize, DT,
		     outdir + "/simulated_raw_data.tif");

  std::cout << "DONE" << std::endl;
}
