cmake_minimum_required(VERSION 3.5.1)
project(SHA_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(attack ../source/main.cpp ../source/sha-256.h ../source/global_const.h ../source/propagate.h ../source/diff_chars.h ../source/carry_graph.h)