#ifndef TRAJECTORY_START_GENERATOR_HH_
#define TRAJECTORY_START_GENERATOR_HH_

# include <random>

# include "point.hh"
# include "shape.hh"

template <size_t N>
class TrajectoryStartGenerator
{
public:
  virtual ~TrajectoryStartGenerator();
  virtual Point<N> generate() = 0;
};

template <size_t N>
class FixedPointTrajectoryStartGenerator: public TrajectoryStartGenerator<N>
{
public:
  FixedPointTrajectoryStartGenerator(const Point<N>& pt);
  virtual ~FixedPointTrajectoryStartGenerator();

  void update_start_point(const Point<N>& new_pt);

  virtual Point<N> generate() override;
protected:
  Point<N> pt_;
};

class RandomBoxTrajectoryStartGenerator: public TrajectoryStartGenerator<2>
{
public:
  RandomBoxTrajectoryStartGenerator(std::mt19937_64& ng, const Box& box);
  virtual ~RandomBoxTrajectoryStartGenerator();

  virtual Point<2> generate() override;
protected:
  std::mt19937_64& ng_;
  Box box_;
  std::uniform_real_distribution<double> randu_;
};

class RandomTrajectoryStartGenerator: public TrajectoryStartGenerator<2>
{
public:
  RandomTrajectoryStartGenerator(std::mt19937_64& ng, const Shape<2>& shape);
  virtual ~RandomTrajectoryStartGenerator();

  virtual Point<2> generate() override;
protected:
  std::mt19937_64& ng_;
  const Shape<2>& shape_;
  RandomBoxTrajectoryStartGenerator rnd_box_;
};

class RandomBoxInPolyTrajectoryStartGenerator:
  public TrajectoryStartGenerator<2>
{
public:
  RandomBoxInPolyTrajectoryStartGenerator(std::mt19937_64& ng,
					  const Shape<2>& poly,
					  const Box& box);
  virtual ~RandomBoxInPolyTrajectoryStartGenerator();

  virtual Point<2> generate() override;
protected:
  std::mt19937_64& ng_;
  const Shape<2>& poly_;
  RandomBoxTrajectoryStartGenerator rnd_box_;
};


class MultiplePolysRandomTrajectoryStartGenerator:
  public TrajectoryStartGenerator<2>
{
public:
  MultiplePolysRandomTrajectoryStartGenerator(std::mt19937_64& ng,
					      const MultiplePolygon& polys);
  virtual ~MultiplePolysRandomTrajectoryStartGenerator();

  virtual Point<2> generate() override;
protected:
  std::mt19937_64& ng_;
  std::vector<double> norm_cum_areas_;
  std::uniform_real_distribution<double> randu_;
  std::vector<RandomTrajectoryStartGenerator> poly_gens_;
};



#endif /// !TRAJECTORY_START_GENERATOR_HH
