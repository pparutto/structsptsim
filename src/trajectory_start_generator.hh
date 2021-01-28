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

class FixedPointTrajectoryStartGenerator: public TrajectoryStartGenerator
{
public:
  FixedPointTrajectoryStartGenerator(const Point& pt);

  virtual Point generate() override;
protected:
  const Point& pt_;
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

class RandomTrajectoryStartGenerator: public TrajectoryStartGenerator
{
public:
  RandomTrajectoryStartGenerator(std::mt19937_64& ng, const Shape& shape);

  virtual Point generate() override;
protected:
  std::mt19937_64& ng_;
  const Shape& shape_;
  RandomBoxTrajectoryStartGenerator rnd_box_;
};

class RandomPolygonTrajectoryStartGenerator: public TrajectoryStartGenerator
{
public:
  RandomPolygonTrajectoryStartGenerator(std::mt19937_64& ng,
					const CompoundPolygon& poly);

  virtual Point generate() override;
protected:
  std::mt19937_64& ng_;
  const CompoundPolygon& poly_;
  RandomBoxTrajectoryStartGenerator rnd_box_;
};


class MultiplePolysRandomTrajectoryStartGenerator:
  public TrajectoryStartGenerator
{
public:
  MultiplePolysRandomTrajectoryStartGenerator(std::mt19937_64& ng,
					      const std::vector<CompoundPolygon>& polys);

  virtual Point generate() override;
protected:
  std::mt19937_64& ng_;
  std::vector<double> norm_cum_areas_;
  std::uniform_real_distribution<double> randu_;
  std::vector<RandomPolygonTrajectoryStartGenerator> poly_gens_;
};



#endif /// !TRAJECTORY_START_GENERATOR_HH
