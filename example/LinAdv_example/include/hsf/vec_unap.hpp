/*! \file vec_unap.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-09-27
 *  \brief unap vector wrapper
 */
#ifndef NUMERICS_VECTOR_VEC_UNAP_HPP
#define NUMERICS_VECTOR_VEC_UNAP_HPP

#include "unapVector.hpp"
#include "hsfVector.hpp"

namespace HSF {
template <typename T>
class UnapVector : public Vector<T> {
 private:
  bool own_;
  /* actual object */
  UNAP::Vector<T> *x_;
  friend VecOp<VecType::UNAP>;

 public:
  UnapVector() : own_(false), x_(NULL) {}
  UnapVector(const label length)
      : Vector<T>(), own_(true), x_(new UNAP::Vector<T>(length)) {}
  /*! reUse: deep copy val or not,
   *  dontDel: don't delete data when deconstruct
   */
  UnapVector(const T *val, const label &length, const bool reUse = false,
             const int dontDel = 0)
      : Vector<T>(),
        own_(true),
        x_(new UNAP::Vector<T>(val, length, reUse, dontDel)) {}
  ~UnapVector() {
    if (own_) delete x_;
  }

  explicit UnapVector(UNAP::Vector<T> *y) : Vector<T>(), own_(false), x_(y) {}

  /*! Typecasting to unap::vector */
  operator UNAP::Vector<T> *() const { return x_; }
  virtual VecType getType() const { return VecType::UNAP; }
};

/*! * * * * * * * * * * * * * * * VecOp * * * * * * * * * * * * * */
/*! UNAP */
template <>
class VecOp<VecType::UNAP> {
 public:
  template <typename T>
  static void axpy(Vector<T> *y, T a, Vector<T> *x) {
    UNAP::axpy(*dynamic_cast<UnapVector<T> *>(y)->x_, a,
               *dynamic_cast<UnapVector<T> *>(x)->x_);
  }
  template <typename T>
  static void aypx(Vector<T> *y, T a, Vector<T> *x) {
    UNAP::aypx(*dynamic_cast<UnapVector<T> *>(y)->x_, a,
               *dynamic_cast<UnapVector<T> *>(x)->x_);
  }
  template <typename T>
  static void waxpy(Vector<T> *w, T a, Vector<T> *x, Vector<T> *y) {
    UNAP::waxpy(*dynamic_cast<UnapVector<T> *>(w)->x_, a,
                *dynamic_cast<UnapVector<T> *>(x)->x_,
                *dynamic_cast<UnapVector<T> *>(y)->x_);
  }
  template <typename T>
  static void axpby(Vector<T> *y, T a, T b, Vector<T> *x) {
    UNAP::axpby(*dynamic_cast<UnapVector<T> *>(y)->x_, a, b,
                *dynamic_cast<UnapVector<T> *>(x)->x_);
  }
  template <typename T>
  static void axpbypz(Vector<T> *y, T a, T b, Vector<T> *x, Vector<T> *z) {
    UNAP::axpbypz(*dynamic_cast<UnapVector<T> *>(y)->x_, a, b,
                  *dynamic_cast<UnapVector<T> *>(x)->x_,
                  *dynamic_cast<UnapVector<T> *>(z)->x_);
  }
  template <typename T>
  static void scale(Vector<T> *x, T a) {
    UNAP::scale(*dynamic_cast<UnapVector<T> *>(x)->x_, a);
  }
  template <typename T>
  static void dot(Vector<T> *x, Vector<T> *y, T &r) {
    r = UNAP::dot(*dynamic_cast<UnapVector<T> *>(x)->x_,
                  *dynamic_cast<UnapVector<T> *>(y)->x_);
  }
  template <typename T>
  static void normlp(Vector<T> *x, scalar lp, T &r) {
    r = UNAP::normlp(*dynamic_cast<UnapVector<T> *>(x)->x_, lp);
  }
  template <typename T>
  static void sum(Vector<T> *x, T &r) {
    UNAP::sum(*dynamic_cast<UnapVector<T> *>(x)->x_, r);
  }
  template <typename T>
  static void copy(Vector<T> *x, Vector<T> *y) {
    UNAP::copy(*dynamic_cast<UnapVector<T> *>(x)->x_,
               *dynamic_cast<UnapVector<T> *>(y)->x_);
  }
  template <typename T>
  static void swap(Vector<T> *x, Vector<T> *y) {
    UNAP::swap(*dynamic_cast<UnapVector<T> *>(x)->x_,
               *dynamic_cast<UnapVector<T> *>(y)->x_);
  }
  template <typename T>
  static void pointwiseMult(Vector<T> *w, Vector<T> *x, Vector<T> *y) {
    UNAP::pointwiseMul(*dynamic_cast<UnapVector<T> *>(w)->x_,
                       *dynamic_cast<UnapVector<T> *>(x)->x_,
                       *dynamic_cast<UnapVector<T> *>(y)->x_);
  }
  template <typename T>
  static void pointwiseDiv(Vector<T> *w, Vector<T> *x, Vector<T> *y) {
    UNAP::pointwiseDiv(*dynamic_cast<UnapVector<T> *>(w)->x_,
                       *dynamic_cast<UnapVector<T> *>(x)->x_,
                       *dynamic_cast<UnapVector<T> *>(y)->x_);
  }
  template <typename T>
  static void vecMax(Vector<T> *x, T &r) {
    UNAP::max(*dynamic_cast<UnapVector<T> *>(x)->x_, r);
  }
  template <typename T>
  static void vecMin(Vector<T> *x, T &r) {
    UNAP::min(*dynamic_cast<UnapVector<T> *>(x)->x_, r);
  }
  template <typename T>
  static void reciprocal(Vector<T> *x) {
    UNAP::reciprocal(*dynamic_cast<UnapVector<T> *>(x)->x_);
  }
  template <typename T>
  static void shift(Vector<T> *x, T s) {
    UNAP::shift(*dynamic_cast<UnapVector<T> *>(x)->x_, s);
  }
  template <typename T>
  static void vecSet(Vector<T> *x, T s) {
    UNAP::setValue(*dynamic_cast<UnapVector<T> *>(x)->x_, s);
  }
};

} /* End namespace HSF */
#endif /* end of include guard: NUMERICS_VECTOR_VEC_UNAP_HPP */
