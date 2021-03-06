cmake_minimum_required(VERSION 2.8)

project(main)

set(CMAKE_CXX_FLAGS "-g -std=c++11 -pthread")

add_library(core STATIC
  bitboard.cpp
  castling.cpp
  castlingtype.cpp
  color.cpp
  evaluation.cpp
  notation.cpp
  file.cpp
  move.cpp
  movegenerator.cpp
  movelist.cpp
  movetype.cpp
  perft.cpp
  piece.cpp
  piecetype.cpp
  position.cpp
  pulse.cpp
  rank.cpp
  search.cpp
  square.cpp
  value.cpp
)

add_executable(pulse main.cpp)
set_target_properties(pulse PROPERTIES OUTPUT_NAME "pulse-${pulse_VERSION}")

target_link_libraries(pulse core)

install(TARGETS pulse DESTINATION .)
