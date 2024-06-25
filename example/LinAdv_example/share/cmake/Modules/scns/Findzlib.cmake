#
# Findzlib.cmake
#
# Input:
#
#    zlib_ROOT: Root path of zlib installation
#
# Output:
#
#    zlib_FOUND: Set to true if a valid zlib is found, false otherwise
#
# If a valid zlib is found, the following variables are set:
#
#    zlib_DEFINITIONS: Compiler definitions for zlib users
#    zlib_INCLUDE_DIRS: Include paths (w/o -I) for zlib users
#    zlib_LIBRARIES: Libraries for zlib users.
#

if (zlib_FOUND)
  return()
endif()

if (NOT zlib_ROOT)
  set(zlib_ROOT ${HSF_ROOT})
endif()

include(LibFindMacros)

libfind_check_includes(zlib NAMES zlib.h PATHS ${zlib_ROOT}/include)
libfind_check_components(zlib NAMES z PATHS ${zlib_ROOT}/lib)
libfind_extract_version(zlib zlib.h ZLIB_VERSION)

libfind_process(zlib)
