#
# Findparmetis.cmake
#
# This module handles standard FindXXX for parmetis. It requires users to set
# "parmetis_ROOT" so as to locate parmetis path. 
#
# Input:
#
#    parmetis_ROOT: Root path of parmetis installation
#
# Output:
#
#    parmetis_FOUND: Set to true if a valid parmetis is found, false otherwise
#
#  If a void parmetis is found, the following variables are set:
#
#    parmetis_DEFINITIONS: Compiler definitions for parmetis users
#    parmetis_INCLUDE_DIRS: Include paths (w/o -I) for parmetis users
#    parmetis_LIBRARIES: Libraries for parmetis users.
#

if (parmetis_FOUND)
  return()
endif()

if (NOT parmetis_ROOT)
  set(parmetis_ROOT  ${HSF_ROOT})
endif ()

include(LibFindMacros)

libfind_check_dependencies(parmetis NAMES metis mpi REQUIRED QUIET)
libfind_check_includes(parmetis NAMES parmetis.h
  PATHS ${parmetis_ROOT}/include NO_DEFAULT_PATH)
libfind_check_components(parmetis NAMES parmetis
  PATHS ${parmetis_ROOT}/lib NO_DEFAULT_PATH)
libfind_extract_version(parmetis parmetis.h PARMETIS_MAJOR_VERSION
  PARMETIS_MINOR_VERSION PARMETIS_SUBMINOR_VERSION AS_INTS)

libfind_process(parmetis)
