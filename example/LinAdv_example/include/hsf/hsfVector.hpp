/*! \file vector.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2013-09-26
 *  \modified 2013-09-28
 *  \brief basic Vector class
 */

#ifndef NUMERICS_VECTOR_VECTOR_HPP
#define NUMERICS_VECTOR_VECTOR_HPP

#include <mpi.h>

#include <cmath>
#include <iostream>

#include "vecOpFactory.hpp"
namespace HSF {

template <class T>
inline void swap(T &a, T &b) {
  T c = a;
  a = b;
  b = c;
}

template <typename T>
class Vector {
 protected:
  T *data_;
  label size_;
  bool own_;

  void deleteData() {
    if (own_) delete[] data_;
  }

 public:
  /*! default constructor */
  Vector() : data_(NULL), size_(0), own_(false) {}

  /*! deep copy constructor */
  Vector(const Vector &v) : data_(new T[v.size_]), size_(v.size_), own_(true) {
    std::copy(v.data_, v.data_ + v.size_, data_);
  }

  /*! move constructor */
  Vector(Vector &&v) { *this = std::move(v); }

  /*! create a length `size` vector */
  explicit Vector(label size) : size_(size), own_(true) { data_ = new T[size]; }

  /*! deep/shallow copy constructor */
  Vector(T *data, label size, bool own = 0)
      : data_(data), size_(size), own_(own) {
    if (own) {
      data_ = new T[size];
      std::copy(data, data + size, data_);
    }
  }
  /*! shallow copy constructor */
  Vector(T *data, label offset, label size)
      : data_(data + offset), size_(size), own_(false) {}
  /*! shallow copy constructor */
  Vector(Vector &base, label offset, label size_)
      : data_(base.data_, offset, size_), size_(size_), own_(false) {}

  /*! deep copy constructor */
  template <label N>
  explicit Vector(const T (&values)[N]) : Vector(N) {
    std::copy(values, values + N, data_);
  }

  /*! Destroys vector. */
  virtual ~Vector() { destroy(); }

  /*! resize, deep copy */
  void setSize(label s) {
    if (s == size_) {
      return;
    }
    if (s < size_) {
      size_ = s;
      return;
    }
    deleteData();
    size_ = s;
    data_ = new T[size_];
    own_ = true;
  }
  void setSize(label s, T *v) {
    setSize(s);
    std::copy(v, v + s, data_);
  }
  void setSize(label s, const Vector &v) { setSize(s, v.data_); }

  /*! change data ptr, not own data */
  void setData(T *d) {
    deleteData();
    data_ = d;
    own_ = false;
  }

  /*! set the Vector data and size, not own data */
  void setDataAndSize(T *d, label s) {
    deleteData();
    data_ = d;
    size_ = s;
    own_ = false;
  }
  /*! copy the Vector data and size, deleting the old data, if owned. */
  void newDataAndSize(T *d, label s) {
    deleteData();
    data_ = new T[s];
    std::copy(d, d + s, data_);
    size_ = s;
    own_ = true;
  }

  /*! reset the Vector to be a reference to a sub-vector of  base. */
  void makeRef(Vector &base, label offset, label size) {
    ASSERT(0 <= offset);
    ASSERT(0 <= size);
    ASSERT(offset + size <= base.size())
    deleteData();
    data_ = base.data() + offset;
    size_ = size;
    own_ = false;
  }
  /*! reset the Vector to be a reference to a sub-vector of base
      without changing its current size. */
  void makeRef(Vector &base, label offset) { makeRef(base, offset, size_); }

  /*! read the Vector data  ownership flag. */
  bool ownsData() const { return own_; }
  /*! destroy a vector */
  void destroy() {
    deleteData();
    data_ = NULL;
    size_ = 0;
    own_ = false;
  }

  /*! returns the size of the vector. */
  label size() const { return size_; }
  T *data() const { return const_cast<T *>((const T *)data_); }
  /*! STL-like begin. */
  T *begin() { return data_; }
  /*! STL-like end. */
  T *end() { return data_ + size_; }
  /*! STL-like begin (const version). */
  const T *begin() const { return data_; }
  /*! STL-like end (const version). */
  const T *end() const { return data_ + size_; }

  /*! access Vector entries using () for 0-based indexing. */
  T &operator()(label i) {
    ASSERT(data_ && i >= 0 && i < size_);
    return data_[i];
  }
  /*! read only access to Vector entries using () for 0-based indexing. */
  const T &operator()(label i) const {
    ASSERT(data_ && i >= 0 && i < size_);
    return data_[i];
  }
  /*! access Vector entries using [] for 0-based indexing. */
  T &operator[](label i) { return (*this)(i); }
  /*! read only access to Vector entries using [] for 0-based indexing. */
  const T &operator[](label i) const { return (*this)(i); }

