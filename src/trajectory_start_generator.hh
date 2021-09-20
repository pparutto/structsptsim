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

template <size_t N>
class RandomBoxTrajectoryStartGenerator: public TrajectoryStartGenerator<N>
{
public:
  RandomBoxTrajectoryStartGenerator(std::mt19937_64& ng, const Box<N>& box);
  virtual ~RandomBoxTrajectoryStartGenerator();

  virtual Point<N> generate() override;
protected:
  std::mt19937_64& ng_;
  Box<N> box_;
  std::uniform_real_distribution<double> randu_;
};

template <size_t N>
class RandomTrajectoryStartGenerator: public TrajectoryStartGenerator<2>
{
public:
  RandomTrajectoryStartGenerator(std::mt19937_64& ng, const Shape<N>& shape);
  virtual ~RandomTrajectoryStartGenerator();

  virtual Point<N> generate() override;
protected:
  std::mt19937_64& ng_;
  const Shape<N>& shape_;
  RandomBoxTrajectoryStartGenerator<N> rnd_box_;
};

class RandomBoxInPolyTrajectoryStartGenerator:
  public TrajectoryStartGenerator<2>
{
public:
  RandomBoxInPolyTrajectoryStartGenerator(std::mt19937_64& ng,
					  const Shape<2>& poly,
					  const Box<2>& box);
  virtual ~RandomBoxInPolyTrajectoryStartGenerator();

  virtual Point<2> generate() override;
protected:
  std::mt19937_64& ng_;
  const Shape<2>& poly_;
  RandomBoxTrajectoryStartGenerator<2> rnd_box_;
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
  std::vector<RandomTrajectoryStartGenerator<2> > poly_gens_;
};



#endif /// !TRAJECTORY_START_GENERATOR_HH
