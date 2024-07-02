#
# LibFindMacros.cmake
#
# A library to simplify cmake find module implementation and assist external
# package processing in CMakeLists.txt.
#

# This library requires behaviors of cmake 3.3, for example the IN_LIST
# keyword in the `if` command. Set it explicitly here so the user does not
# need to change their CMakeLists.txt.
cmake_policy(VERSION 3.3)

#############################################################################
#
# General macros to enhance cmake itself
#
#############################################################################

#
# libfind_find_package(pkg [other find_package args])
#
# a drop-in replacement of find_package which handles special packages such as
# mpi and openmp, and checks whether the output of find_package conforms to
# the cmake convention.
#
macro (libfind_find_package pkg)
  if (${pkg} STREQUAL "mpi" OR ${pkg} STREQUAL "MPI")
    libfind_find_mpi(${ARGN})
  elseif(${pkg} STREQUAL "openmp" OR ${pkg} STREQUAL "OpenMP")
    libfind_find_openmp(${ARGN})
  else()
    find_package(${pkg} ${ARGN})
  endif()
  # check non-standard behaviors
  if(${pkg}_FOUND)
    if (NOT DEFINED ${pkg}_DEFINITIONS AND (DEFINED ${pkg}_FLAGS OR DEFINED
      ${pkg}_COMPILE_FLAGS))
      message(AUTHOR_WARNING "Find module of package '${pkg}' does not define
      '${pkg}_DEFINITIONS' but define other relavant variables. This is
      non-standard behavior and shall be fixed.")
      if (DEFINED ${pkg}_FLAGS)
        set(${pkg}_DEFINITIONS ${${pkg}_FLAGS})
      elseif(DEFINED ${pkg}_COMPILE_FLAGS)
        set(${pkg}_DEFINITIONS ${${pkg}_COMPILE_FLAGS})
      endif()
    endif()
    if (NOT DEFINED ${pkg}_INCLUDE_DIRS AND (DEFINED ${pkg}_INCLUDE_PATH OR
      DEFINED ${pkg}_INCLUDES))
      message(AUTHOR_WARNING "Find module of package '${pkg}' does not define
      '${pkg}_INCLUDE_DIRS' but define other relavant variables. This is
      non-standard behavior and shall be fixed.")
      if (DEFINED ${pkg}_INCLUDE_PATH)
        set(${pkg}_INCLUDE_DIRS ${${pkg}_INCLUDE_PATH})
      elseif(DEFINED ${pkg}_INCLUDES)
        set(${pkg}_INCLUDE_DIRS ${${pkg}_INCLUDES})
      endif()
    endif()
    if (NOT DEFINED ${pkg}_LIBRARIES AND (DEFINED ${pkg}_LIBS OR DEFINED
      ${pkg}_LIB))
      message(AUTHOR_WARNING "Find module of package '${pkg}' does not define
      '${pkg}_LIBRARIES' but define other relavant variables. This is
      non-standard behavior and shall be fixed.")
      if (DEFINED ${pkg}_LIBS)
        set(${pkg}_LIBRARIES ${${pkg}_LIBS})
      elseif(DEFINED ${pkg}_LIB)
        set(${pkg}_LIBRARIES ${${pkg}_LIB})
      endif()
    endif()
    if (${pkg}_LIBRARIES)
      libfind_dedup_libraries(${pkg}_LIBRARIES ${${pkg}_LIBRARIES})
      libfind_remove_dangerious_libs(${pkg}_LIBRARIES ${${pkg}_LIBRARIES})
    endif()
  endif()
endmacro()

#
# libfind_find_mpi([other find_package args])
#
# Find MPI package and convert the output to standard form for the 'mpi'
# package. It internally calls find_package(MPI)
#
function (libfind_find_mpi)
  if (mpi_FOUND)
    return()
  endif()
  find_package(MPI ${ARGN})
  set(mpi_FOUND ${MPI_FOUND} PARENT_SCOPE)
  if (MPI_FOUND)
    set(mpi_COMPILE_FLAGS "")
    set(mpi_INCLUDE_PATH "")
    set(mpi_LINK_FLAGS "")
    set(mpi_LIBS "")
    foreach (lang CXX C Fortran)
      set(mpi_${lang}_COMPILER "${MPI_${lang}_COMPILER}" PARENT_SCOPE)
      list(APPEND mpi_COMPILE_FLAGS ${MPI_${lang}_COMPILE_FLAGS})
      list(APPEND mpi_INCLUDE_PATH ${MPI_${lang}_INCLUDE_PATH})
      list(APPEND mpi_LINK_FLAGS ${MPI_${lang}_LINK_FLAGS})
      list(APPEND mpi_LIBS ${MPI_${lang}_LIBRARIES})
    endforeach()
    if (mpi_COMPILE_FLAGS)
      list(REMOVE_DUPLICATES mpi_COMPILE_FLAGS)
      string(STRIP "${mpi_COMPILE_FLAGS}" mpi_COMPILE_FLAGS)
    endif()
    if (mpi_INCLUDE_PATH)
      list(REMOVE_DUPLICATES mpi_INCLUDE_PATH)
      string(STRIP "${mpi_INCLUDE_PATH}" mpi_INCLUDE_PATH)
    endif()
    if (mpi_LINK_FLAGS)
      list(REMOVE_DUPLICATES mpi_LINK_FLAGS)
      string(STRIP "${mpi_LINK_FLAGS}" mpi_LINK_FLAGS)
    endif()
    set(mpi_libraries ${mpi_LINK_FLAGS} ${mpi_LIBS})
    libfind_remove_dangerious_libs(mpi_libraries ${mpi_libraries})
    set(mpi_DEFINITIONS ${mpi_COMPILE_FLAGS} PARENT_SCOPE)
    set(mpi_INCLUDE_DIRS ${mpi_INCLUDE_PATH} PARENT_SCOPE)
    set(mpi_LIBRARIES ${mpi_libraries} PARENT_SCOPE)
  endif()
endfunction()

#
# libfind_find_openmp([other find_package args])
#
# Find OpenMP package and convert the output to standard form for the 'openmp'
# package. It internally calls find_package(OpenMP)
#
function (libfind_find_openmp)
  if (openmp_FOUND)
    return()
  endif()
  find_package(OpenMP ${ARGN})
  if (OpenMP_C_FLAGS)
    set(openmp_FOUND TRUE PARENT_SCOPE)
    set(omp_FLAGS "")
    set(omp_LIBS "")
    foreach (lang C CXX Fortran)
      list(APPEND omp_FLAGS ${OpenMP_${lang}_FLAGS})
      list(APPEND omp_LIBS ${OpenMP_${lang}_LIBRARIES})
    endforeach()
    if (omp_FLAGS)
      list(REMOVE_DUPLICATES omp_FLAGS)
    endif()
    if (omp_LIBS)
      list(REMOVE_DUPLICATES omp_LIBS)
      libfind_remove_dangerious_libs(omp_LIBS ${omp_LIBS})
    endif()
    set(openmp_DEFINITIONS ${omp_FLAGS} PARENT_SCOPE)
    set(openmp_LIBRARIES ${omp_LIBS} PARENT_SCOPE)
    if (DEFINED OpenMP_C_VERSION)
      set(openmp_VERSION ${OpenMP_C_VERSION} PARENT_SCOPE)
    elseif(DEFINED OpenMP_C_SPEC_DATE)
      set(openmp_VERSION ${OpenMP_C_SPEC_DATE} PARENT_SCOPE)
    endif()
  else ()
    set(openmp_FOUND FALSE PARENT_SCOPE)
  endif()
endfunction()

#
# libfind_pkgconfig(<prefix> [QUIET] [REQUIRED] <other pkg_check_module arguments>)
#
# Detect a package using pkg_check_modules and convert the results to standard
# cmake find module outputs. The <PREFIX>_STATIC values from pkg_check_modules
# are used and doubled checked for availablity.
#
function (libfind_pkgconfig PKG)
  if (${PKG}_FOUND)
    return()
  endif()
  # extract the value of option QUIET and REQUIRED of pkg_check_module since
  # we need them in this macro. The arguments are consumed.
  cmake_parse_arguments(LFPC "QUIET;REQUIRED" "" "" ${ARGN})
  set(pkgconf_args ${LFPC_UNPARSED_ARGUMENTS})
  find_package(PkgConfig QUIET REQUIRED)
  pkg_check_modules(${PKG}_I QUIET ${pkgconf_args})
  if (${PKG}_I_FOUND)
    # Use the returned variables from pkg_check_modules as a hint to find all
    # required include dirs and libraries. This is the recommended approach by
    # cmake developers, since pkg-config is not always reliable.
    set(found TRUE)
    set(reason_missing_include FALSE)
    set(reason_missing_lib FALSE)
    set(missing_includes "")
    set(missing_libs "")
    foreach (dir ${${PKG}_I_STATIC_INCLUDE_DIRS})
      if (NOT EXISTS ${dir})
        set(found FALSE)
        set(reason_missing_include TRUE)
        list(APPEND missing_includes ${dir})
      endif()
    endforeach()
    set(${PKG}_LIBS "")
    foreach (lib ${${PKG}_I_STATIC_LIBRARIES})
      find_library(lib${lib} NAMES ${lib} PATHS ${${PKG}_I_STATIC_LIBRARY_DIRS})
      if (lib${lib})
        list(APPEND ${PKG}_LIBS ${lib${lib}})
      else()
        set(found FALSE)
        set(reason_missing_lib TRUE)
        list(APPEND missing_libs ${lib})
      endif()
    endforeach()
    if (found)
      set(${PKG}_FOUND TRUE PARENT_SCOPE)
      if (${PKG}_I_STATIC_CFLAGS_OTHER)
        set(${PKG}_DEFINITIONS ${${PKG}_I_STATIC_CFLAGS_OTHER} PARENT_SCOPE)
      endif()
      set(${PKG}_INCLUDE_DIRS ${${PKG}_I_STATIC_INCLUDE_DIRS} PARENT_SCOPE)
      # NOTE: the ${PKG}_I_STATIC_LDFLAGS_OTHER is ignored at the moment as
      # it may require to appear in specific position of the linker args. In
      # case of problems, we shall use ${PKG}_I_STATIC_LDFLAGS instead.
      libfind_remove_dangerious_libs(${PKG}_LIBS ${${PKG}_LIBS})
      set(${PKG}_LIBRARIES ${${PKG}_LIBS} PARENT_SCOPE)
      if (${PKG}_I_VERSION)
        set(${PKG}_VERSION ${${PKG}_I_VERSION} PARENT_SCOPE)
      endif()
      if (NOT LFPC_QUIET)
        message(STATUS "Found ${PKG} ${${PKG}_I_VERSION}")
      endif()
    else()
      set(${PKG}_FOUND FALSE PARENT_SCOPE)
      if (LFPC_REQUIRED)
        set(msg "REQUIRED PACKAGE NOT FOUND:")
        set(msg "${msg} Invalid pkg_check_module return for '${pkgconf_args}':\n")
        if (reason_missing_include)
          set(msg "${msg}  Include directories '${missing_includes}' not found\n")
        endif()
        if (reason_missing_lib)
          set(msg "${msg}  Libraries '${missing_libs}' not found\n")
        endif()
        message(FATAL_ERROR "${msg}")
      endif()
    endif()
  else()
    set(${PKG}_FOUND FALSE PARENT_SCOPE)
    if (LFPC_REQUIRED)
      set(msg "REQUIRED PACKAGE NOT FOUND:")
      set(msg "${msg} pkg_check_module can not find module '${pkgconf_args}',")
      set(msg "${msg} please check CMAKE_PREFIX_PATH and the PKG_CONFIG_PATH")
      set(msg "${msg} environment variable, and make sure you are passing")
      set(msg "${msg} the correct pkg_check_module args\n")
      message(FATAL_ERROR "${msg}")
    endif()
  endif()
endfunction()

#
# _dedup_non_grouping_library(<OUT> [lib...])
#
# deduplicate a list of library items which do not contain "-Wl,--start-group"
# and "-Wl,--end-group"
#
function (_dedup_non_grouping_library OUT)
  if (NOT ARGN)
    return()
  endif()
  # First normalize the list by combining the '-Wl,-rpath -Wl,path'
  string(REGEX REPLACE "-Wl,-rpath[ \t]+-Wl,([^ \t]+)" "-Wl,-rpath,\\1" list1 "${ARGN}")
  string(REGEX REPLACE "-Wl,-rpath;-Wl,([^;]+)" "-Wl,-rpath,\\1" list1 "${list1}")
  # Then dedup the library path and keep the first occurance
  set(tmp "")
  foreach(n ${list1})
    if (n MATCHES "^-[^l].+$" AND n IN_LIST tmp)
      continue()
    endif()
    list(APPEND tmp ${n})
  endforeach()
  set(list1 ${tmp})
  # Finally dedup the libraries and keep the last occurance
  list(REVERSE list1)
  list(REMOVE_DUPLICATES list1)
  list(REVERSE list1)
  set(${OUT} ${list1} PARENT_SCOPE)
endfunction()

#
# libfind_dedup_libraries(<OUT> LIB [LIB...])
#
# Remove duplicated libraries from the library list while still preserves
# their dependency order.
#
function (libfind_dedup_libraries OUT)
  if (NOT ARGN)
    return()
  endif()
  # We first sweep the libs, turn each group into a deduped special
  # "library" and preserve the other libs.
  set(other_libs "")
  set(group_libs "")
  set(in_group FALSE)
  foreach (i ${ARGN})
    if (${i} MATCHES "^-Wl,--start-group$")
      set(in_group TRUE)
    elseif (${i} STREQUAL "-Wl,--end-group")
      if (group_libs)
        _dedup_non_grouping_library(group_libs ${group_libs})
      endif()
      string(REPLACE ";" "<>" group_libs "${group_libs}")
      list(APPEND other_libs "[${group_libs}]")
      set(group_libs "")
      set(in_group FALSE)
    else()
      if (in_group)
        list(APPEND group_libs ${i})
      else()
        list(APPEND other_libs ${i})
      endif()
    endif()
  endforeach()
  if (in_group)
    message(FATAL_ERROR "-Wl,--start-group is not closed by --Wl,--end-group, "
      "this is not allowed.\nThe libs is: ${ARGN}")
  endif()
  # We then dedup the transformed other libs
  if (other_libs)
    _dedup_non_grouping_library(other_libs ${other_libs})
  endif()
  # Finally, we transform the internal representation into standard form
  set(libs "")
  foreach (i ${other_libs})
    set(is_group FALSE)
    if (i MATCHES "\\[.+\\]")
      string(REGEX REPLACE "^\\[(.+)\\]$" "\\1" i "${i}")
      string(REPLACE "<>" ";" i "${i}")
      set(is_group TRUE)
    endif()
    if (is_group)
      list(APPEND libs "-Wl,--start-group")
    endif()
    foreach (k ${i})
      list(APPEND libs ${k})
    endforeach()
    if (is_group)
      list(APPEND libs "-Wl,--end-group")
    endif()
  endforeach()
  set(${OUT} ${libs} PARENT_SCOPE)
endfunction()

#############################################################################
#
# Macros for CMakeLists.txt
#
#############################################################################

#
# libfind_check_external_packages(PREFIX [REQUIRED] [QUIET] NAMES [PKG [PKG...]])
#
# use libfind_find_package to check a list of external packages and gather
# their outputs to <PREFIX>_{DEFINITIONS,INCLUDE_DIRS,LIBRARIES} for easy
# usage of other parts of a CMakeLists.txt. <PREFIX>_<lang>_COMPILER is also
# set if any <pkg>_<lang>_COMPILER is found.
#
macro (libfind_check_external_packages PREFIX)
  cmake_parse_arguments(LFCEP "REQUIRED;QUIET" "" "NAMES" ${ARGN})
  set(lfcep_args "")
  if (LFCEP_REQUIRED)
    list(APPEND lfcep_args REQUIRED)
  endif()
  if (LFCEP_QUIET)
    list(APPEND lfcep_args QUIET)
  endif()
  foreach (pkg ${LFCEP_NAMES})
    libfind_find_package(${pkg} ${lfcep_args})
    if (${pkg}_FOUND)
      foreach (lang CXX C Fortran)
        if (${pkg}_${lang}_COMPILER)
          set(${PREFIX}_${lang}_COMPILER ${${pkg}_${lang}_COMPILER})
        endif()
      endforeach()
      list(APPEND ${PREFIX}_DEFINITIONS ${${pkg}_DEFINITIONS})
      list(APPEND ${PREFIX}_INCLUDE_DIRS ${${pkg}_INCLUDE_DIRS})
      list(APPEND ${PREFIX}_LIBRARIES ${${pkg}_LIBRARIES})
    endif()
  endforeach()
  if (${PREFIX}_DEFINITIONS)
    list(REMOVE_DUPLICATES ${PREFIX}_DEFINITIONS)
  endif()
  if (${PREFIX}_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES ${PREFIX}_INCLUDE_DIRS)
  endif()
  if (${PREFIX}_LIBRAIRES)
    libfind_dedup_libraries(${PREFIX}_LIBRARIES ${${PREFIX}_LIBRARIES})
    libfind_remove_dangerious_libs(${PREFIX}_LIBRARIES ${${PREFIX}_LIBRARIES})
  endif()
  unset(lfcep_args)
endmacro()

#
# libfind_relocate_lib(<VAR> <lib> [NO_LIBRARY_PATH])
#
# This macro converts fullpath linking libs generated by cmake's find
# modules to standard "-LXXX -Wl,rpath,XXX -lYYY" notation. When the
# 'NO_LIBRARY_PATH' option is selected, only '-lYYY' is returned, so that the
# library can adapt to the system native one when linking.
#
function (libfind_relocate_lib var lib)
  if (NOT ${lib} MATCHES "^.*lib.+\\.(so|a)$")
    set(${var} "${lib}" PARENT_SCOPE)
    return()
  endif()
  cmake_parse_arguments(LFRL "NO_LIBRARY_PATH" "" "" ${ARGN})
  set(tmp "")
  if (NOT LFRL_NO_LIBRARY_PATH)
    get_filename_component(path "${lib}" DIRECTORY)
    set(tmp "${tmp} -L${path} -Wl,-rpath,${path}")
  endif()
  get_filename_component(name "${lib}" NAME)
  if (name MATCHES "^lib(.+)\\.(so|a)$")
    string(REGEX REPLACE "lib(.+)\\.(so|a)" "-l\\1" tmp1 ${name})
    set(tmp "${tmp} ${tmp1}")
  else()
    set(tmp "${tmp} ${lib}")
  endif()
  string(STRIP "${tmp}" tmp)
  if (tmp)
    set(${var} "${tmp}" PARENT_SCOPE)
  endif()
endfunction()

#
# libfind_process_compile_dependencies(<prefix> REQUIRED pkg [pkg...] OPTIONAL
# pkg [pkg...] DEFAULT_ENABLED pkg [pkg...])
#
# Process compile dependencies and export the results to the rest of the
# CMakeLists.
#
# This macro processes two types of packages: required and optional. Each
# required package is searched and an error will occur if not found. For each
# optinal package, it is by default turned on if in the DEFAULT_ENABLED list,
# and turned off otherwise. Turned-off packages will never be searched.
# Turned-on packages will be searched but no errors will occur in case of not
# found.
#
# The following outputs are set, in addition to those set regularlly by find
# modules:
#
# 1. <prefix>_{DEFINITIONS,INCLUDE_DIRS,LIBRARIES}: Combined find module
#    outputs
# 2. <prefix>_DETECTED: A list of all detected packages
#
# Users shall set CMAKE_MODULE_PATH to contain find modules of the listed
# packages, otherwise cmake will complain. Users can set <pkg>_ROOT to find
# package at a specified location. See the related module documentation for
# more details on controlling find modules.
#
macro (libfind_process_compile_dependencies PREFIX)
  cmake_parse_arguments(LFPCD "" "" "REQUIRED;OPTIONAL;DEFAULT_ENABLED" ${ARGN})
  # Export and process optional package options
  foreach (pkg ${LFPCD_OPTIONAL})
    string(TOUPPER ${pkg} pkgU)
    if (LFPCD_DEFAULT_ENABLED AND pkg IN_LIST LFPCD_DEFAULT_ENABLED)
      option(ENABLE_${pkgU} "Enable ${pkg} integration (default: ON)" ON)
    else()
      option(ENABLE_${pkgU} "Enable ${pkg} integration (default: OFF)" OFF)
    endif()
  endforeach()
  # Compute asked optional packages
  set(lfpcd_optional_asked "")
  foreach (pkg ${LFPCD_OPTIONAL})
    string(TOUPPER ${pkg} pkgU)
    if (ENABLE_${pkgU})
      list(APPEND lfpcd_optional_asked ${pkg})
    endif()
  endforeach()
  # Check all packages
  libfind_check_external_packages(${PREFIX}_1 NAMES ${LFPCD_REQUIRED} REQUIRED)
  if (lfpcd_optional_asked)
    libfind_check_external_packages(${PREFIX}_2 NAMES ${lfpcd_optional_asked})
  endif()
  # Compute detected packages
  set(${PREFIX}_DETECTED ${LFPCD_REQUIRED})
  foreach (pkg ${lfpcd_optional_asked})
    if (${pkg}_FOUND)
      list(APPEND ${PREFIX}_DETECTED ${pkg})
    endif()
  endforeach()
  if (${PREFIX}_DETECTED)
    list(REMOVE_DUPLICATES ${PREFIX}_DETECTED)
  endif()
  # Produce outputs
  set(${PREFIX}_DEFINITIONS ${${PREFIX}_1_DEFINITIONS}
    ${${PREFIX}_2_DEFINITIONS})
  set(${PREFIX}_INCLUDE_DIRS ${${PREFIX}_1_INCLUDE_DIRS}
    ${${PREFIX}_2_INCLUDE_DIRS})
  set(${PREFIX}_LIBRARIES ${${PREFIX}_1_LIBRARIES}
    ${${PREFIX}_2_LIBRARIES})
  if (${PREFIX}_DEFINITIONS)
    list(REMOVE_DUPLICATES ${PREFIX}_DEFINITIONS)
  endif()
  if (${PREFIX}_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES ${PREFIX}_INCLUDE_DIRS)
  endif()
  if (${PREFIX}_LIBRARIES)
    libfind_dedup_libraries(${PREFIX}_LIBRARIES ${${PREFIX}_LIBRARIES})
    libfind_remove_dangerious_libs(${PREFIX}_LIBRARIES ${${PREFIX}_LIBRARIES})
  endif()
  # clean variables since I am a macro.
  foreach (v LFPCD_REQUIRED LFPCD_OPTIONAL LFPCD_DEFAULT_ENABLED
      ${PREFIX}_1_DEFINITIONS ${PREFIX}_1_INCLUDE_DIRS ${PREFIX}_1_LIBRARIES
      ${PREFIX}_2_DEFINITIONS ${PREFIX}_2_INCLUDE_DIRS ${PREFIX}_2_LIBRARIES
      lfpcd_optional_asked pkg pkgU)
    unset(${v})
  endforeach()
endmacro()

#
# libfind_format_flags(<prefix> DEFINITIONS def [def...] INCLUDE_DIRS inc
# [inc...] LIBRARIES lib [lib...])
#
# Converts the list of compiler flags, include directories, and linking
# libraries generated by cmake find modules to the string representation used
# by traditional tools such as make and pkg-config.
#
# Input:
#    <pkg>: the package name used by cmake find modules
#
# Output:
#    <pkg>_CFLAGS: String representing compiler flags in DEFINITIONS
#    <pkg>_CPPFLAGS: String representing includes in INCLUDE_DIRS
#    <pkg>_LIBS: String representing libraries in LIBRARIES
#
function(libfind_format_flags prefix)
  cmake_parse_arguments(LFFF "" "" "DEFINITIONS;INCLUDE_DIRS;LIBRARIES" ${ARGN})
  if (LFFF_DEFINITIONS)
    list(REMOVE_DUPLICATES LFFF_DEFINITIONS)
    set(tmp "")
    foreach(n ${LFFF_DEFINITIONS})
      string(STRIP ${n} n)
      if(${n} MATCHES "^-.+$")
        set(tmp "${tmp} ${n}")
      else()
        set(tmp "${tmp} -D${n}")
      endif()
    endforeach()
    string(STRIP "${tmp}" tmp)
    if (tmp)
      set(${prefix}_CFLAGS "${tmp}" PARENT_SCOPE)
    endif()
  endif()
  if (LFFF_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES LFFF_INCLUDE_DIRS)
    set(tmp "")
    foreach(n ${LFFF_INCLUDE_DIRS})
      string(STRIP ${n} n)
      if(${n} MATCHES "^-I.+$")
        set(tmp "${tmp} ${n}")
      else()
        set(tmp "${tmp} -I${n}")
      endif()
    endforeach()
    string(STRIP "${tmp}" tmp)
    if (tmp)
      set(${prefix}_CPPFLAGS "${tmp}" PARENT_SCOPE)
    endif()
  endif()
  set(LFFF_SYSTEM_LIBS pthread c m rt mpifort mpicxx mpi gomp gfortran stdc++)
  if (LFFF_LIBRARIES)
    libfind_dedup_libraries(LFFF_LIBRARIES ${LFFF_LIBRARIES})
    set(tmp "")
    foreach(n ${LFFF_LIBRARIES})
      string(STRIP ${n} n)
      if (n MATCHES "^.*lib.+\\.(so|a)$")
        string(REGEX REPLACE ".*lib(.+)\\.(so|a)" "\\1" tmp1 ${n})
        if (tmp1 IN_LIST LFFF_SYSTEM_LIBS)
          libfind_relocate_lib(nn ${n} NO_LIBRARY_PATH)
        else()
          libfind_relocate_lib(nn ${n})
        endif()
        set(tmp "${tmp} ${nn}")
      else()
        set(tmp "${tmp} ${n}")
      endif()
    endforeach()
    string(STRIP "${tmp}" tmp)
    if (tmp)
      # Since many libraris can share the same "-L/-Wl,-rpath", we deduplicate
      # it here. We assume the paths do not contain spaces.
      string(REGEX MATCHALL "[^ ]+" tmp1 "${tmp}")
      libfind_dedup_libraries(tmp1 ${tmp1})
      set(tmp "")
      foreach (n ${tmp1})
        set(tmp "${tmp} ${n}")
      endforeach()
      string(STRIP ${tmp} tmp)
      set(${prefix}_LIBS "${tmp}" PARENT_SCOPE)
    endif()
  endif()
endfunction()

#
# libfind_remove_compile_definition(<regex> <OUT> <def> [def...])
#
# Remove a compile flag safely from a list of compile definitons. The flag to
# remove is denoted by a regex.
#
function (libfind_remove_compile_definition regex OUT def)
  set(tmp "")
  foreach (n ${def} ${ARGN})
    string(REGEX REPLACE "(^|[ \\t]+)${regex}" "" n "${n}")
    string(STRIP "${n}" n)
    if (NOT n STREQUAL "")
      list(APPEND tmp ${n})
    endif()
  endforeach()
  set(${OUT} "${tmp}" PARENT_SCOPE)
endfunction()

#
# libfind_remove_library(<name> <OUT> <lib> [lib...])
#
# Remove a library safely from a list of libraries. The library name is
# denoted by <name>. For example, one can remove libm by specify name as 'm'.
#
function (libfind_remove_library name OUT lib)
  set(tmp "")
  foreach (n ${lib} ${ARGN})
    # Remove full library name
    string(REGEX REPLACE "[^ ]*lib${name}\\.a" "" n "${n}")
    string(REGEX REPLACE "[^ ]*lib${name}\\.so\\.[^ ]+" "" n "${n}")
    string(REGEX REPLACE "[^ ]*lib${name}\\.so" "" n "${n}")
    # Remove shorthand like -lm
    string(REGEX REPLACE "^-l${name}$" "" n "${n}")
    string(REGEX REPLACE " -l${name}$" "" n "${n}")
    string(REGEX REPLACE "^-l${name} " " " n "${n}")
    string(REGEX REPLACE " -l${name} " " " n "${n}")
    string(STRIP "${n}" n)
    if (NOT n STREQUAL "")
      list(APPEND tmp ${n})
    endif()
  endforeach()
  set(${OUT} "${tmp}" PARENT_SCOPE)
endfunction()

#
# libfind_remove_dangerious_libs(<OUT> <in> [in...])
#
# Remove some dangerious libraries from the library list so compilers and
# systems will be happy. Only the c library is removed at the moment.
#
function (libfind_remove_dangerious_libs OUT in)
  # Compilers usually link implicitly to c and some libraries (such as Tianhe
  # MPI and Sunway MPI) will fail if the c library appears before the mpi
  # libraries. So it is dangerious.
  set(dangerious_libs c)
  set(tmp ${in} ${ARGN})
  foreach(l ${dangerious_libs})
    libfind_remove_library(${l} tmp ${tmp})
  endforeach()
  set(${OUT} "${tmp}" PARENT_SCOPE)
endfunction()

#############################################################################
#
# Macros for find modules
#
#############################################################################

#
# libfind_find_dependent_package(<prefix> <another package> [find_package args])
#
# Find another package and make it a dependency of the current package.
# This also automatically forwards the "REQUIRED" and "QUIET" argument.
#
macro (libfind_find_dependent_package PREFIX PKG)
  set(${PREFIX}_args ${PKG} ${ARGN})
  if (${PREFIX}_FIND_REQUIRED)
    set(${PREFIX}_args ${${PREFIX}_args} REQUIRED)
  endif()
  if (${PREFIX}_FIND_QUIETLY)
    set(${PREFIX}_args ${${PREFIX}_args} QUIET)
  endif()
  libfind_find_package(${${PREFIX}_args})
  unset(${PREFIX}_args)
  list(APPEND ${PREFIX}_DEPENDENCIES ${PKG})
endmacro()

#
# libfind_pkg_check_modules(<prefix> <pkg> [extra args to pkg_check_modules])
#
# Find another package and make it a dependency of the current package with
# pkg_check_modules. This also automatically forwards the "REQUIRED" and
# "QUIET" argument. The pkg_check_modules output for `pkg-config --static` is
# selected and the output is converted to adhere to standard cmake find_package
# convention.
#
macro (libfind_pkg_check_modules PREFIX PKG)
  find_package(PkgConfig QUIET REQUIRED)
  set(${PREFIX}_args ${PKG})
  if (${PREFIX}_FIND_REQUIRED)
    list(APPEND ${PREFIX}_args REQUIRED)
  endif()
  if (${PREFIX}_FIND_QUIETLY)
    list(APPEND ${PREFIX}_args QUIET)
  endif()
  libfind_pkgconfig(${${PREFIX}_args})
  unset(${PREFIX}_args)
  list(APPEND ${PREFIX}_DEPENDENCIES ${PKG})
endmacro()

#
# libfind_check_pkgconfig(<prefix> <other pkg_check_module arguments>)
#
# Check the definitions, include directories, and libraries of current package
# through pkg-config. Will set the <prefix>_PROCESS_{INCLUDES,LIBS,DEFS}
# according to the value detected.
#
macro (libfind_check_pkgconfig PREFIX MOD)
  set(pkgconf_args ${MOD} ${ARGN})
  find_package(PkgConfig QUIET REQUIRED)
  pkg_check_modules(tmp_P QUIET ${pkgconf_args})
  if (tmp_P_FOUND)
    foreach (dir ${tmp_P_STATIC_INCLUDE_DIRS})
      if (NOT EXISTS ${dir})
        set("${PREFIX}_${dir}" "${PREFIX}_${dir}-NOTFOUND")
      else()
        set("${PREFIX}_${dir}" "${dir}")
      endif()
      list(APPEND ${PREFIX}_PROCESS_INCLUDES "${PREFIX}_${dir}")
    endforeach()
    foreach (lib ${tmp_P_STATIC_LIBRARIES})
      find_library("${PREFIX}_COMPONENT_${lib}" NAMES ${lib}
        PATHS ${tmp_P_STATIC_LIBRARY_DIRS} ${CMAKE_LIBRARY_PATH}
        ${CMAKE_SYSTEM_LIBRARY_PATH} /usr/lib64 /usr/lib NO_DEFAULT_PATH)
      list(APPEND ${PREFIX}_PROCESS_LIBS "${PREFIX}_COMPONENT_${lib}")
    endforeach()
    if (tmp_P_VERSION)
      set(${PREFIX}_VERSION ${tmp_P_VERSION})
    endif()
    list(APPEND ${PREFIX}_PROCESS_DEFS ${tmp_P_STATIC_CFLAGS_OTHER})
  else()
    # nothing is found, set the library flags to indicate that
    set(${PREFIX}_pkgconf_INCLUDE_DIR "${PREFIX}_pkgconf_INCLUDE_DIR-NOTFOUND")
    list(APPEND ${PREFIX}_PROCESS_INCLUDES ${PREFIX}_pkgconf_INCLUDE_DIR)
  endif()
  unset(tmp_P_FOUND)
  unset(pkgconf_args)
endmacro()

#
# libfind_check_dependencies(PREFIX [REQUIRED] [QUIET] NAMES pkg [pkg...])
#
# check a list of packages and make the found ones dependencies of the current
# package. This is intended for find module implementors to handle external
# dependencies gracefully.
#
# NOTE: Since this function is called in find modules, which call this
# function themselves, the variables can clash into each other. So we shall
# define it as a function to scope the variables. This introduces an overhead
# to export the variables defined by libfind_find_package to parent scope.
#
function (libfind_check_dependencies PREFIX)
  cmake_parse_arguments(LFCD "REQUIRED;QUIET" "" "NAMES" ${ARGN})
  set(lfcd_args "")
  if (LFCD_REQUIRED OR ${PREFIX}_FIND_REQUIRED)
    list(APPEND lfcd_args REQUIRED)
  endif()
  if (LFCD_QUIET OR ${PREFIX}_FIND_QUIETLY)
    list(APPEND lfcd_args QUIET)
  endif()
  foreach (pkg ${LFCD_NAMES})
    libfind_find_package(${pkg} ${lfcd_args})
    if (${pkg}_FOUND)
      list(APPEND ${PREFIX}_DEPENDENCIES ${pkg})
    endif()
    foreach (n FOUND ROOT_DIR
        VERSION VERSION_MAJOR VERSION_MINOR VERSION_PATCH
        DEFINITIONS INCLUDE_DIRS LIBRARIES
        CXX_COMPILER C_COMPILER Fortran_COMPILER)
      if (DEFINED ${pkg}_${n})
        set(${pkg}_${n} ${${pkg}_${n}} PARENT_SCOPE)
      endif()
    endforeach()
  endforeach()
  if (${PREFIX}_DEPENDENCIES)
    list(REMOVE_DUPLICATES ${PREFIX}_DEPENDENCIES)
  endif()
  set(${PREFIX}_DEPENDENCIES ${${PREFIX}_DEPENDENCIES} PARENT_SCOPE)
  unset(lfcd_args)
endfunction()

#
# libfind_add_definitions(PREFIX [DEF...])
#
# add definitions to the current package for libfind_process to use.
#
macro (libfind_add_definitions PREFIX)
  list(APPEND ${PREFIX}_PROCESS_DEFS ${ARGN})
endmacro()

#
# libfind_check_includes(PREFIX [NO_DEFAULT_PATH] NAMES name [name...]
#                        PATHS path [path...])
#
# Check if the path contain a valid include dir. Will set
# ${PREFIX}_INCLUDE_DIR to the directory or ${PREFIX}_INCLUDE_DIR-NOT_FOUND
#
macro (libfind_check_includes PREFIX)
  cmake_parse_arguments(LFCI "NO_DEFAULT_PATH" "" "NAMES;PATHS" ${ARGN})
  set(lfci_args "")
  if (LFCI_NO_DEFAULT_PATH)
    set(lfci_args NO_DEFAULT_PATH)
  endif()
  find_path(${PREFIX}_INCLUDE_DIR NAMES ${LFCI_NAMES} PATHS ${LFCI_PATHS}
    ${lfci_args})
  # HACK: cmake always search for /usr/include even when NO_DEFAULT_PATH is
  # passed. We can not avoid this, so we check that here.
  if (LFCI_NO_DEFAULT_PATH AND
      NOT ${PREFIX}_INCLUDE_DIR STREQUAL "${PREFIX}_INCLUDE_DIR-NOTFOUND" AND
      NOT ${PREFIX}_INCLUDE_DIR IN_LIST LFCI_PATHS)
    set(${PREFIX}_INCLUDE_DIR "${PREFIX}_INCLUDE_DIR-NOTFOUND")
  endif()
  list(APPEND ${PREFIX}_PROCESS_INCLUDES ${PREFIX}_INCLUDE_DIR)
  unset(lfci_args)
endmacro()

#
# libfind_check_components(PREFIX [NO_DEFAULT_PATH] NAMES name [name...]
#                          PATHS path [path...])
#
# Find required library components for <package> in specified <path>s. Will
# set ${PREFIX}_COMPONENT_${COMP} to the fullname of the component, or
# ...-NOT_FOUND otherwise. The ${PREFIX}_PROCESS_LIBS are set to these
# variables to assist libfind_process.
#
macro (libfind_check_components PREFIX)
  cmake_parse_arguments(LFCC "NO_DEFAULT_PATH" "" "NAMES;PATHS" ${ARGN})
  set(lfcc_args "")
  if (LFCC_NO_DEFAULT_PATH)
    set(lfcc_args NO_DEFAULT_PATH)
  endif()
  foreach (c ${LFCC_NAMES})
    find_library(${PREFIX}_COMPONENT_${c} NAMES ${c} PATHS ${LFCC_PATHS}
      ${lfcc_args})
    # HACK: cmake always search for /usr/lib even when NO_DEFAULT_PATH is
    # passed. We can not avoid this, so we check that here.
    if (LFCC_NO_DEFAULT_PATH AND
        NOT ${PREFIX}_COMPONENT_${c} STREQUAL
        "${PREFIX}_COMPONENT_${c}-NOTFOUND")
      get_filename_component(tmp ${${PREFIX}_COMPONENT_${c}} PATH)
      if (NOT tmp IN_LIST LFCC_PATHS)
        set(${PREFIX}_COMPONENT_${c} "${PREFIX}_COMPONENT_${c}-NOTFOUND")
      endif()
    endif()
    list(APPEND ${PREFIX}_PROCESS_LIBS ${PREFIX}_COMPONENT_${c})
  endforeach()
  unset(lfcc_args)
endmacro()

#
# libfind_extract_version(<prefix> <header> <define> [define ...] [AS_INTS] [QUIET])
#
# Extracts a version #define from a version.h file, output stored to <PREFIX>_VERSION.
# This function does nothing if the version variable is already defined. If
# AS_INTS is provided, a list of defines is treated as integers denoting parts
# of the version, and they will be "." joined to form the final version
# string.
#
function (libfind_extract_version PREFIX VERSION_H)
  # Skip processing if we already have a version or if the include dir was not found
  if (${PREFIX}_VERSION OR NOT ${PREFIX}_INCLUDE_DIR)
    return()
  endif()
  cmake_parse_arguments(LFEV "AS_INTS;QUIET" "" "" ${ARGN})
  set(quiet FALSE)
  if (${PREFIX}_FIND_QUIETLY OR LFEV_QUIET)
    set(quiet TRUE)
  endif()
  if (NOT LFEV_UNPARSED_ARGUMENTS)
    message(AUTHOR_WARNING "Not enough arguments passed, can not proceed.")
    return()
  endif()
  # Read the header and parse for version number
  set(filename "${${PREFIX}_INCLUDE_DIR}/${VERSION_H}")
  if (NOT EXISTS ${filename})
    if (NOT quiet)
      message(AUTHOR_WARNING "Unable to find ${${PREFIX}_INCLUDE_DIR}/${VERSION_H}")
    endif()
    return()
  endif()
  file(READ "${filename}" header)
  if (LFEV_AS_INTS)
    set(match "")
    foreach (VER ${LFEV_UNPARSED_ARGUMENTS})
      set(str "")
      string(REGEX REPLACE ".*#[ \t]*define[ \t]*${VER}[ \t]+([0-9.]+).*" "\\1"
        str "${header}")
      if (str STREQUAL header)
        message(AUTHOR_WARNING "Unable to find \#define ${VER} <INTEGER> from ${${PREFIX}_INCLUDE_DIR}/${VERSION_H}")
        return()
      else()
        set(match "${match}.${str}")
      endif()
    endforeach()
    string(SUBSTRING ${match} 1 -1 match)
  else()
    list(GET LFEV_UNPARSED_ARGUMENTS 0 DEFINE_NAME)
    string(REGEX REPLACE ".*#[ \t]*define[ \t]*${DEFINE_NAME}[ \t]*\"([^\n]*)\".*" "\\1" match "${header}")
    # No regex match?
    if (match STREQUAL header)
      if (NOT quiet)
        message(AUTHOR_WARNING "Unable to find \#define ${DEFINE_NAME} \"<version>\" from ${${PREFIX}_INCLUDE_DIR}/${VERSION_H}")
      endif()
      return()
    endif()
  endif()
  # Export the version string
  set(${PREFIX}_VERSION "${match}" PARENT_SCOPE)
endfunction()

#
# libfind_process(<PREFIX> [GROUP_COMPONENTS])
#
# Do the final processing once the paths have been detected.  If include dirs
# are needed, ${PREFIX}_PROCESS_INCLUDES should be set to contain all the
# variables, each of which contain one include directory.  Ditto for
# ${PREFIX}_PROCESS_LIBS and library files.  If compiler flags are needed,
# ${PREFIX}_PROCESS_DEFS should be set to the definitions.
#
# Will set ${PREFIX}_FOUND, ${PREFIX}_DEFINITIONS, ${PREFIX}_INCLUDE_DIRS and
# ${PREFIX}_LIBRARIES.  Also handles errors in case library detection was
# required, etc.
#
# If GROUP_COMPONENTS is set, current package's library components are grouped
# by '-Wl,--(start|end)-group' to make them work like a group. This is
# necessary for components that are inter-dependent.
#
function (libfind_process PREFIX)
  # Skip processing if already processed during this configuration run
  if (${PREFIX}_FOUND)
    return()
  endif()

  cmake_parse_arguments(LFP "GROUP_COMPONENTS" "" "" ${ARGN})

  set(found TRUE)  # Start with the assumption that the package was found

  # Did we find any files? Did we miss includes? These are for formatting better error messages.
  set(some_files FALSE)
  set(missing_headers FALSE)

  # Shorthands for some variables that we need often
  set(quiet ${${PREFIX}_FIND_QUIETLY})
  set(required ${${PREFIX}_FIND_REQUIRED})
  set(exactver ${${PREFIX}_FIND_VERSION_EXACT})
  set(findver "${${PREFIX}_FIND_VERSION}")
  set(version "${${PREFIX}_VERSION}")

  # Lists of config option names (all, includes, libs, definitions)
  unset(configopts)
  set(includeopts ${${PREFIX}_PROCESS_INCLUDES})
  set(libraryopts ${${PREFIX}_PROCESS_LIBS})
  set(definitionopts ${${PREFIX}_PROCESS_DEFS})

  if (includeopts)
    list(REMOVE_DUPLICATES includeopts)
  endif()
  if (libraryopts)
    list(REMOVE_DUPLICATES libraryopts)
  endif()
  if (definitionopts)
    list(REMOVE_DUPLICATES definitionopts)
  endif()

  # Clear the variables where the results are stored
  unset(includes)
  unset(libs)
  unset(defs)

  # Process all definitions
  foreach (d ${definitionopts})
    list(APPEND defs ${d})
  endforeach()

  # Process all includes and set found false if any are missing
  foreach (i ${includeopts})
    list(APPEND configopts ${i})
    if (NOT "${${i}}" STREQUAL "${i}-NOTFOUND")
      list(APPEND includes "${${i}}")
    else()
      set(found FALSE)
      set(missing_headers TRUE)
    endif()
  endforeach()

  # Process all libraries and set found false if any are missing
  if (LFP_GROUP_COMPONENTS AND libraryopts)
    list(APPEND libs "-Wl,--start-group")
  endif()
  foreach (l ${libraryopts})
    list(APPEND configopts ${l})
    if (NOT "${${l}}" STREQUAL "${l}-NOTFOUND")
      list(APPEND libs "${${l}}")
    else()
      set(found FALSE)
    endif()
  endforeach()
  if (LFP_GROUP_COMPONENTS AND libraryopts)
    list(APPEND libs "-Wl,--end-group")
  endif()

  # Process the results of dependent packages. Note that the libraries shall
  # be appended to this library, instead of prepended, so this process comes
  # after the processed package.
  if (${PREFIX}_DEPENDENCIES)
    # NOTE: Duplicated dependencies shall keep the last one, but list will
    # keep the first one. So we reverse it before and after dedup.
    list(REVERSE ${PREFIX}_DEPENDENCIES)
    list(REMOVE_DUPLICATES ${PREFIX}_DEPENDENCIES)
    list(REVERSE ${PREFIX}_DEPENDENCIES)
  endif()
  foreach (p ${${PREFIX}_DEPENDENCIES})
    foreach(d ${${p}_DEFINITIONS})
      list(APPEND defs ${d})
    endforeach()
    foreach(i ${${p}_INCLUDE_DIRS})
      list(APPEND includes ${i})
    endforeach()
    foreach(l ${${p}_LIBRARIES})
      list(APPEND libs ${l})
    endforeach()
  endforeach()

  # Dedup as much as possible
  if (defs)
    list(REMOVE_DUPLICATES defs)
  endif()
  if (includes)
    list(REMOVE_DUPLICATES includes)
  endif()
  if (libs)
    libfind_dedup_libraries(libs ${libs})
    libfind_remove_dangerious_libs(libs ${libs})
  endif()

  # The following logic just process the errors and do the final outputing.

  # Version checks
  if (found AND findver)
    if (NOT version)
      message(WARNING "The find module for ${PREFIX} does not provide "
        "version information, so we'll just assume that it is OK. Please "
        "fix the module or remove package version requirements to get rid "
        "of this warning.")
    elseif (version VERSION_LESS findver OR
        (exactver AND NOT version VERSION_EQUAL findver))
      set(found FALSE)
      set(version_unsuitable TRUE)
    endif()
  endif()

  # If all-OK, hide all config options, export variables, print status and exit
  if (found)
    foreach (i ${configopts})
      mark_as_advanced(${i})
    endforeach()
    # Calculate ROOT_DIR and adjust ROOT
    if (${PREFIX}_INCLUDE_DIR)
      if (${PREFIX}_INCLUDE_DIR MATCHES ".*/include")
        get_filename_component(tmp ${${PREFIX}_INCLUDE_DIR} DIRECTORY)
      else()
        set(tmp "${${PREFIX}_INCLUDE_DIR}")
      endif()
      set(${PREFIX}_ROOT_DIR ${tmp} PARENT_SCOPE)
      set(${PREFIX}_ROOT ${tmp})
    elseif(${PREFIX}_ROOT)
      set(${PREFIX}_ROOT_DIR ${${PREFIX}_ROOT} PARENT_SCOPE)
    endif()
    # Set other required variables
    set (${PREFIX}_DEFINITIONS ${defs} PARENT_SCOPE)
    set (${PREFIX}_INCLUDE_DIRS ${includes} PARENT_SCOPE)
    set (${PREFIX}_LIBRARIES ${libs} PARENT_SCOPE)
    set (${PREFIX}_FOUND TRUE PARENT_SCOPE)
    # Output some information if not quiet
    if (NOT quiet)
      set(msg "Found ${PREFIX}")
      if (DEFINED ${PREFIX}_VERSION)
        set(msg "${msg} ${${PREFIX}_VERSION}")
      endif()
      if (${PREFIX}_ROOT)
        set(msg "${msg} (${${PREFIX}_ROOT})")
      endif()
      message(STATUS "${msg}")
      if (LIBFIND_DEBUG)
        message(STATUS "  ${PREFIX}_DEPENDENCIES=${${PREFIX}_DEPENDENCIES}")
        message(STATUS "  ${PREFIX}_DEFINITIONS=${defs}")
        message(STATUS "  ${PREFIX}_INCLUDE_DIRS=${includes}")
        message(STATUS "  ${PREFIX}_LIBRARIES=${libs}")
        message(STATUS "  ${PREFIX}_INCLUDE_OPTS=${includeopts}")
        message(STATUS "  ${PREFIX}_LIBRARY_OPTS=${libraryopts}")
      endif()
    endif()
    return()
  else()
    set (${PREFIX}_FOUND FALSE PARENT_SCOPE)
  endif()

  # Format messages for debug info and the type of error
  unset(msg)
  set(vars "Relevant CMake configuration variables:\n")
  foreach (i ${configopts})
    mark_as_advanced(CLEAR ${i})
    set(val ${${i}})
    if ("${val}" STREQUAL "${i}-NOTFOUND")
      set (val "<not found>")
    elseif (val AND NOT EXISTS ${val})
      set (val "${val}  (does not exist)")
    else()
      set(some_files TRUE)
    endif()
    set(vars "${vars}  ${i}=${val}\n")
  endforeach()
  string(CONCAT vars "${vars}You may use CMake GUI, cmake -D or ccmake to "
    "modify the values. Delete CMakeCache.txt to discard all values and "
    "force full re-detection if necessary.\n")
  if (version_unsuitable)
    set(msg "${PREFIX} ${${PREFIX}_VERSION} was found but")
    if (exactver)
      set(msg "${msg} only version ${findver} is acceptable.")
    else()
      set(msg "${msg} version ${findver} is the minimum requirement.")
    endif()
  else()
    if (missing_headers)
      string(CONCAT msg "We could not find development headers for "
        "${PREFIX}. Do you have the necessary dev package installed?")
    elseif (some_files)
      string(CONCAT msg "We only found some files of ${PREFIX}, not all of "
        "them. Perhaps your installation is incomplete or maybe we just "
        "didn't look in the right place?")
      if(findver)
        string(CONCAT msg "${msg} This could also be caused by incompatible "
          "version (if it helps, at least ${PREFIX} ${findver} should work).")
      endif()
    else()
      set(msg "We were unable to find package ${PREFIX}.")
    endif()
  endif()

  # Fatal error out if REQUIRED
  if (required)
    set(msg "REQUIRED PACKAGE NOT FOUND\n${msg}")
    message(FATAL_ERROR "${msg}\n${vars}")
  endif()
endfunction()
