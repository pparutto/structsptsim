#ifndef MOTION_HH_
#define MOTION_HH_

# include <random>

# include "motion.hh"
# include "trajectory_start_generator.hh"
# include "trajectory_end_condition.hh"
# include "trajectory_recorder.hh"
# include "cum_distrib_function.hh"

template <size_t N>
class Motion
{
public:
  virtual ~Motion() = default;
  virtual Point<N> step_euler(const Point<N>& p) = 0;
  virtual bool subsample() const = 0;
};

template <size_t N>
class BrownianMotion: public Motion<N>
{
public:
  virtual ~BrownianMotion() = default;
  BrownianMotion(std::mt19937_64& ng, double D, double dt);

  virtual Point<N> step_euler(const Point<N>& p) override;
  virtual bool subsample() const override;

  double D() const { return this->D_; }
  double dt() const { return this->dt_; }
protected:
  std::mt19937_64& ng_;
  std::normal_distribution<> randn_;
  double D_;
  double dt_;
  double s2Ddt_;
};

class EmpiricalMotion: public Motion<2>
{
public:
  virtual ~EmpiricalMotion() = default;
  EmpiricalMotion(std::mt19937_64& ng, CumDistribFunction& ivel_cdf,
		  double dt);

  virtual Point<2> step_euler(const Point<2>& p) override;
  virtual bool subsample() const override;

  double dt() const { return this->dt_; }
protected:
  std::mt19937_64& ng_;
  std::uniform_real_distribution<> randu_;
  CumDistribFunction& ivel_cdf_;
  double dt_;
};

//From
// A Hidden Markov Model for Single Particle Tracks Quantifies Dynamic
// Interactions between LFA-1 and the Actin Cytoskeleton
// Raibatak Das, Christopher W. Cairo, Daniel Coombs
// https://doi.org/10.1371/journal.pcbi.1000556
class HMM2DMotion: public Motion<2>
{
public:
  virtual ~HMM2DMotion() = default;
  HMM2DMotion(std::mt19937_64& ng, const std::array<double,2>& Ds,
	      const std::array<double, 2>& rates, double dt);

  virtual Point<2> step_euler(const Point<2>& p) override;
  virtual bool subsample() const override;

  std::string to_str() const;

  double dt() const { return this->dt_; }
protected:
  std::mt19937_64& ng_;
  std::uniform_real_distribution<> randu_;
  double dt_;
  std::array<double,2> rates_;
  std::vector<BrownianMotion<2> > bms_;
  unsigned cur_state_;
  std::array<std::array<double,2>,2> transi_;
};

#endif /// !MOTION_HH
