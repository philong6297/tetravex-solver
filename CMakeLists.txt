cmake_minimum_required(VERSION 3.0.0)
project(tetravex-solver VERSION 0.1.0)

set(LONGLP_PROJECT_DIR "${PROJECT_SOURCE_DIR}")
set(LONGLP_PROJECT_SRC_DIR "${LONGLP_PROJECT_DIR}/src")
set(LONGLP_PROJECT_EXTERNAL_DIR "${LONGLP_PROJECT_DIR}/external")
set(LONGLP_PROJECT_DATA_DIR "${LONGLP_PROJECT_DIR}/data")
set(LONGLP_PROJECT_OUTPUT_DIR "${PROJECT_BINARY_DIR}")

add_subdirectory(tools/cmake)
add_subdirectory(data)
longlp_make_exec(
  TARGET_NAME ${PROJECT_NAME}
  USE_DEFAULT
  SOURCES
  #[[]] PRIVATE
  #[[]]   src/main.cpp
  #[[]]   src/engine/game.cpp
  #[[]]   src/engine/game.h
  #[[]]   src/engine/game_generator.cpp
  #[[]]   src/engine/game_generator.h
  #[[]]   src/solver/solver.cpp
  #[[]]   src/solver/solver.h
  #[[]]   src/solver/thread_pool.cpp
  #[[]]   src/solver/thread_pool.h
  LINK_LIBRARIES
  INCLUDE_DIRECTORIES
  #[[]] PRIVATE
  #[[]]   src
  DEPENDENCIES
  #[[]] copy_data
)
