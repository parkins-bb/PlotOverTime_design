/*! \file scalar.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-01-06
 *  \brief scalar type main header
 */

#ifndef TENSOR_SCALAR_SCALAR_HPP
#define TENSOR_SCALAR_SCALAR_HPP

#include "utilityType.h"
// Don't use cmath
#include <math.h>

#include <cstdint>

namespace HSF {
namespace math {

/*! Largest and smallest scalar values allowed in certain parts of the code.
 * See std::numeric_limits max(), min(), epsilon() */
#if defined(SCALAR_FLOAT32)
const float GREAT = 1.0e+6;
const float ROOTGREAT = 1.0e+3;
const float VGREAT = 1.0e+37;
const float ROOTVGREAT = 1.0e+18;
const float SMALL = 1.0e-6;
const float ROOTSMALL = 1.0e-3;
const float VSMALL = 1.0e-37;
const float ROOTVSMALL = 1.0e-18;

#define Scalar float
#define ScalarVGREAT VGREAT
#define ScalarVSMALL VSMALL
#define ScalarROOTVGREAT ROOTVGREAT
#define ScalarROOTVSMALL ROOTVSMALL
/*! float 类型常用函数，参考math.h标准库 */
#define transFunc(func) \
  inline Scalar func(const Scalar s) { return ::func##f(s); }

#else
const double GREAT = 1.0e+15;
const double ROOTGREAT = 3.0e+8;
const double VGREAT = 1.0e+300;
const double ROOTVGREAT = 1.0e+150;
const double SMALL = 1.0e-15;
const double ROOTSMALL = 3.0e-8;
const double VSMALL = 1.0e-300;
const double ROOTVSMALL = 1.0e-150;

#define Scalar double
#define ScalarVGREAT VGREAT
#define ScalarVSMALL VSMALL
#define ScalarROOTVGREAT ROOTVGREAT
#define ScalarROOTVSMALL ROOTVSMALL
/*! double 类型常用函数，参考math.h标准库 */
#define transFunc(func) \
  inline Scalar func(const Scalar s) { return ::func(s); }

#endif

}  // namespace math
} /* End namespace HSF */

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
#include "Scalar.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
#undef Scalar
#undef ScalarVGREAT
#undef ScalarVSMALL
#undef ScalarROOTVGREAT
#undef ScalarROOTVSMALL
#undef transFunc

namespace HSF {
namespace math {

/*! 比较大小 */
#define MAXMIN(retType, type1, type2)                  \
                                                       \
  inline retType min(const type1 s1, const type2 s2) { \
    return (s1 < s2) ? s1 : s2;                        \
  }                                                    \
                                                       \
  inline retType max(const type1 s1, const type2 s2) { \
    return (s2 < s1) ? s1 : s2;                        \
  }

MAXMIN(int8_t, int8_t, int8_t)
MAXMIN(int16_t, int16_t, int16_t)
MAXMIN(int32_t, int32_t, int32_t)
MAXMIN(int64_t, int64_t, int32_t)
MAXMIN(int64_t, int32_t, int64_t)
MAXMIN(int64_t, int64_t, int64_t)

/*! 浮点型比较大小及指数函数，返回浮点型 */
#define MAXMINPOW(retType, type1, type2)                   \
  MAXMIN(retType, type1, type2)                            \
  inline double pow(const type1 base, const type2 expon) { \
    return ::pow(double(base), double(expon));             \
  }

MAXMINPOW(double, double, double)
MAXMINPOW(double, double, float)
MAXMINPOW(double, float, double)
MAXMINPOW(double, double, int)
MAXMINPOW(double, int, double)
MAXMINPOW(double, double, long)
MAXMINPOW(double, long, double)
MAXMINPOW(float, float, float)
MAXMINPOW(float, float, int)
MAXMINPOW(float, int, float)
MAXMINPOW(float, float, long)
MAXMINPOW(float, long, float)

#if defined(LABEL_INT64)
MAXMINPOW(double, double, int64_t)
MAXMINPOW(double, int64_t, double)
MAXMINPOW(float, float, int64_t)
MAXMINPOW(float, int64_t, float)
#endif

#undef MAXMIN
#undef MAXMINPOW

/*! Type narrowing from double to float
 *  \param[in] double
 *  \return float
 */
inline float narrowFloat(const double val) {
  static const float floatScalarVGREAT = 1.0e+37;
  static const float floatScalarVSMALL = 1.0e-37;
  return ((val <= -floatScalarVGREAT)  ? -floatScalarVGREAT
          : (val >= floatScalarVGREAT) ? floatScalarVGREAT
          : (val > -floatScalarVSMALL && val < floatScalarVSMALL)
              ? 0
              : static_cast<float>(val));
}

}  // namespace math
} /* End namespace HSF */

#endif /* end of include guard: TENSOR_SCALAR_SCALAR_HPP */
