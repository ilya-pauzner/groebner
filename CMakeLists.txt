cmake_minimum_required(VERSION 3.9)
project(Groebner)

set(CMAKE_CXX_STANDARD 17)

include_directories(F:/boost_1_66_0)
add_executable(Groebner main.cpp monomial.cpp test.cpp monomial_order.cpp helpers.cpp)