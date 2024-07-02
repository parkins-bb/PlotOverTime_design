#
# Findmetis.cmake
#
# Input:
#
#    metis_ROOT: Root path of metis installation
#
# Output:
#
#    metis_FOUND: Set to true if a valid metis is found, false otherwise
#
#  If a void metis is found, the following variables are set:
#
#    metis_DEFINITIONS: Compiler definitions for metis users
#    metis_INCLUDE_DIRS: Include paths (w/o -I) for metis users
#    metis_LIBRARIES: Libraries for metis users.
#

if (metis_FOUND)
  return()
endif()

if (NOT metis_ROOT)
  set(metis_ROOT  ${HSF_ROOT})
endif ()

include(LibFindMacros)

libfind_check_includes(metis NAMES metis.h
  PATHS ${metis_ROOT}/include NO_DEFAULT_PATH)
libfind_check_components(metis NAMES metis
  PATHS ${metis_ROOT}/lib NO_DEFAULT_PATH)
libfind_extract_version(metis metis.h METIS_VER_MAJOR METIS_VER_MINOR
  METIS_VER_SUBMINOR AS_INTS)

libfind_process(metis)
