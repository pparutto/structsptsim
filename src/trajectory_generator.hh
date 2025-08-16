#ifndef TRAJECTORY_GENERATOR_HH_
#define TRAJECTORY_GENERATOR_HH_

# include <random>

# include "motion.hh"
# include "trajectory_start_generator.hh"
# include "trajectory_end_condition.hh"
# include "trajectory_recorder.hh"
# include "collider.hh"
# include "logger.hh"

template <size_t N>
class TrajectoryGenerationException: public std::exception
{
public:
  TrajectoryGenerationException (const CollisionException<N>& base_except,
				 std::string what);
  ~TrajectoryGenerationException() = default;

  virtual const char* what() const noexcept override;

  const CollisionException<N>& base_except() const { return this->base_except_; }
 protected:
  CollisionException<N> base_except_;
  std::string what_;
};


template <size_t N>
class TrajectoryGenerator
{
public:
  TrajectoryGenerator(TrajectoryStartGenerator<N>& traj_start,
		      Motion<N>& motion_model,
		      TrajectoryEndCondition<N>* traj_end,
		      TrajectoryRecorder<N>* traj_rec,
		      Collider<N>& collider,
		      const Shape<N>* sim_reg,
		      Logger* log);
  ~TrajectoryGenerator();

  void init();
  void generate_step();
  bool finished();
  double cur_t();
  Trajectory<N> get();

  Trajectory<N> generate();
  bool has_generated() const;

  bool subsample() const;

protected:
  TrajectoryStartGenerator<N>& traj_start_;
  Motion<N>& motion_model_;
  TrajectoryEndCondition<N>* traj_end_;
  TrajectoryRecorder<N>* traj_rec_;
  Collider<N>& collider_;
  const Shape<N>* sim_reg_;
  bool done_;
  bool has_gen_;
  Logger* log_;
};

template <size_t N>
class TrajectoryGeneratorFactory
{
public:
  TrajectoryGeneratorFactory(TrajectoryStartGenerator<N>& traj_start,
			     const std::vector<Motion<N>*>& motion_models,
			     const std::vector<double>& motion_ps,
			     TrajectoryEndConditionFactory<N>& traj_end_facto,
			     TrajectoryRecorderFactory<N>& traj_rec_facto,
			     Collider<N>& collider,
			     const Shape<N>* sim_reg,
			     Logger* log,
			     std::mt19937_64& ng_);

  TrajectoryGenerator<N>* get(double t0);

  TrajectoryStartGenerator<N>& traj_start();
  TrajectoryEndConditionFactory<N>& traj_end_facto();

protected:
  TrajectoryStartGenerator<N>& traj_start_;
  std::vector<Motion<N>*> motion_models_;
  std::vector<double> motion_ps_;
  TrajectoryEndConditionFactory<N>& traj_end_facto_;
  TrajectoryRecorderFactory<N>& traj_rec_facto_;
  Collider<N>& collider_;
  const Shape<N>* sim_reg_;
  Logger* log_;
  std::mt19937_64& ng_;
  std::uniform_real_distribution<> randu_;
};


#endif /// !TRAJECTORY_GENERATOR_HH
