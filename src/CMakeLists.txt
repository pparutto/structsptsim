set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -Wno-deprecated-copy")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(libsim brownian_motion.cc)
