/*! \file zero.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-01-10
 *  \brief A class representing the concept of 0 (zero) that can be used to
 *  avoid manipulating objects known to be \em zero at compile-time.
 */

#ifndef NUMERICS_TENSOR_ZERO_HPP
#define NUMERICS_TENSOR_ZERO_HPP
#include <iostream>

#include "scalar.hpp"

namespace HSF {

/*! 0，用来初始化各阶张量 */
class zero {
 public:
  /*! Default construct */
  zero() {}

  //以下均为隐式类型转换运算符

  /*! Return false (0) for bool */
  operator bool() const { return false; }

  /*! Return 0 for float */
  operator float() const { return 0; }

  /*! Return 0 for double */
  operator double() const { return 0; }
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*! Global zero (0) */
static const zero Zero;

/*! Write to Ostream emits no content */
inline std::ostream& operator<<(std::ostream& os, const zero&) { return os; }

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

/*! 0 + 0 = 0 */
inline const zero operator+(const zero&, const zero&) { return Zero; }

/*! x + 0 = x */
template <class Type>
inline const Type& operator+(const Type& val, const zero&) {
  return val;
}

/*! 0 + x = x */
template <class Type>
inline const Type& operator+(const zero&, const Type& val) {
  return val;
}

/*! 0 - 0 = 0 */
inline const zero operator-(const zero&, const zero&) { return Zero; }

/*! x - 0 = x */
template <class Type>
inline const Type& operator-(const Type& val, const zero&) {
  return val;
}

/*! 0 - x = -x */
template <class Type>
inline const Type operator-(const zero&, const Type& val) {
  return -val;
}

/*! 0 * 0 = 0 */
inline const zero operator*(const zero&, const zero&) { return Zero; }

/*! 0 * x = 0 */
template <class Type>
inline const zero operator*(const Type& val, const zero&) {
  return Zero;
}

/*! x * 0 = 0 */
template <class Type>
inline const zero operator*(const zero&, const Type& val) {
  return Zero;
}

/*! 0 / x = 0 */
template <class Type>
inline const zero operator/(const zero&, const Type& val) {
  return Zero;
}

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

/*! min(0,0)=0 */
inline zero min(const zero&, const zero&) { return Zero; }

/*! min(0,scalar t) */
template <class Type>
inline Type min(const zero&, const Type& t) {
  return min(scalar(0), t);
}

/*! min(scalar t,0) */
template <class Type>
inline Type min(const Type& t, const zero&) {
  return min(t, scalar(0));
}

/*! max(0,0)=0 */
inline zero max(const zero&, const zero&) { return Zero; }

/*! max(0,scalar t) */
template <class Type>
inline Type max(const zero&, const Type& t) {
  return max(scalar(0), t);
}

/*! max(scalar t,0) */
template <class Type>
inline Type max(const Type& t, const zero&) {
  return max(t, scalar(0));
}

} /* End namespace HSF */

#endif /* end of include guard: NUMERICS_TENSOR_ZERO_HPP */
