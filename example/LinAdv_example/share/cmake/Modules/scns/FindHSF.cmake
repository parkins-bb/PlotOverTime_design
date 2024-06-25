#
# FindHSF.cmake
#
# Input:
#
# HSF_ROOT: Root path of HSF installation
#
# Output:
#
# HSF_FOUND: Set to true if a valid HSF is found, false otherwise
#
# If a void HSF is found, the following variables are set:
#
# HSF_DEFINITIONS: Compiler definitions for HSF users HSF_INCLUDE_DIRS: Include
# paths (w/o -I) for HSF users HSF_LIBRARIES: Libraries for HSF users.
#

set(allowed_ndims 2 3)
if(NOT JAUMIN_NDIM)
  set(JAUMIN_NDIM 2)
elseif(NOT JAUMIN_NDIM IN_LIST allowed_ndims)
  message(
    FATAL_ERROR
      "Invalid JAUMIN_NDIM value '${JAUMIN_NDIM}', should be '2' or '3'")
endif()

if(HSF_FOUND)
  return()
endif()

if(NOT HSF_ROOT)
  message(FATAL_ERROR "HSF_ROOT not set")
endif()

# LibFindMacros
if(NOT libfind_ROOT)
  set(libfind_ROOT ${HSF_ROOT})
endif()

# utilities
if(NOT utilities_ROOT)
  set(utilities_ROOT ${HSF_ROOT})
endif()
# hdf5
if(NOT hdf5_ROOT)
  set(hdf5_ROOT ${HSF_ROOT})
endif()
# zoltan
if(NOT zoltan_ROOT)
  set(zoltan_ROOT ${HSF_ROOT})
endif()
# parmetis
if(NOT parmetis_ROOT)
  set(parmetis_ROOT ${HSF_ROOT})
endif()
# metis
if(NOT metis_ROOT)
  set(metis_ROOT ${HSF_ROOT})
endif()
# yamlcpp
if (NOT yamlcpp_ROOT)
  set(yamlcpp_ROOT  ${HSF_ROOT})
endif ()

# use LibFindMacros
set(libfind_module_path "${libfind_ROOT}/share/cmake/Modules/scns")
if(NOT libfind_module_path IN_LIST CMAKE_MODULE_PATH)
  list(INSERT CMAKE_MODULE_PATH 0 ${libfind_module_path})
endif()
include(LibFindMacros)

# dependence
set(hsf_compile_deps "utilities;hypre;hdf5;zoltan;parmetis;yaml-cpp;metis;mpi")
libfind_check_dependencies(HSF NAMES ${hsf_compile_deps} REQUIRED QUIET)

libfind_add_definitions(HSF "-DNDIM=${JAUMIN_NDIM}")
libfind_add_definitions(HSF "-DTIMERS")

libfind_check_includes(HSF NAMES ustrMesh.hpp PATHS ${HSF_ROOT}/include/hsf
                       NO_DEFAULT_PATH)

libfind_check_components(HSF NAMES hsf PATHS ${HSF_ROOT}/lib NO_DEFAULT_PATH)

libfind_process(HSF)
