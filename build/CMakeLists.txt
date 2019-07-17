cmake_minimum_required(VERSION 3.5.1)
project(SHA_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(attack ../search/diff_char/main.cpp ../source/sha-256.h ../source/global_const.h ../source/propagate.h ../search/diff_char/phase1/phase1.h ../source/carry_graph.h)