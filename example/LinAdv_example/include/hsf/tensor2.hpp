/*! \file tensor2.hpp
 *  \author Zhao Chengpeng
 *  \date 2022-01-18
 *  \email chengpeng_zhao@foxmail.com
 *  \brief rank 2 tensor
 */

#ifndef TENSOR_TENSOR2_TENSOR2_HPP
#define TENSOR_TENSOR2_TENSOR2_HPP

#include "tensor1.hpp"
#include "tensorSpace.hpp"

namespace HSF {

template <class Cmpt, direction Ndim = 3>
class Tensor2 : public TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> {
 public:
  // * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * *

  /*! 不进行赋值 */
  inline Tensor2() : TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>() {}
  /*! 赋值为0 */
  inline Tensor2(const HSF::zero Zero)
      : TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>(Zero) {}
  /*! 所有元素赋值为s */
  inline Tensor2(const Cmpt s) : TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>(s) {}
  /*! 用数组赋值 */
  inline Tensor2(const Cmpt (&v)[Ndim * Ndim])
      : TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>(v) {}
  /*! 拷贝赋值 */
  inline Tensor2(const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& t)
      : TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>(t) {}

  // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * *

  /*! 获取某列 */
  Tensor1<Cmpt, Ndim> col(const direction& c) const {
    Tensor1<Cmpt, Ndim> t1;
    if (c < Ndim) {
      for (direction i = 0; i < Ndim; ++i) {
        t1[i] = (*this)(i, c);
      }
    } else {
      std::cerr << "Invaild column access!" << std::endl;
    }
    return t1;
  }

  /*! 获取某行 */
  Tensor1<Cmpt, Ndim> row(const direction& c) const {
    Tensor1<Cmpt, Ndim> t1;
    if (c < Ndim) {
      for (direction i = 0; i < Ndim; ++i) {
        t1[i] = (*this)(c, i);
      }
    } else {
      std::cerr << "Invaild column access!" << std::endl;
    }
    return t1;
  }

  /*! 获取对角 */
  Tensor1<Cmpt, Ndim> diag() const {
    Tensor1<Cmpt, Ndim> t1;
    for (direction i = 0; i < Ndim; ++i) {
      t1[i] = (*this)(i, i);
    }
    return t1;
  }

  /*! 2阶张量的迹（等于其缩并结果） */
  Cmpt tr() {
    Cmpt res = 0;
    for (direction i = 0; i < Ndim; ++i) {
      for (direction j = 0; j < Ndim; ++j) {
        if (i == j) res += (*this)(i, j);
      }
    }
    return res;
  }

  /*！转置 */
  TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> transpose() {
    TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> ts;
    for (direction i = 0; i < Ndim; ++i) {
      for (direction j = 0; j < Ndim; ++j) {
        ts(i, j) = (*this)(j, i);
      }
    }
    return ts;
  }
};

} /* End namespace HSF */

namespace HSF {
// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * *

//推荐采用global functions，因为TensorSpace无法使用Tensor2的方法

/*! 2阶张量的迹（等于其缩并结果）*/
template <class Cmpt, direction Ndim>
Cmpt tr(const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& t) {
  Cmpt res = 0;
  for (direction i = 0; i < Ndim; ++i) {
    for (direction j = 0; j < Ndim; ++j) {
      if (i == j) res += t(i, j);
    }
  }
  return res;
}

/*! 2阶张量的转置 */
template <class Cmpt, direction Ndim>
TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> transpose(
    const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& A) {
  TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> ts;
  for (direction i = 0; i < Ndim; ++i) {
    for (direction j = 0; j < Ndim; ++j) {
      ts(i, j) = A(j, i);
    }
  }
  return ts;
}

/*! Return the spherical part of a Tensor */
template <class Cmpt, direction Ndim>
inline TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> sph(
    const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& A) {
  TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> ts;
  return kronecker<Cmpt, Ndim>((1.0 / 3) * tr(A));
}

/*! Return the deviatoric part of a Tensor */
template <class Cmpt, direction Ndim>
inline TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> dev(
    const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& A) {
  return A - sph(A);
}

/*! Return the symmetric part of a Tensor */
template <class Cmpt, direction Ndim>
inline TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> symm(
    const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& A) {
  TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> ts;
  return 0.5 * (A + transpose(A));
}

/*! Return the symmetric part of a Tensor */
template <class Cmpt, direction Ndim>
inline TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> skew(
    const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& A) {
  TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> ts;
  return 0.5 * (A - transpose(A));
}

/*! 2阶张量的行列式
 * 由于维度不定，这里没有采用余子式相关的公式
 */
template <class Cmpt, direction Ndim>
inline Cmpt det(const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& t) {
  Cmpt Ia = tr(t);
  Cmpt IIa_ = tr(t & t);
  Cmpt IIIa_ = tr(t & t & t);
  return (IIIa_ - 3 * IIa_ * Ia / 2 + Ia * Ia * Ia / 2) / 3;
}

/*! 2阶张量的三个不变量，通过二阶张量的矩(tr(A)、tr(A&A)、tr(A&A&A))得到 */
template <class Cmpt, direction Ndim>
inline void charInvs(const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& t, Cmpt& Ia,
                     Cmpt& IIa, Cmpt& IIIa) {
  // characteristic invariants
  Ia = tr(t);
  Cmpt IIa_ = tr(t & t);
  Cmpt IIIa_ = tr(t & t & t);
  IIa = (Ia * Ia - IIa_) / 2;
  IIIa = (IIIa_ - 3 * IIa_ * Ia / 2 + Ia * Ia * Ia / 2) / 3;
}

/*! 伴随矩阵，Return the cofactor Tensor of a Tensor */
template <class Cmpt, direction Ndim>
inline TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> cof(
    const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& A) {
  // characteristic invariants
  Cmpt Ia, IIa, IIIa;
  charInvs(A, Ia, IIa, IIIa);
  // identity
  Tensor2<Cmpt, Ndim> I = kronecker<Cmpt, Ndim>(1);
  return ((A & A) - A * Ia + I * IIa);
}

/* 2阶张量当做矩阵求逆 */
template <class Cmpt, direction Ndim>
inline TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> inv(
    const TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& A) {
  // characteristic invariants
  Cmpt Ia, IIa, IIIa;
  charInvs(A, Ia, IIa, IIIa);
  // identity
  Tensor2<Cmpt, Ndim> I = kronecker<Cmpt, Ndim>(1);
  if (IIIa == 0) {
    std::cerr << "Inverse doesn't exist!" << std::endl;
    exit(1);
  } else {
    return ((A & A) - A * Ia + I * IIa) / IIIa;
  }
}

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * *

/*! Division of a Vector by a Tensor */
template <class Cmpt, direction Ndim>
inline TensorSpace<Cmpt, 1, Ndim, Ndim> operator/(
    const TensorSpace<Cmpt, 1, Ndim, Ndim>& v,
    TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim>& t) {
  return v & inv(t);
}

} /* End namespace HSF */

#endif /* end of include guard: TENSOR_TENSOR2_TENSOR2_HPP */