  /*! dot product with a `T *` array. */
  inline T operator*(const T *) const;
  /*! return the inner-product. */
  inline T operator*(const Vector &v) const;
  /*! copy size() entries from  v. */
  inline Vector &operator=(const T *v);
  /*! copy assignment. */
  inline Vector &operator=(const Vector &v);
  /*! move assignment */
  inline Vector &operator=(Vector &&v);
  /*!  vector = constant. */
  inline Vector &operator=(T value);
  inline Vector &operator*=(T c);
  /*!  (*this)(i) *= v(i) */
  inline Vector &operator*=(const Vector &v);
  inline Vector &operator/=(T c);
  /*!  (*this)(i) /= v(i) */
  inline Vector &operator/=(const Vector &v);
  inline Vector &operator-=(T c);
  inline Vector &operator-=(const Vector &v);
  inline Vector &operator+=(T c);
  inline Vector &operator+=(const Vector &v);

  void setSub(const Vector &v, label offset) {
    ASSERT(v.Size() + offset <= size_);
    const label vs = v.size();
    const T *vp = v.data();
    T *p = data_ + offset;
    for (label i = 0; i < vs; i++) {
      p[i] = vp[i];
    }
  }
  void addSub(const Vector &v, label offset) {
    ASSERT(v.size() + offset <= size_);
    const label vs = v.size();
    const T *vp = v.data();
    T *p = data_ + offset;
    for (label i = 0; i < vs; i++) {
      p[i] += vp[i];
    }
  }
  /*! swap the contents of two Vectors */
  void swap(Vector &other) {
    HSF::swap(data_, other.data_);
    HSF::swap(size_, other.size_);
    HSF::swap(own_, other.own_);
  }

  /*! negative dof values cause the -dof-1 position in elemvect to receive
      the -val in from this Vector. */
  /*! extract entries listed in dofs to the output Vector elemvect. */
  inline void getSubVector(const std::vector<label> &dofs, Vector &elemvect) const;
  /*! extract entries listed in dofs to the output array elem_data. */
  inline void getSubVector(const std::vector<label> &dofs, T *elem_data) const;
  /*! set the entries listed in  dofs to the given  value. */
  inline void setSubVector(const std::vector<label> &dofs, const T value);
  /*! set the entries listed in  dofs to the values given in elemvect */
  inline void setSubVector(const std::vector<label> &dofs, const Vector &elemvect);
  /*! set the entries listed in  dofs to the values given the  array */
  inline void setSubVector(const std::vector<label> &dofs, T *elem_data);
  /*! add elements of the  elemvect Vector to the entries listed in dofs */
  inline void addSubVector(const std::vector<label> &dofs, const Vector &elemvect);
  /*! add elements of the  elem_data array to the entries listed in dofs */
  inline void addSubVector(const std::vector<label> &dofs, T *elem_data);
  /*! add  a times the elements of elemvect to the entries listed in dofs */
  inline void addSubVector(const std::vector<label> &dofs, const T a,
                           const Vector &elemvect);
  /*! set all vector entries NOT in the  dofs std::vector to the given  val. */
  inline void setOtherSubVector(const std::vector<label> &dofs, const T val);

  /*! Returns the l2 norm of the vector. */
  inline scalar norml2() const;
  /*! Returns the l_infinity norm of the vector. */
  inline scalar normlinf() const;
  /*! Returns the l_1 norm of the vector. */
  inline scalar norml1() const;
  /*! Returns the l_p norm of the vector. */
  inline scalar normlp(scalar p) const;
  /*! Returns the maximal element of the vector. */
  inline T max() const;
  /*! Returns the minimal element of the vector. */
  inline T min() const;
  /*! Return the sum of the vector entries */
  inline T sum() const;

