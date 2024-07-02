/*! \file ops.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-01-07
 *  \brief Various functors for unary and binary operations. Can be used for
    parallel combine-reduce operations or other places requiring a functor.
    (OpenFOAM)
 */

#ifndef NUMERICS_TENSOR_OPS_HPP
#define NUMERICS_TENSOR_OPS_HPP

namespace HSF {
/*! Assignment operation taking two parameters, returning void.
 *  Alters the value of the first parameter.
 *  Eg, plusEqOp for (x += y)
 */
#define EqOp(opName, op)                              \
                                                      \
  template <class T1, class T2>                       \
  struct opName##Op2 {                                \
    void operator()(T1& x, const T2& y) const { op; } \
  };                                                  \
                                                      \
  template <class T>                                  \
  struct opName##Op {                                 \
    void operator()(T& x, const T& y) const { op; }   \
  };

EqOp(eq, x = y)
EqOp(plusEq, x += y)
EqOp(minusEq, x -= y)
EqOp(multiplyEq, x *= y)
EqOp(divideEq, x /= y)
EqOp(eqSqr, x = math::sqr(y))
EqOp(eqMag, x = math::mag(y))
EqOp(eqMagSqr, x = math::magSqr(y))
EqOp(plusEqMagSqr, x += math::magSqr(y))
EqOp(maxEq, x = math::max(x, y))
EqOp(minEq, x = math::min(x, y))
EqOp(minMagSqrEq, x = (math::magSqr(x) <= math::magSqr(y) ? x : y))
EqOp(maxMagSqrEq, x = (math::magSqr(x) >= math::magSqr(y) ? x : y))
EqOp(andEq, x = (x && y))
EqOp(orEq, x = (x || y))
EqOp(xorEq, x = (x != y))
EqOp(bitAndEq, x = (x & y))
EqOp(bitOrEq, x = (x | y))
EqOp(bitXorEq, x = (x ^ y))
EqOp(eqMinus, x = -y)
EqOp(nopEq, (void)x)

#undef EqOp

// Operation taking two parameters, returning the first type.
// Neither parameter is altered.
//     Eg, plusOp for (x + y)

#define Op(opName, op)                                           \
                                                                 \
  template <class T, class T1, class T2>                         \
  struct opName##Op3 {                                           \
    T operator()(const T1& x, const T2& y) const { return op; }  \
  };                                                             \
                                                                 \
  template <class T1, class T2>                                  \
  struct opName##Op2 {                                           \
    T1 operator()(const T1& x, const T2& y) const { return op; } \
  };                                                             \
                                                                 \
  template <class T>                                             \
  struct opName##Op {                                            \
    T operator()(const T& x, const T& y) const { return op; }    \
  };

Op(sum, x + y)
Op(plus, x + y)
Op(minus, x - y)
Op(multiply, x* y)
Op(divide, x / y)
Op(cmptMultiply, math::cmptMultiply(x, y))
Op(cmptPow, math::cmptPow(x, y))
Op(cmptDivide, math::cmptDivide(x, y))
Op(stabilise, math::stabilise(x, y))
Op(max, math::max(x, y))
Op(min, math::min(x, y))
Op(minMagSqr, (math::magSqr(x) <= math::magSqr(y) ? x : y))
Op(maxMagSqr, (math::magSqr(x) >= math::magSqr(y) ? x : y))
Op(minMod, math::minMod(x, y))

Op(bitAnd, (x & y))
Op(bitOr, (x | y))
Op(bitXor, (x ^ y))

#undef Op

/*! Operations taking two parameters (unaltered), returning bool */
#define BoolOp(opName, op)                                         \
                                                                   \
  template <class T1, class T2>                                    \
  struct opName##Op2 {                                             \
    bool operator()(const T1& x, const T2& y) const { return op; } \
  };                                                               \
                                                                   \
  template <class T>                                               \
  struct opName##Op {                                              \
    bool operator()(const T& x, const T& y) const { return op; }   \
  };

/*! Operations taking one parameter, returning bool.
 * The comparison value is defined during construction
 */
#define Bool1Op(opName, op)                          \
                                                     \
  template <class T>                                 \
  struct opName##Op1 {                               \
    const T& value;                                  \
                                                     \
    opName##Op1(const T& v) : value(v) {}            \
                                                     \
    bool operator()(const T& x) const { return op; } \
  };

BoolOp(and, x&& y)
BoolOp(or, x || y)
BoolOp(xor, (!x) != (!y))  // With forced bool context
BoolOp(equal, x == y)
BoolOp(notEqual, x != y)
BoolOp(less, x < y)
BoolOp(lessEq, x <= y)
BoolOp(greater, x > y)
BoolOp(greaterEq, x >= y)

Bool1Op(equal, x == value)
Bool1Op(notEqual, x != value)
Bool1Op(less, x < value)
Bool1Op(lessEq, x <= value)
Bool1Op(greater, x > value)
Bool1Op(greaterEq, x >= value)

#undef BoolOp
#undef Bool1Op

} /* End namespace HSF */

#endif /* end of include guard: NUMERICS_TENSOR_OPS_HPP */
