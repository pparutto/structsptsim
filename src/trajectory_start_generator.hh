#ifndef TRAJECTORY_START_GENERATOR_HH_
#define TRAJECTORY_START_GENERATOR_HH_

# include <random>

# include "point.hh"
# include "shape.hh"

class TrajectoryStartGenerator
{
public:
  virtual Point generate() = 0;
};

class RandomBoxTrajectoryStartGenerator: public TrajectoryStartGenerator
{
public:
  RandomBoxTrajectoryStartGenerator(std::mt19937_64& ng, const Box& box);

  virtual Point generate() override;

protected:
  std::mt19937_64& ng_;
  Box box_;
  std::uniform_real_distribution<double> randu_;
};

#endif /// !TRAJECTORY_START_GENERATOR_HH
