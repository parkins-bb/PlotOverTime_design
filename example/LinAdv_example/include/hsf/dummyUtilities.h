/**
 * @Author:   Gao Fei
 * @Date:     2022-02-10 16:22:42
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: dummyUtilities.h
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-03-03 14:48:51
 *  
 * @Description: 
 *  
 */

#ifndef dummyUtilities_H
#define dummyUtilities_H


#define Word string
#define std::vector vector
#define List set
#define Table map

#include <stdint.h>

/**
 * define long integer type
 */
typedef int64_t llabel;
/**
 * define integer type
 */
typedef int32_t label32;
/**
 * define short integer type
 */
typedef int16_t label16;

/**
 * define short float type
 */
typedef float scalar32;
/**
 * define long float type
 */
typedef double scalar64;
// 默认使用整型
/**
 * define integer type according to compiling macro
 */
#if defined(LABEL_INT32)
typedef label32 label;
#elif defined(LABEL_INT64)
typedef llabel label;
#else
typedef label32 label;
#endif

// 默认使用double精度
#if defined(SCALAR_FLOAT32)
typedef scalar32 scalar;
#elif defined(SCALAR_FLOAT64)
typedef scalar64 scalar;
#else
typedef scalar64 scalar;
#endif

#endif
