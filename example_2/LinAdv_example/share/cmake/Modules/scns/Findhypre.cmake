#
# Findhypre.cmake
#
# Input:
#
#    hypre_ROOT: Root path of hypre installation
#
# Output:
#
#    hypre_FOUND: Set to true if a valid hypre is found, false otherwise
#
#  If a void hypre is found, the following variables are set:
#
#    hypre_DEFINITIONS: Compiler definitions for hypre users
#    hypre_INCLUDE_DIRS: Include paths (w/o -I) for hypre users
#    hypre_LIBRARIES: Libraries for hypre users.
#

if (hypre_FOUND)
  return()
endif()

if (NOT hypre_ROOT)
  set(hypre_ROOT  ${HSF_ROOT})
endif ()

include(LibFindMacros)

libfind_check_dependencies(hypre NAMES mpi REQUIRED QUIET)
libfind_check_includes(hypre NAMES HYPRE.h HYPRE_config.h
  PATHS ${hypre_ROOT}/include NO_DEFAULT_PATH)
libfind_check_components(hypre NAMES HYPRE
  PATHS ${hypre_ROOT}/lib NO_DEFAULT_PATH)
libfind_extract_version(hypre HYPRE_config.h HYPRE_RELEASE_VERSION)

libfind_process(hypre)
