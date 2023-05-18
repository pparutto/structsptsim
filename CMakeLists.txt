cmake_minimum_required (VERSION 2.6)
project (ersptsim CXX)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -Wno-deprecated-copy")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

include_directories ("${PROJECT_SOURCE_DIR}/external/tclap-1.2.1")
include_directories ("${PROJECT_SOURCE_DIR}/src")
add_subdirectory (src)

add_executable(gensim bin/gensim.cc)
add_executable(genimgs bin/genimgs.cc)
add_executable(gensim3D bin/gensim3D.cc)
add_executable(genfree2Dsim bin/genfree2Dsim.cc)
add_executable(brown2D bin/brown2D.cc)
add_executable(genboxcollide test/genboxcollide.cc)
add_executable(gentrajs test/gentrajs.cc)
add_executable(geometry test/geometry.cc)

target_link_libraries (gensim libsim -ldl -ltiff)
target_link_libraries (genimgs libsim -ldl -ltiff)
target_link_libraries (gensim3D libsim -ldl -ltiff)
target_link_libraries (genfree2Dsim libsim -ldl -ltiff)
target_link_libraries (brown2D libsim -ldl -ltiff)
target_link_libraries (genboxcollide libsim -ldl)
target_link_libraries (gentrajs libsim -ldl)
target_link_libraries (geometry libsim -ldl)
