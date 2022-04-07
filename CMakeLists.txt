# CMakeList.txt : CMake project for CMakeProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CMakeProject")

# Add source to this project's executable.
add_executable (CMakeProject "Main.cpp" "ShuntingYard.cpp" "ShuntingYard.h" "Geometry.h" "Geometry.cpp" "Huff.h" "Utilities.h")
