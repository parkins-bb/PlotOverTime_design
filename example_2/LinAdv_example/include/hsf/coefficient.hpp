/*! \file coefficient.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-08
 *  \brief Coefficient class
 */

#ifndef REGION_FIELD_COEFFICIENT_HPP
#define REGION_FIELD_COEFFICIENT_HPP

#include <functional>

#include "tensor.hpp"
#include "utilities.h"
namespace HSF {
template <typename T>
class Field;

/*! @brief Base class Coefficients that optionally depend on space and time.
    \note elemAttr: use to find element attribute key, the key may either be
   domain attribute numbers or boundary attribute numbers or element ID (in
   FieldCoefficient)
 */
template <typename T>
class Coefficient {
 protected:
  scalar time_;

 public:
  Coefficient() : time_(0.) {}
  /// Set the time for time dependent coefficients
  virtual void setTime(scalar t) { time_ = t; }
  /// Get the time for time dependent coefficients
  scalar getTime() { return time_; }
  /*! @brief Evaluate the coefficient  at point ip in element with elemAttr. */
  virtual T eval(const Tensor1<scalar> &ip, label elemAttr = -1) = 0;
  /*! @brief Evaluate the coefficient at the point ip, at time  t. */
  T eval(const Tensor1<scalar> &ip, scalar t, label elemAttr = -1) {
    setTime(t);
    return eval(ip, elemAttr);
  }
  virtual ~Coefficient() {}
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
///  Constant across space and time
template <typename T>
class ConstantCoefficient : public Coefficient<T> {
 public:
  T constant_;

  /// c is value of constant function
  explicit ConstantCoefficient(T c = 1.0) { constant_ = c; }

  /// Evaluate the coefficient at point ip in elment elemAttr.
  virtual T eval(const Tensor1<scalar> &ip, label elemAttr = -1) {
    return (constant_);
  }
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
/** @brief A piecewise constant coefficient */
template <typename T>
class PWConstCoefficient : public Coefficient<T> {
 private:
  std::vector<T> constants_;

 public:
  /// Constructs a piecewise constant coefficient in num subdomains
  explicit PWConstCoefficient(int num = 0) : constants_(num) {}
  /*! c should be a vector defined by attributes, so for region with
      attribute i, c[i] is the coefficient in that region */
  PWConstCoefficient(const std::vector<T> &c) { constants_ = c; }

  /// Update the constants_ with vector c.
  void updateConstants(const std::vector<T> &c) { constants_ = c; }
  /// Return a reference to the i-th constant
  T &operator()(int i) { return constants_[i]; }
  /// Set the constants_ for all attributes to constant c.
  void operator=(T c) {
    for (T &i : constants_) i = c;
  }
  /// Returns the number of constants_ representing different attributes.
  int getNConst() { return constants_.size(); }
  /// Evaluate the coefficient.
  virtual T eval(const Tensor1<scalar> &ip, label elemAttr = -1);
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
/*! @brief A piecewise coefficient with the pieces keyed off the element
 * attribute numbers. A value of zero will be returned for missing attribute
 */
template <typename T>
class PWCoefficient : public Coefficient<T> {
 private:
  /*! Ownership of the Coefficient objects will NOT be transferred */
  Table<int, Coefficient<T> *> pieces_;
  /*! Any missing attributes or NULL coefficient pointers will result in a value
   * of zero being returned for that attribute. */
  void initMap(const std::vector<int> &attr, const std::vector<Coefficient<T> *> &coefs);

 public:
  /// Constructs a piecewise coefficient
  explicit PWCoefficient() {}
  /// Construct the coefficient using arrays describing the pieces_
  PWCoefficient(const std::vector<int> &attr, const std::vector<Coefficient<T> *> &coefs) {
    initMap(attr, coefs);
  }

  /// Set the time for time dependent coefficients
  virtual void setTime(scalar t);
  /// Replace a set of coefficients
  void updateCoefficients(const std::vector<int> &attr,
                          const std::vector<Coefficient<T> *> &coefs) {
    initMap(attr, coefs);
  }
  /// Replace a single Coefficient for a particular attribute
  void updateCoefficient(int attr, Coefficient<T> &coef) {
    pieces_[attr] = &coef;
  }
  /// Remove a single Coefficient for a particular attribute
  void zeroCoefficient(int attr) { pieces_.erase(attr); }
  /// Evaluate the coefficient.
  virtual T eval(const Tensor1<scalar> &ip, label elemAttr = -1);
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
/// A general function coefficient
template <typename T>
class FunctionCoefficient : public Coefficient<T> {
 protected:
  std::function<T(const Tensor1<scalar> &)> func_;
  std::function<T(const Tensor1<scalar> &, scalar)> tdFunc_;

 public:
  /// Define a time-independent coefficient from a std function
  /** \param F time-independent std::function */
  FunctionCoefficient(std::function<T(const Tensor1<scalar> &)> F)
      : func_(std::move(F)) {}
  /// Define a time-dependent coefficient from a std function
  /** \param TDF time-dependent function */
  FunctionCoefficient(std::function<T(const Tensor1<scalar> &, scalar)> TDF)
      : tdFunc_(std::move(TDF)) {}
  /// Evaluate the coefficient at ip.
  virtual T eval(const Tensor1<scalar> &ip, label elemAttr = -1);
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
/** @brief A coefficient that depends on 1 or 2 parent coefficients and a
    transformation rule represented by a C-function.
    \f$ C(x,t) = T(Q1(x,t)) \f$ or \f$ C(x,t) = T(Q1(x,t), Q2(x,t)) \f$
    where T is the transformation rule, and Q1/Q2 are the parent coefficients.*/
template <typename T>
class TransformedCoefficient : public Coefficient<T> {
 private:
  Coefficient<T> *q1_;
  Coefficient<T> *q2_;
  T (*transform1_)(T);
  T (*transform2_)(T, T);

 public:
  TransformedCoefficient(Coefficient<T> *q, T (*F)(T))
      : q1_(q), transform1_(F) {
    q2_ = 0;
    transform2_ = 0;
  }
  TransformedCoefficient(Coefficient<T> *q1, Coefficient<T> *q2, T (*F)(T, T))
      : q1_(q1), q2_(q2), transform2_(F) {
    transform1_ = 0;
  }

  /// Set the time for internally stored coefficients
  void setTime(scalar t);
  /// Evaluate the coefficient at ip.
  virtual T eval(const Tensor1<scalar> &ip, label elemAttr = -1);
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
/// Coefficient defined by a Field. This coefficient is mesh dependent.
template <typename T>
class FieldCoefficient : public Coefficient<T> {
 private:
  const Field<T> *f_;

 public:
  FieldCoefficient() : f_(NULL) {}
  /** Construct FieldCoefficient from a given Field */
  FieldCoefficient(const Field<T> *f) : f_(f) {}

  /// Set the internal Field
  void setField(const Field<T> *f) { f_ = f; }
  /// Get the internal Field
  const Field<T> *getField() const { return f_; }
  /// Evaluate the coefficient at ip.
  virtual T eval(const Tensor1<scalar> &ip, label elemID = -1);
};

} /* End namespace HSF */
#endif /* end of include guard: REGION_FIELD_COEFFICIENT_HPP */
