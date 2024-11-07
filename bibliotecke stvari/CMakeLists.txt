cmake_minimum_required(VERSION 3.24)
project(POOP_projekat_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(POOP_projekat_1 main.cpp Table.h Table.cpp Database.h Database.cpp Column.h Column.cpp MyExeptions.h Condition.h Condition.cpp Equals.h Equals.cpp NotEquals.h NotEquals.cpp Change.h Change.cpp OrderBy.h OrderBy.cpp)
