cmake_minimum_required(VERSION 3.5.1)
project(SHA_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(SHA_1 main.cpp SHA-256.h SHA-256.cpp GLOBAL_CONSTANTS.h difference_propagation_tables.h difference_propagation_tables.cpp diff_chars.h diff_chars.cpp carry_graph.h carry_graph.cpp)