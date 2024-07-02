# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(BuildPlotOverTimeLibrary)

# Specify the include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/hsf)

# Add the source files
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/PlotOverTime.cpp
)

# Add the library
add_library(PlotOverTime STATIC ${SOURCES})

# Sepcify thr library directories
link_directories(${PROJECT_SOURCE_DIR}/lib)

# Link the static libraries
target_link_libraries(PlotOverTime libhsf.a)

# Specify the library output directory
set_target_properties(PlotOverTime PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

