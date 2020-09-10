cmake_minimum_required (VERSION 2.6)
project (ersptsim CXX)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -Wno-deprecated-copy")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

include_directories ("${PROJECT_SOURCE_DIR}/src")
add_subdirectory (src)

add_executable(gensim bin/gensim.cc)
add_executable(genboxcollide test/genboxcollide.cc)
add_executable(geometry test/geometry.cc)

target_link_libraries (gensim libsim -ldl)
target_link_libraries (genboxcollide libsim -ldl)
target_link_libraries (geometry libsim -ldl)
