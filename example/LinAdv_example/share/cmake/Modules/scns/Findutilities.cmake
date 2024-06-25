#
# Findutilities.cmake
#
# Input:
#
# utilities_ROOT: Root path of utilities installation
#
# Output:
#
# utilities_FOUND: Set to true if a valid utilities is found, false otherwise
#
# If a void utilities is found, the following variables are set:
#
# utilities_DEFINITIONS: Compiler definitions for utilities users
# utilities_INCLUDE_DIRS: Include paths (w/o -I) for utilities users
# utilities_LIBRARIES: Libraries for utilities users.
#

if(utilities_FOUND)
  return()
endif()

if(NOT utilities_ROOT)
  set(utilities_ROOT ${HSF_ROOT})
endif()

include(LibFindMacros)

libfind_check_includes(utilities NAMES utilities.h PATHS
                       ${utilities_ROOT}/include NO_DEFAULT_PATH)
libfind_check_components(utilities NAMES utilities PATHS ${utilities_ROOT}/lib
                         NO_DEFAULT_PATH)

libfind_process(utilities)
