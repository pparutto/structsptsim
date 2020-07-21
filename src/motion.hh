#ifndef MOTION_HH_
#define MOTION_HH_

# include <random>

# include "motion.hh"
# include "trajectory_start_generator.hh"
# include "trajectory_end_condition.hh"
# include "trajectory_recorder.hh"


class Motion
{
public:
  virtual Point step_euler(const Point& p) = 0;
protected:
};

class BrownianMotion: public Motion
{
public:
  BrownianMotion(std::mt19937_64& ng, double D, double dt);

  virtual Point step_euler(const Point& p) override;
protected:
  std::mt19937_64& ng_;
  std::normal_distribution<> randn_;
  double D_;
  double dt_;
  double s2Ddt_;
};


#endif /// !MOTION_HH
