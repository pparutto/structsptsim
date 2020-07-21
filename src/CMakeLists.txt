set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -Wno-deprecated-copy")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(libsim
  io.cc
  motion.cc
  shape.cc
  simulation.cc
  simulation_end_condition.cc
  trajectory.cc
  trajectory_end_condition.cc
  trajectory_generator.cc
  trajectory_recorder.cc
  trajectory_start_generator.cc)
