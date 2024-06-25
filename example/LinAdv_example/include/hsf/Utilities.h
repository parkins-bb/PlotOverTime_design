//
// 文件名:        Utilities.h
// 软件包:     JAUMIN toolbox
// 版权  :	(c) 2004-2010 北京应用物理与计算数学研究所
// 版本号:    $Revision: 257 $
// 修改  :    $Date: 2012-02-16 12:55:07 +0800 (四, 2012-02-16) $
// 描述  : 内置函数类, 封装一些常用的简单函数.
//

#ifndef included_tbox_Utilities
#define included_tbox_Utilities

#include <ostream>
#ifndef included_JAUMIN_config
#include "JAUMIN_config.h"
#endif

#ifndef included_tbox_Complex
#include "Complex.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <iostream>

#ifndef included_String
#include <string>
using namespace std;
#define included_String
#endif

#ifndef included_sys_types
#include <sys/types.h>
#define included_sys_types
#endif

#ifndef included_sys_stat
#include <sys/stat.h>
#define included_sys_stat
#endif

#include <unistd.h>

#include "mpi.h"

namespace HSF {
namespace tbox {

#ifdef _MSC_VER
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
typedef int mode_t;
#define S_ISDIR(m) (((m)&S_IFMT) == S_IFDIR)
#define S_IRUSR 0
#define S_IWUSR 0
#define S_IXUSR 0
#endif

/**
 * 定义一个空的赋值语句, 以避免编译器发出某些警告信息.
 */
#ifdef __INSURE__
#define NULL_STATEMENT \
  if (0) int nullstatement = 0
#else
#define NULL_STATEMENT
#endif

/**
 * 在一个实际并不执行的表达式中使用变量, 以避免GNU编译器对未使用的变量发出警告.
 */
#define NULL_USE(variable)            \
  do {                                \
    if (0) {                          \
      char *temp = (char *)&variable; \
      temp++;                         \
    }                                 \
  } while (0)

/**
 * 从C++源代码中抛出错误异常, 以及相应的文件名和行号.
 * 该宏参数可能是一些标准的输出表达式.
 */
#ifndef LACKS_SSTREAM
#define TBOX_ERROR(X)                                                    \
  do {                                                                   \
    ostringstream tboxos;                                                \
    tboxos << X << ends;                                                 \
    std::cout << tboxos.str().c_str() << ENDL;                           \
    std::cout << "file: " << __FILE__ << ", line: " << __LINE__ << ENDL; \
    MPI_Abort(COMM::getGlobalComm().getMpiComm(), 1);                    \
    exit(1);                                                             \
  } while (0)
#else
#define TBOX_ERROR(X)    \
  do {                   \
    ostrstream tboxos;   \
    tboxos << X << ends; \
  } while (0)
#endif

/**
 * 在不终止程序的情况下, 输出警告信息, 源代码文件名和行号.
 */
#ifndef LACKS_SSTREAM
#define TBOX_WARNING(X)                                                  \
  do {                                                                   \
    ostringstream tboxos;                                                \
    tboxos << X << ends;                                                 \
    std::cout << tboxos.str().c_str() << ENDL;                           \
    std::cout << "file: " << __FILE__ << ", line: " << __LINE__ << ENDL; \
  } while (0)
#else
#define TBOX_WARNING(X)  \
  do {                   \
    ostrstream tboxos;   \
    tboxos << X << ends; \
  } while (0)
#endif

/**
 * 如果给定的表达式为false, 那么从C++源代码中抛出一个错误异常, 文件名和行号.
 */
#ifndef LACKS_SSTREAM
#define TBOX_ASSERT(EXP)                                                   \
  do {                                                                     \
    if (!(EXP)) {                                                          \
      ostringstream tboxos;                                                \
      tboxos << "Failed assertion: " << ends;                              \
      std::cout << tboxos.str().c_str() << ENDL;                           \
      std::cout << "file: " << __FILE__ << ", line: " << __LINE__ << ENDL; \
    }                                                                      \
  } while (0)
#else
#define TBOX_ASSERT(EXP)                      \
  do {                                        \
    if (!(EXP)) {                             \
      ostrstream tboxos;                      \
      tboxos << "Failed assertion: " << ends; \
    }                                         \
  } while (0)
#endif

#define TBOX_GDB_BREAKPOINT()                      \
  do {                                             \
    int pid = getpid();                            \
    int nproc, myrank;                             \
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);         \
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);        \
    for (int i = 0; i < nproc; i++) {              \
      if (myrank == i) {                           \
        char str[100];                             \
        sprintf(str,                               \
                "pid of proc %d is %d,"            \
                " use 'gdb attach %d' to debug\n", \
                i, pid, pid);                      \
        cout << str;                               \
      }                                            \
      MPI_Barrier(MPI_COMM_WORLD);                 \
    }                                              \
    if (myrank == 0) {                             \
      cout << "Press Enter key to continue";       \
      getchar();                                   \
    }                                              \
    MPI_Barrier(MPI_COMM_WORLD);                   \
  } while (0)

#define CHECK_ENV(env_name) \
  (getenv(env_name) != 0 && atoi(getenv(env_name)) > 0)

}  // namespace tbox
}  // namespace HSF

#endif
