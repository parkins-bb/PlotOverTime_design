#
# Findyaml-cpp.cmake
#
# Input:
#
#    yaml-cpp_ROOT: Root path of yaml-cpp installation
#
# Output:
#
#    yaml-cpp_FOUND: Set to true if a valid yaml-cpp is found, false otherwise
#
#  If a void yaml-cpp is found, the following variables are set:
#
#    yaml-cpp_DEFINITIONS: Compiler definitions for yaml-cpp users
#    yaml-cpp_INCLUDE_DIRS: Include paths (w/o -I) for yaml-cpp users
#    yaml-cpp_LIBRARIES: Libraries for yaml-cpp users.
#

if (yaml-cpp_FOUND)
  return()
endif()

if (NOT yaml-cpp_ROOT)
  set(yaml-cpp_ROOT  ${HSF_ROOT})
endif ()

include(LibFindMacros)

libfind_check_includes(yaml-cpp NAMES yaml-cpp/yaml.h
  PATHS ${yaml-cpp_ROOT}/include NO_DEFAULT_PATH)
libfind_check_components(yaml-cpp NAMES yaml-cpp
  PATHS ${yaml-cpp_ROOT}/lib NO_DEFAULT_PATH)

libfind_process(yaml-cpp)
