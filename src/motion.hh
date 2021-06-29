#ifndef MOTION_HH_
#define MOTION_HH_

# include <random>

# include "motion.hh"
# include "trajectory_start_generator.hh"
# include "trajectory_end_condition.hh"
# include "trajectory_recorder.hh"
# include "cum_distrib_function.hh"


class Motion
{
public:
  virtual ~Motion() = default;
  virtual Point step_euler(const Point& p) = 0;
  virtual bool subsample() const = 0;
protected:
};

class BrownianMotion: public Motion
{
public:
  virtual ~BrownianMotion() = default;
  BrownianMotion(std::mt19937_64& ng, double D, double dt);

  virtual Point step_euler(const Point& p) override;
  virtual bool subsample() const override;

  double dt() const { return this->dt_; }
protected:
  std::mt19937_64& ng_;
  std::normal_distribution<> randn_;
  double D_;
  double dt_;
  double s2Ddt_;
};

class EmpiricalMotion: public Motion
{
public:
  virtual ~EmpiricalMotion() = default;
  EmpiricalMotion(std::mt19937_64& ng, const CumDistribFunction& ivel_cdf,
		  double dt);

  virtual Point step_euler(const Point& p) override;
  virtual bool subsample() const override;

  double dt() const { return this->dt_; }
protected:
  std::mt19937_64& ng_;
  std::uniform_real_distribution<> randu_;
  const CumDistribFunction& ivel_cdf_;
  double dt_;
};

#endif /// !MOTION_HH
