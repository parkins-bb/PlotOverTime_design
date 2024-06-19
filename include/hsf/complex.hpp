/*! \file complex.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-01-07
 *  \brief complex type
 */

#ifndef NUMERICS_TENSOR_COMPLEX_HPP
#define NUMERICS_TENSOR_COMPLEX_HPP

#include <complex>

#include "scalar.hpp"
#include "zero.hpp"

namespace HSF {

/*! 复数类型，部分函数使用<complex>库 */
class Complex {
 private:
  //- Real and imaginary parts
  scalar re_, im_;

 public:
  // Friend Operators
  friend Complex operator-(const Complex& c);
  friend std::istream& operator>>(std::istream& os, Complex& c);
  friend std::ostream& operator<<(std::ostream& os, const Complex& c);
  friend Complex operator+(const Complex& c1, const Complex& c2);
  friend Complex operator+(const Complex& c, const scalar& s);
  friend Complex operator+(const scalar& s, const Complex& c);
  friend Complex operator-(const Complex& c1, const Complex& c2);
  friend Complex operator-(const Complex& c, const scalar& s);
  friend Complex operator-(const scalar& s, const Complex& c);
  friend Complex operator*(const Complex& c1, const Complex& c2);
  friend Complex operator*(const Complex& c, const scalar& s);
  friend Complex operator*(const scalar& s, const Complex& c);
  friend Complex operator/(const Complex& c1, const Complex& c2);
  friend Complex operator/(const Complex& c, const scalar& s);
  friend Complex operator/(const scalar& s, const Complex& c);

  // Constructors

  //- Default construct, as zero-initialized
  inline Complex() : re_(0), im_(0) {}
  inline Complex(const HSF::zero) : re_(0), im_(0) {}

  //- Construct from real component
  inline Complex(const scalar r) : re_(r), im_(0) {}

  //- Construct from real and imaginary parts
  inline Complex(const scalar r, const scalar i) : re_(r), im_(i) {}

  //- Construct from array
  inline Complex(const scalar (&array)[2]) : re_(array[0]), im_(array[1]) {}

  //- Implicit construct from std::complex
  inline Complex(const std::complex<float>& c) : re_(c.real()), im_(c.imag()) {}

  //- Implicit construct from std::complex
  inline Complex(const std::complex<double>& c)
      : re_(c.real()), im_(c.imag()) {}

  // Member Functions

  //- Real part of complex number - STL naming
  const scalar& real() const { return re_; }

  //- Imaginary part of complex number - STL naming
  const scalar& imag() const { return im_; }

  //- Set real part of complex number - STL naming
  inline void real(scalar val) { re_ = val; }

  //- Set imaginary part of complex number - STL naming
  inline void imag(scalar val) { im_ = val; }

  // Access

  //- Real part of complex number
  inline scalar Re() const { return re_; }

  //- Imaginary part of complex number
  inline scalar Im() const { return im_; }

  // Edit

  //- Real part of complex number
  inline scalar& Re() { return re_; }

  //- Imaginary part of complex number
  inline scalar& Im() { return im_; }

  //- Complex conjugate
  inline Complex conjugate() const { return Complex(re_, -im_); }

  // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * *

  //- Implicit conversion to std::complex
  operator std::complex<scalar>() const {
    return std::complex<scalar>(re_, im_);
  }

  //- Assign scalar (imag = zero)
  inline void operator=(const scalar& s) {
    re_ = s;
    im_ = 0;
  }
  inline void operator=(const HSF::zero) {
    re_ = 0;
    im_ = 0;
  }

  inline void operator+=(const Complex& c) {
    re_ += c.re_;
    im_ += c.im_;
  }
  inline void operator+=(const scalar& s) { re_ += s; }

  inline void operator-=(const Complex& c) {
    re_ -= c.re_;
    im_ -= c.im_;
  }
  inline void operator-=(const scalar& s) { re_ -= s; }

  inline void operator*=(const Complex& c) { *this = (*this) * c; }
  inline void operator*=(const scalar& s) {
    re_ *= s;
    im_ *= s;
  }