  virtual VecType getType() const { return VecType::DEFAULT; }
};

/*!  * * * * * * * * * * * * * "virtual" functions * * * * * * * * * * * * */
/// \brief y=y+a*x
template <typename T>
void axpy(Vector<T> *y, T a, Vector<T> *x) {
  ASSERT(y->getType() == x->getType())
  VecOpFactory<T>::axpy(y->getType(), y, a, x);
}
/// \brief y=a*y+x
template <typename T>
void aypx(Vector<T> *y, T a, Vector<T> *x) {
  ASSERT(y->getType() == x->getType())
  VecOpFactory<T>::aypx(y->getType(), y, a, x);
}

/// \brief w=ax+y
template <typename T>
void waxpy(Vector<T> *w, T a, Vector<T> *x, Vector<T> *y) {
  ASSERT(y->getType() == x->getType() && y->getType() == w->getType())
  VecOpFactory<T>::waxpy(w->getType(), a, x, y);
}

/// \brief y=ax+by
template <typename T>
void axpby(Vector<T> *y, T a, T b, Vector<T> *x) {
  ASSERT(y->getType() == x->getType())
  VecOpFactory<T>::axpby(y->getType(), y, a, b, x);
}

/// \brief y=ax+by+z
template <typename T>
void axpbypz(Vector<T> *y, T a, T b, Vector<T> *x, Vector<T> *z) {
  ASSERT(y->getType() == x->getType() && y->getType() == z->getType())
  VecOpFactory<T>::axpbypz(y->getType(), y, a, b, x, z);
}

/// \brief x=a*x
template <typename T>
void scale(Vector<T> *x, T a) {
  VecOpFactory<T>::scale(x->getType(), x, a);
}

/// \brief r=x*y
template <typename T>
void dot(Vector<T> *x, Vector<T> *y, T &r) {
  ASSERT(y->getType() == x->getType())
  VecOpFactory<T>::dot(y->getType(), x, y, r);
}

/// \brief r=||x||_lp
template <typename T>
void normlp(Vector<T> *x, scalar lp, T &r) {
  VecOpFactory<T>::normlp(x->getType(), x, lp, r);
}

/// \brief r=sum(xi)
template <typename T>
void sum(Vector<T> *x, T &r) {
  VecOpFactory<T>::sum(x->getType(), x, r);
}

/// \brief y=x
template <typename T>
void copy(Vector<T> *x, Vector<T> *y) {
  ASSERT(y->getType() == x->getType())
  VecOpFactory<T>::copy(x->getType(), x, y);
}

/// \brief y=x and x=y
template <typename T>
void swap(Vector<T> *x, Vector<T> *y) {
  ASSERT(y->getType() == x->getType())
  VecOpFactory<T>::swap(x->getType(), x, y);
}

/// \brief wi=xi*yi
template <typename T>
void pointwiseMult(Vector<T> *w, Vector<T> *x, Vector<T> *y) {
  ASSERT(y->getType() == x->getType() && y->getType() == w->getType())
  VecOpFactory<T>::pointwiseMult(w->getType(), x, y);
}

/// \brief wi=xi/yi
template <typename T>
void pointwiseDiv(Vector<T> *w, Vector<T> *x, Vector<T> *y) {
  ASSERT(y->getType() == x->getType() && y->getType() == w->getType())
  VecOpFactory<T>::pointwiseDiv(w->getType(), x, y);
}

/// \brief r=max(xi)
template <typename T>
void vecMax(Vector<T> *x, T &r) {
  VecOpFactory<T>::vecMax(x->getType(), x, r);
}

/// \brief r=min(xi)
template <typename T>
void vecMin(Vector<T> *x, T &r) {
  VecOpFactory<T>::vecMin(x->getType(), x, r);
}

/// \brief xi=1/xi
template <typename T>
void reciprocal(Vector<T> *x) {
  VecOpFactory<T>::reciprocal(x->getType(), x);
}

/// \brief xi=s+xi
template <typename T>
void shift(Vector<T> *x, T s) {
  VecOpFactory<T>::shift(x->getType(), x, s);
}

/// \brief xi=s
template <typename T>
void vecSet(Vector<T> *x, T s) {
  VecOpFactory<T>::vecSet(x->getType(), x, s);
}
/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */

template <typename T>
inline void swap(Vector<T> &a, Vector<T> &b) {
  a.swap(b);
}

template <typename T>
inline T Vector<T>::operator*(const T *v) const {
  T dot = 0.;
  for (label i = 0; i < size_; i++) dot += data_[i] * v[i];
  return dot;
}
template <typename T>
inline T Vector<T>::operator*(const Vector<T> &v) const {
  return operator*(v.data_);
}
template <typename T>
inline Vector<T> &Vector<T>::operator=(const T *v) {
  std::copy(v, v + size_, data_);
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator=(const Vector<T> &v) {
  newDataAndSize(v.data_, v.size_);
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator=(Vector<T> &&v) {
  *this = std::move(v);
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator=(T value) {
  for (label i = 0; i < size_; i++) data_[i] = value;
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator*=(T c) {
  for (label i = 0; i < size_; i++) data_[i] *= c;
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator*=(const Vector<T> &v) {
  ASSERT(size_ == v.size_);
  for (label i = 0; i < size_; i++) data_[i] *= v[i];
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator/=(T c) {
  for (label i = 0; i < size_; i++) data_[i] /= c;
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator/=(const Vector<T> &v) {
  ASSERT(size_ == v.size_);
  for (label i = 0; i < size_; i++) data_[i] /= v[i];
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator-=(T c) {
  for (label i = 0; i < size_; i++) data_[i] -= c;
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator-=(const Vector<T> &v) {
  ASSERT(size_ == v.size_);
  for (label i = 0; i < size_; i++) data_[i] -= v[i];
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator+=(T c) {
  for (label i = 0; i < size_; i++) data_[i] += c;
  return *this;
}
template <typename T>
inline Vector<T> &Vector<T>::operator+=(const Vector<T> &v) {
  ASSERT(size_ == v.size_);
  for (label i = 0; i < size_; i++) data_[i] += v[i];
  return *this;
}
template <typename T>
inline void Vector<T>::getSubVector(const std::vector<label> &dofs,
                                    Vector<T> &elemvect) const {
  ASSERT(dofs.size() <= elemvect.size())
  getSubVector(dofs, elemvect.data_);
}
template <typename T>
inline void Vector<T>::getSubVector(const std::vector<label> &dofs,
                                    T *elem_data) const {
  const label n = dofs.size();
  for (label i = 0; i < n; i++) {
    const label dofi = dofs[i];
    elem_data[i] = dofi >= 0 ? data_[dofi] : -data_[-dofi - 1];
  }
}
/*! set the entries listed in  dofs to the given  value. */
template <typename T>
inline void Vector<T>::setSubVector(const std::vector<label> &dofs, const T value) {
  const label n = dofs.size();
  for (label i = 0; i < n; i++) {
    const label dofi = dofs[i];
    if (dofi >= 0)
      data_[dofi] = value;
    else
      data_[-1 - dofi] = -value;
  }
}
/*! set the entries listed in  dofs to the values given in elemvect */
template <typename T>
inline void Vector<T>::setSubVector(const std::vector<label> &dofs,
                                    const Vector<T> &elemvect) {
  ASSERT(dofs.size() <= elemvect.size())
  setSubVector(dofs, elemvect.data_);
}
/*! set the entries listed in  dofs to the values given the  array */
template <typename T>
inline void Vector<T>::setSubVector(const std::vector<label> &dofs, T *elem_data) {
  const label n = dofs.size();
  for (label i = 0; i < n; i++) {
    const label dofi = dofs[i];
    if (dofi >= 0)
      data_[dofi] = elem_data[i];
    else
      data_[-1 - dofi] = -elem_data[i];
  }
}
template <typename T>
inline void Vector<T>::addSubVector(const std::vector<label> &dofs,
                                    const Vector<T> &elemvect) {
  ASSERT(dofs.size() <= elemvect.size())
  addSubVector(dofs, elemvect.data_);
}
/*! add elements of the  elem_data array to the entries listed in dofs */
template <typename T>
inline void Vector<T>::addSubVector(const std::vector<label> &dofs, T *elem_data) {
  const label n = dofs.size();
  for (label i = 0; i < n; i++) {
    const label dofi = dofs[i];
    if (dofi >= 0)
      data_[dofi] += elem_data[i];
    else
      data_[-1 - dofi] -= elem_data[i];
  }
}
/*! add a times the elements of elemvect to the entries listed in dofs */
template <typename T>
inline void Vector<T>::addSubVector(const std::vector<label> &dofs, const T a,
                                    const Vector<T> &elemvect) {
  const label n = dofs.size();
  const T *x = elemvect.data_;
  for (label i = 0; i < n; i++) {
    const label dofi = dofs[i];
    if (dofi >= 0)
      data_[dofi] += a * x[i];
    else
      data_[-1 - dofi] -= a * x[i];
  }
}
template <typename T>
inline void Vector<T>::setOtherSubVector(const std::vector<label> &dofs,
                                         const T val) {
  const label n = dofs.size();
  const label N = size_;
  Vector<T> dofsVal(n);
  T *y = dofsVal.data_;
  for (label i = 0; i < n; i++) y[i] = data_[dofs[i]];
  for (label i = 0; i < N; i++) data_[i] = val;
  for (label i = 0; i < n; i++) data_[dofs[i]] = y[i];
}

template <typename T>
inline scalar Vector<T>::norml2() const {
  COUT << "Not implemented!" << ENDL;
  return -1;
}
template <typename T>
inline scalar Vector<T>::normlinf() const {
  COUT << "Not implemented!" << ENDL;
  return -1;
}
template <typename T>
inline scalar Vector<T>::norml1() const {
  COUT << "Not implemented!" << ENDL;
  return -1;
}
template <typename T>
inline scalar Vector<T>::normlp(scalar p) const {
  COUT << "Not implemented!" << ENDL;
  return -1;
}

template <>
inline scalar Vector<scalar>::norml2() const {
  if (0 == size_) {
    return 0.0;
  }
  if (1 == size_) {
    return std::abs(data_[0]);
  }
  scalar scale = 0.0;
  scalar sum = 0.0;
  for (int i = 0; i < size_; i++) {
    if (data_[i] != 0.0) {
      const scalar absdata = fabs(data_[i]);
      if (scale <= absdata) {
        const scalar sqr_arg = scale / absdata;
        sum = 1.0 + sum * (sqr_arg * sqr_arg);
        scale = absdata;
        continue;
      }  // end if scale <= absdata
      const scalar sqr_arg = absdata / scale;
      sum += (sqr_arg * sqr_arg);  // else scale > absdata
    }                              // end if data[i] != 0
  }
  return scale * std::sqrt(sum);
}
template <>
inline scalar Vector<scalar>::normlinf() const {
  scalar max = 0.;
  for (label i = 0; i < size_; i++) max = std::max(std::abs(data_[i]), max);
  return max;
}
template <>
inline scalar Vector<scalar>::norml1() const {
  scalar sum = 0.;
  for (label i = 0; i < size_; i++) sum += std::abs(data_[i]);
  return sum;
}
template <>
inline scalar Vector<scalar>::normlp(scalar p) const {
  ASSERT(p > 0.0)
  if (p == 1.0) return norml1();
  if (p == 2.0) return norml2();
  if (p < std::numeric_limits<scalar>::infinity()) {
    if (0 == size_) return 0.0;
    if (1 == size_) return std::abs(data_[0]);
    scalar scale = 0.0;
    scalar sum = 0.0;

    for (label i = 0; i < size_; i++) {
      if (data_[i] != 0.0) {
        const scalar absdata = std::abs(data_[i]);
        if (scale <= absdata) {
          sum = 1.0 + sum * std::pow(scale / absdata, p);
          scale = absdata;
          continue;
        }                                     // end if scale <= absdata
        sum += std::pow(absdata / scale, p);  // else scale > absdata
      }                                       // end if data[i] != 0
    }
    return scale * std::pow(sum, 1.0 / p);
  }  // end if p < infinity()

  return normlinf();  // else p >= infinity()
}

template <typename T>
inline T Vector<T>::max() const {
  ASSERT(size_ > 0)
  T max = data_[0];
  for (label i = 1; i < size_; i++) {
    if (data_[i] > max) {
      max = data_[i];
    }
  }
  return max;
}
template <typename T>
inline T Vector<T>::min() const {
  ASSERT(size_ > 0)
  T min = data_[0];
  for (label i = 1; i < size_; i++) {
    if (data_[i] < min) {
      min = data_[i];
    }
  }
  return max;
}
template <typename T>
inline T Vector<T>::sum() const {
  T sum = 0.0;

  for (label i = 0; i < size_; i++) {
    sum += data_[i];
  }

  return sum;
}

/*! * * * * * * * * * * * * * * * VecOp * * * * * * * * * * * * * */
/*! DEFAULT  */
template <VecType type>
class VecOp {
 public:
  template <typename T>
  static void axpy(Vector<T> *y, T a, Vector<T> *x) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void aypx(Vector<T> *y, T a, Vector<T> *x) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void waxpy(Vector<T> *w, T a, Vector<T> *x, Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void axpby(Vector<T> *y, T a, T b, Vector<T> *x) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void axpbypz(Vector<T> *y, T a, T b, Vector<T> *x, Vector<T> *z) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void scale(Vector<T> *x, T a) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void dot(Vector<T> *x, Vector<T> *y, T &r) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void normlp(Vector<T> *x, scalar lp, T &r) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void sum(Vector<T> *x, T &r) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void copy(Vector<T> *x, Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void swap(Vector<T> *x, Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void pointwiseMult(Vector<T> *w, Vector<T> *x, Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void pointwiseDiv(Vector<T> *w, Vector<T> *x, Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void vecMax(Vector<T> *x, T &r) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void vecMin(Vector<T> *x, T &r) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void reciprocal(Vector<T> *x) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void shift(Vector<T> *x, T s) {
    COUT << "Not implemented!" << ENDL;
  }
  template <typename T>
  static void vecSet(Vector<T> *x, T s) {
    COUT << "Not implemented!" << ENDL;
  }
};

}  // namespace HSF
#endif /* end of include guard: NUMERICS_VECTOR_VECTOR_HPP */
