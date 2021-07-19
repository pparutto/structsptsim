#ifndef TRAJECTORY_START_GENERATOR_HH_
#define TRAJECTORY_START_GENERATOR_HH_

# include <random>

# include "point.hh"
# include "shape.hh"

class TrajectoryStartGenerator
{
public:
  virtual ~TrajectoryStartGenerator();
  virtual Point generate() = 0;
};

class FixedPointTrajectoryStartGenerator: public TrajectoryStartGenerator
{
public:
  FixedPointTrajectoryStartGenerator(const Point& pt);
  virtual ~FixedPointTrajectoryStartGenerator();

  void update_start_point(const Point& new_pt);

  virtual Point generate() override;
protected:
  Point pt_;
};

class RandomBoxTrajectoryStartGenerator: public TrajectoryStartGenerator
{
public:
  RandomBoxTrajectoryStartGenerator(std::mt19937_64& ng, const Box& box);
  virtual ~RandomBoxTrajectoryStartGenerator();

  virtual Point generate() override;
protected:
  std::mt19937_64& ng_;
  Box box_;
  std::uniform_real_distribution<double> randu_;
};

class RandomTrajectoryStartGenerator: public TrajectoryStartGenerator
{
public:
  RandomTrajectoryStartGenerator(std::mt19937_64& ng, const Shape& shape);
  virtual ~RandomTrajectoryStartGenerator();

  virtual Point generate() override;
protected:
  std::mt19937_64& ng_;
  const Shape& shape_;
  RandomBoxTrajectoryStartGenerator rnd_box_;
};

class MultiplePolysRandomTrajectoryStartGenerator:
  public TrajectoryStartGenerator
{
public:
  MultiplePolysRandomTrajectoryStartGenerator(std::mt19937_64& ng,
					      const MultiplePolygon& polys);
  virtual ~MultiplePolysRandomTrajectoryStartGenerator();

  virtual Point generate() override;
protected:
  std::mt19937_64& ng_;
  std::vector<double> norm_cum_areas_;
  std::uniform_real_distribution<double> randu_;
  std::vector<RandomTrajectoryStartGenerator> poly_gens_;
};



#endif /// !TRAJECTORY_START_GENERATOR_HH
