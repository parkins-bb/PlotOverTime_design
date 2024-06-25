/*! \file vec_hypre.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-09-27
 *  \brief hypre vector wrapper
 */
#ifndef NUMERICS_VECTOR_VEC_HYPRE_HPP
#define NUMERICS_VECTOR_VEC_HYPRE_HPP

#include "hsfVector.hpp"
// hypre header files
#include "_hypre_parcsr_ls.h"
#include "_hypre_parcsr_mv.h"
#include "seq_mv.h"

namespace HSF {
class HypreVector : public Vector<scalar> {
 private:
  bool own_;
  /* actual object */
  hypre_ParVector *x_;

 public:
  HypreVector() : own_(false), x_(NULL) {}
  /*! 后续有需要加上相应构造和成员函数 */

  ~HypreVector() {
    if (own_) delete x_;
  }
  explicit HypreVector(hypre_ParVector *y)
      : Vector<scalar>(), own_(false), x_(y) {}

  /*! Typecasting to hypre's hypre_ParVector* */
  operator hypre_ParVector *() const { return x_; }
  virtual VecType getType() const { return VecType::HYPRE; }
};

/*! * * * * * * * * * * * * * * * VecOp * * * * * * * * * * * * * */
/*! HYPRE */
template <>
class VecOp<VecType::HYPRE> {
 public:
  template <typename T>
  static void axpy(Vector<T> *y, T a, Vector<T> *x) {}
  template <typename T>
  static void aypx(Vector<T> *y, T a, Vector<T> *x) {}
  template <typename T>
  static void waxpy(Vector<T> *w, T a, Vector<T> *x, Vector<T> *y) {}
  template <typename T>
  static void axpby(Vector<T> *y, T a, T b, Vector<T> *x) {}
  template <typename T>
  static void axpbypz(Vector<T> *y, T a, T b, Vector<T> *x, Vector<T> *z) {}
  template <typename T>
  static void scale(Vector<T> *x, T a) {}
  template <typename T>
  static void dot(Vector<T> *x, Vector<T> *y, T &r) {}
  template <typename T>
  static void normlp(Vector<T> *x, scalar lp, T &r) {}
  template <typename T>
  static void sum(Vector<T> *x, T &r) {}
  template <typename T>
  static void copy(Vector<T> *x, Vector<T> *y) {}
  template <typename T>
  static void swap(Vector<T> *x, Vector<T> *y) {}
  template <typename T>
  static void pointwiseMult(Vector<T> *w, Vector<T> *x, Vector<T> *y) {}
  template <typename T>
  static void pointwiseDiv(Vector<T> *w, Vector<T> *x, Vector<T> *y) {}
  template <typename T>
  static void vecMax(Vector<T> *x, T &r) {}
  template <typename T>
  static void vecMin(Vector<T> *x, T &r) {}
  template <typename T>
  static void reciprocal(Vector<T> *x) {}
  template <typename T>
  static void shift(Vector<T> *x, T s) {}
  template <typename T>
  static void vecSet(Vector<T> *x, T s) {}
};

} /* End namespace HSF */
#endif /* end of include guard: NUMERICS_VECTOR_VEC_HYPRE_HPP */
