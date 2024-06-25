#
# Findunap.cmake
#
# Input:
#
# unap_ROOT: Root path of unap installation
#
# Output:
#
# unap_FOUND: Set to true if a valid unap is found, false otherwise
#
# If a void unap is found, the following variables are set:
#
# unap_DEFINITIONS: Compiler definitions for unap users unap_INCLUDE_DIRS:
# Include paths (w/o -I) for unap users unap_LIBRARIES: Libraries for unap
# users.
#

if(unap_FOUND)
  return()
endif()

if(NOT unap_ROOT)
  set(unap_ROOT  ${HSF_ROOT})
endif()

include(LibFindMacros)

libfind_check_includes(unap NAMES unap.h PATHS ${unap_ROOT}/include
                       NO_DEFAULT_PATH)
libfind_check_components(unap NAMES unap PATHS ${unap_ROOT}/lib NO_DEFAULT_PATH)

libfind_process(unap)
