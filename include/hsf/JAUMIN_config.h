
#ifndef included_JAUMIN_config
#define included_JAUMIN_config

#ifndef DEBUG_OMP
//#define DEBUG_OMP
#endif

#ifndef HAVE_BOOL

#endif

#ifndef HAVE_CHAR

#endif

#ifndef HAVE_FLOAT

#endif

#ifndef HAVE_DCOMPLEX
#define HAVE_DCOMPLEX 1
#endif

#define HAVE_MULTIBLOCK

#ifndef HOST_OS_IS_SOLARIS

#endif

#ifndef _POSIX_SOURCE

#endif

#ifndef _ALL_SOURCE

#endif

#ifndef _POWER

#endif

#if defined(__SUNPRO_CC) || defined(_MSC_VER)
#define LACKS_BOOL_VOID_RESOLUTION
#endif

#if defined(_MSC_VER)
#define LACKS_PROPER_MEMORY_HANDLER 1
#endif

#ifndef LACKS_PROPER_XDR_HEADER
#define LACKS_PROPER_XDR_HEADER 1
#endif

#if defined(__xlC__)
#define REQUIRES_CMATH 1
#endif

#ifndef LACKS_NAMESPACE

#endif

#ifndef LACKS_TEMPLATE_COMPLEX

#endif

#define STL_SSTREAM_HEADER_FILE <sstream>
#define LACKS_STRSTREAM

#ifndef IOMANIP_HEADER_FILE
#define IOMANIP_HEADER_FILE <iomanip>
#endif
#ifndef IOMANIP_HEADER_IS_BROKEN

#endif

#ifndef IOSTREAM_HEADER_FILE
#define IOSTREAM_HEADER_FILE <iostream>
#endif
#ifndef IOSTREAM_HEADER_IS_BROKEN

#endif

#ifndef LACKS_IOMANIP_LEFT

#endif

#ifndef INCLUDE_TEMPLATE_IMPLEMENTATION

#endif

#ifndef LACKS_MEMBER_FUNCTION_SPECIALIZATION

#endif

#ifndef LACKS_STATIC_DATA_INSTANTIATION

#endif

#ifndef LACKS_STANDARD_STATIC_DATA_SPECIALIZATION

#endif

#ifndef HAVE_ISNAN_TEMPLATE

#endif

#ifndef LACKS_EXCEPTION_HANDLING

#endif

#ifndef HAVE_PRAGMA_STATIC_DATA_SPECIALIZATION

#endif

#ifndef HAVE_SPECIAL_COMPLEX_OSTREAM_INSTANTIATION

#endif

#ifndef HAVE_SPECIAL_STRING_OSTREAM_INSTANTIATION

#endif

#ifndef RESTRICT_IS_BROKEN

#endif

#ifndef NAMESPACE_IS_BROKEN

#endif

#ifndef HAVE_EXCEPTION_HANDLING
#define HAVE_EXCEPTION_HANDLING 1
#endif

#ifndef NAN_IS_BROKEN
#define NAN_IS_BROKEN 1
#endif

#ifndef FLT_SNAN_IS_BROKEN
#define FLT_SNAN_IS_BROKEN 1
#endif

#ifndef DBL_SNAN_IS_BROKEN
#define DBL_SNAN_IS_BROKEN 1
#endif

#ifndef FLT_MAX_IS_BROKEN

#endif

#ifndef DBL_MAX_IS_BROKEN

#endif

#ifndef OSTRINGSTREAM_TYPE_IS_BROKEN

#endif

#ifndef OSTRSTREAM_TYPE_IS_BROKEN
#define OSTRSTREAM_TYPE_IS_BROKEN 1
#endif

#ifndef LACKS_X11
#define LACKS_X11 1
#endif

#ifndef BOOST_DISABLE_THREADS

#endif


#ifndef HAVE_XDR

#endif

#ifndef HAVE_MPI
#define HAVE_MPI 1
#endif

#ifdef HAVE_MPI
#define MPICH_IGNORE_CXX_SEEK
#endif

#ifndef HAVE_HDF5
#define HAVE_HDF5 1
#endif


#ifndef HAVE_JXPAMG
/* #undef HAVE_JXPAMG */
#endif

#ifndef HAVE_SUNDIALS
/* #undef HAVE_SUNDIALS */
#endif

#ifndef HAVE_HYPRE
/* #undef HAVE_HYPRE */
#endif

#ifndef HAVE_PETSC
/* #undef HAVE_PETSC */
#endif

#ifndef HAVE_JADLIB
#define HAVE_JADLIB 1
#endif

#ifndef HAVE_METIS
#define HAVE_METIS 1
#endif

#ifndef HAVE_PARMETIS
#define HAVE_PARMETIS 1
#endif

#ifndef HAVE_ZOLTAN
#define HAVE_ZOLTAN 1
#endif

#ifndef HAVE_JLICENCE
/* #undef HAVE_JLICENCE */
#endif

#ifndef JAUMIN_VERSION
#define JAUMIN_VERSION "1.9.0"
#endif

#ifndef HYPRE_SEQUENTIAL

#endif

#ifndef MAX_NPROCS
#define MAX_NPROCS 512
#endif

#ifndef HAVE_VAMPIR

#endif

#ifndef HAVE_TAU

#endif

#ifndef DEBUG_NO_INLINE

#endif

#ifndef DEBUG_TRACE_CALLS

#endif

#ifndef DEBUG_CHECK_ASSERTIONS
#define DEBUG_CHECK_ASSERTIONS 1
#endif

#if 0
#ifdef NDEBUG
// release version
#undef DEBUG_CHECK_ASSERTIONS
#else
// debug version
#define DEBUG_CHECK_ASSERTIONS 1
#endif
#endif

//#ifndef DEBUG_RESTART
//#define DEBUG_RESTART
//#endif

#ifndef DEBUG_INITIALIZE_UNDEFINED

#endif

#ifndef DEBUG_TIMES
#define DEBUG_TIMES 1
#endif

#ifdef _MSC_VER

#define LACKS_NAMESPACE_IN_DECLARE

#define drand48() ((double)rand() / (double)RAND_MAX)
#define mrand48() (rand())

#define isnan _isnan

#endif

#define INCLUDE_DEPRACATED 1

#endif
#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#undef INCLUDE_TEMPLATE_IMPLEMENTATION
#endif

#ifndef NUM_FILL_TYPE
#define NUM_FILL_TYPE 3
#endif

#define H5_USE_16_API