  inline void operator/=(const Complex& c) { *this = *this / c; }
  inline void operator/=(const scalar& s) {
    re_ /= s;
    im_ /= s;
  }

  inline bool operator==(const Complex& c) const {
    return (math::equal(re_, c.re_) && math::equal(im_, c.im_));
  }
  inline bool operator!=(const Complex& c) const { return !operator==(c); }
};

namespace math {

// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

/*! 模的平方
 *  \param [in] c : \f$a+bi\f$
 *  \return  \f$a^2+b^2\f$
 */
inline scalar magSqr(const Complex& c) {
  return (c.real() * c.real() + c.imag() * c.imag());
}

/*! 模
 *  \param[in] c : \f$a+bi\f$
 *  \return  \f$\sqrt{a^2+b^2}\f$
 */
inline scalar mag(const Complex& c) { return std::hypot(c.real(), c.imag()); }

/*! 平方
 *  \param[in] c : \f$a+bi\f$
 *  \return  \f$(a+bi)^2\f$
 */
inline Complex sqr(const Complex& c) { return c * c; }

/*! sgn() https://en.wikipedia.org/wiki/Sign_function#Complex_signum
 *  \param[in] c : \f$a+bi\f$
 *  \return  \f$\frac{c}{|c|}\f$
 */
inline Complex sgn(const Complex& c) {
  const scalar s(mag(c));
  return s < math::ROOTVSMALL ? Zero : c / s;
}

/*! csgn() https://en.wikipedia.org/wiki/Sign_function#Complex_signum
 *  \param[in] c : \f$a+bi\f$
 *  \return  \f$\begin{cases}1 & \text { if } \operatorname{Re}(z)>0, \\ -1 &
 * \text { if } \operatorname{Re}(z)<0, \\ \operatorname{sgn}
 * \operatorname{Im}(z) & \text { if } \operatorname{Re}(z)=0\end{cases}\f$
 */
inline scalar csgn(const Complex& c) {
  return math::equal(c.Re(), 0.0) ? math::sign(c.Im()) : math::sign(c.Re());
}

/*! 返回模较小的复数
 *  \param[in] c1 c2
 *  \return  c1 if \f$|c1|<|c2|\f$
 */
inline const Complex& min(const Complex& c1, const Complex& c2) {
  if (magSqr(c1) < magSqr(c2)) {
    return c1;
  }
  return c2;
}

/*! 返回模较大的复数
 *  \param[in] c1 c2
 *  \return  c1 if \f$|c1|>|c2|\f$
 */
inline const Complex& max(const Complex& c1, const Complex& c2) {
  if (magSqr(c1) < magSqr(c2)) {
    return c2;
  }
  return c1;
}

/*! <complex>标准库中函数 */
#define transFunc(func)                                   \
  inline Complex func(const Complex& c) {                 \
    return static_cast<Complex>(                          \
        std::func(static_cast<std::complex<scalar>>(c))); \
  }

/*! returns the magnitude of a complex number */
transFunc(abs);
/*! returns the phase angle */
transFunc(arg);
/*! returns the squared magnitude */
transFunc(norm);
/*! returns the complex conjugate */
transFunc(conj);
/*! complex base e exponential */
transFunc(exp);
/*! complex natural logarithm with the branch cuts along the negative real
   axis */
transFunc(log);
/*! complex common logarithm with the branch cuts along the negative real
   axis */
transFunc(log10);
/*! complex square root in the range of the right half-plane */
transFunc(sqrt);
/*! computes sine of a complex number */
transFunc(sin);
/*! computes cosine of a complex number */
transFunc(cos);
/*! computes tangent of a complex number */
transFunc(tan);
/*! computes hyperbolic sine of a complex number */
transFunc(sinh);
/*! computes hyperbolic cosine of a complex number */
transFunc(cosh);
/*! 	computes hyperbolic tangent of a complex number */
transFunc(tanh);

// C++11才有的函数
#if (__cplusplus >= 201103L)
transFunc(asin);
transFunc(acos);
transFunc(atan);
transFunc(asinh);
transFunc(acosh);
transFunc(atanh);
#endif

#undef transFunc

/*! Special treatment for pow() */
inline Complex pow(const Complex& x, const Complex& y) {
  return std::pow(static_cast<std::complex<scalar>>(x),
                  static_cast<std::complex<scalar>>(y));
}

/*! Combinations of complex and integral/float */
#define powFuncs(type2)                                       \
  inline Complex pow(const Complex& x, const type2 y) {       \
    return std::pow(static_cast<std::complex<scalar>>(x), y); \
  }                                                           \
                                                              \
  inline Complex pow(const type2 x, const Complex& y) {       \
    return std::pow(static_cast<std::complex<scalar>>(x),     \
                    static_cast<std::complex<scalar>>(y));    \
  }

powFuncs(int);
powFuncs(float);
powFuncs(double);
powFuncs(long);
#if defined(LABEL_INT64)
powFuncs(int64_t);
#endif

#undef powFuncs
}  // namespace math

// * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * *//

/*! \f$-(a+bi)\f$ */
inline Complex operator-(const Complex& c) { return Complex(-c.re_, -c.im_); }

/*! 输入流 */
inline std::istream& operator<<(std::istream& is, Complex& c) {
  scalar r, i;
  is >> r >> i;
  c.real(r);
  c.imag(i);
  return is;
}

/*! 输出形式 */
inline std::ostream& operator<<(std::ostream& os, const Complex& c) {
  os << "(" << c.re_ << "," << c.im_ << ")";
  return os;
}

/*! \f$(a_1+b_1i)+(a_2+b_2i)\f$ */
inline Complex operator+(const Complex& c1, const Complex& c2) {
  return Complex(c1.re_ + c2.re_, c1.im_ + c2.im_);
}

/*! \f$(a+bi)+(s)\f$ */
inline Complex operator+(const Complex& c, const scalar& s) {
  return Complex(c.re_ + s, c.im_);
}

/*! \f$(s)+(a+bi)\f$ */
inline Complex operator+(const scalar& s, const Complex& c) {
  return Complex(c.re_ + s, c.im_);
}

/*! \f$(a_1+b_1i)-(a_2+b_2i)\f$ */
inline Complex operator-(const Complex& c1, const Complex& c2) {
  return Complex(c1.re_ - c2.re_, c1.im_ - c2.im_);
}

/*! \f$(a+bi)-(s)\f$ */
inline Complex operator-(const Complex& c, const scalar& s) {
  return Complex(c.re_ - s, c.im_);
}

/*! \f$(s)-(a+bi)\f$ */
inline Complex operator-(const scalar& s, const Complex& c) {
  return Complex(s - c.re_, -c.im_);
}

/*! \f$(a_1+b_1i)*(a_2+b_2i)\f$ */
inline Complex operator*(const Complex& c1, const Complex& c2) {
  return Complex(c1.re_ * c2.re_ - c1.im_ * c2.im_,
                 c1.im_ * c2.re_ + c1.re_ * c2.im_);
}

/*! \f$(a+bi)*(s)\f$ */
inline Complex operator*(const Complex& c, const scalar& s) {
  return Complex(s * c.re_, s * c.im_);
}

/*! \f$s*(a+bi)\f$ */
inline Complex operator*(const scalar& s, const Complex& c) {
  return Complex(s * c.re_, s * c.im_);
}

/*! \f$\frac{a_1+b_1i}{a_2+b_2i}\f$ */
inline Complex operator/(const Complex& c1, const Complex& c2) {
  const scalar sqrC2 = math::magSqr(c2);

  return Complex((c1.re_ * c2.re_ + c1.im_ * c2.im_) / sqrC2,
                 (c1.im_ * c2.re_ - c1.re_ * c2.im_) / sqrC2);
}

/*! \f$\frac{a+bi}{s}\f$ */
inline Complex operator/(const Complex& c, const scalar& s) {
  return Complex(c.re_ / s, c.im_ / s);
}

/*! \f$\frac{s}{a+bi}\f$ */
inline Complex operator/(const scalar& s, const Complex& c) {
  return Complex(s) / c;
}

inline Complex operator~(const Complex& c) { return c.conjugate(); }

} /* End namespace HSF */

#endif /* end of include guard: NUMERICS_TENSOR_COMPLEX_HPP */
