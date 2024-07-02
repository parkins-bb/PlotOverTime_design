/*! \file vecOpFactory.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-09-28
 *  \brief vector operation factory
 */
#ifndef NUMERICS_VECTOR_VECOP_HPP
#define NUMERICS_VECTOR_VECOP_HPP

#include "utilities.h"
namespace HSF {
template <typename T>
class Vector;

enum class VecType { DEFAULT, HYPRE, UNAP };

#define SWITCHVECOP(opName, ...)                       \
  switch (type) {                                   \
    case VecType::UNAP:                             \
      VecOp<VecType::UNAP>::opName(__VA_ARGS__);    \
      break;                                        \
    case VecType::HYPRE:                            \
      VecOp<VecType::HYPRE>::opName(__VA_ARGS__);   \
      break;                                        \
    case VecType::DEFAULT:                          \
    default:                                        \
      VecOp<VecType::DEFAULT>::opName(__VA_ARGS__); \
      break;                                        \
  }

template <typename T>
class VecOpFactory {
 public:
  static void axpy(VecType type, Vector<T> *y, T a, Vector<T> *x);

  static void aypx(VecType type, Vector<T> *y, T a, Vector<T> *x);

  static void waxpy(VecType type, Vector<T> *w, T a, Vector<T> *x,
                    Vector<T> *y);

  static void axpby(VecType type, Vector<T> *y, T a, T b, Vector<T> *x);

  static void axpbypz(VecType type, Vector<T> *y, T a, T b, Vector<T> *x,
                      Vector<T> *z);

  static void scale(VecType type, Vector<T> *x, T a);

  static void dot(VecType type, Vector<T> *x, Vector<T> *y, T &r);

  static void normlp(VecType type, Vector<T> *x, scalar lp, T &r);

  static void sum(VecType type, Vector<T> *x, T &r);

  static void copy(VecType type, Vector<T> *x, Vector<T> *y);

  static void swap(VecType type, Vector<T> *x, Vector<T> *y);

  static void pointwiseMult(VecType type, Vector<T> *w, Vector<T> *x,
                            Vector<T> *y);

  static void pointwiseDiv(VecType type, Vector<T> *w, Vector<T> *x,
                           Vector<T> *y);

  static void vecMax(VecType type, Vector<T> *x, T &r);

  static void vecMin(VecType type, Vector<T> *x, T &r);

  static void reciprocal(VecType type, Vector<T> *x);

  static void shift(VecType type, Vector<T> *x, T s);

  static void vecSet(VecType type, Vector<T> *x, T s);
};

} /* End namespace HSF */
#endif /* end of include guard: NUMERICS_VECTOR_VECOP_HPP */
