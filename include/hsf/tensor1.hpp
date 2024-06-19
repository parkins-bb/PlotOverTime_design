/*! \file tensor1.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-01-18
 *  \brief 矢量类
 */

#ifndef TENSOR_TENSOR1_TENSOR1_HPP
#define TENSOR_TENSOR1_TENSOR1_HPP

#include "tensorSpace.hpp"

namespace HSF {

template <class Cmpt, direction Ndim = 3>
class Tensor1 : public TensorSpace<Cmpt, 1, Ndim, Ndim> {
 public:
  // * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * *

  /*! 不进行赋值 */
  inline Tensor1() : TensorSpace<Cmpt, 1, Ndim, Ndim>() {}
  /*! 赋值为0 */
  inline Tensor1(const HSF::zero Zero)
      : TensorSpace<Cmpt, 1, Ndim, Ndim>(Zero) {}
  /*! 所有元素赋值为s */
  inline Tensor1(const Cmpt s) : TensorSpace<Cmpt, 1, Ndim, Ndim>(s) {}
  /*! 用数组赋值 */
  inline Tensor1(const Cmpt (&v)[Ndim]) : TensorSpace<Cmpt, 1, Ndim, Ndim>(v) {}
  /*! 拷贝赋值 */
  inline Tensor1(const TensorSpace<Cmpt, 1, Ndim, Ndim> &t)
      : TensorSpace<Cmpt, 1, Ndim, Ndim>(t) {}

  inline Cmpt const x() const { return (*this)[0]; }
  inline Cmpt const y() const { return (*this)[1]; }
  inline Cmpt const z() const { return (*this)[2]; }
  inline Cmpt &x() { return (*this)[0]; }
  inline Cmpt &y() { return (*this)[1]; }
  inline Cmpt &z() { return (*this)[2]; }
};

} /* End namespace HSF */

namespace HSF {
// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * *

/* 矢量叉乘张量, Cross Product
 * 由于叉乘的就近原则，张量叉乘矢量感觉难以实现(涉及到张量的分解)
 * 采用 ^ 表示叉乘，与OpenFOAM保持一致
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, 1, Ndim, Ndim> operator^(
    const TensorSpace<Cmpt, 1, Ndim, Ndim> &v1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> &t2) {
  return reduction(reduction(permutation<Cmpt, Ndim>(1) * v1 * t2, 1, 3), 1, 2);
}
/* 3维矢量间的叉乘，和上面一般情况的函数相比效率更高 */
template <class Cmpt>
inline TensorSpace<Cmpt, 1, 3, 3> operator^(
    const TensorSpace<Cmpt, 1, 3, 3> &v1,
    const TensorSpace<Cmpt, 1, 3, 3> &v2) {
  return TensorSpace<Cmpt, 1, 3, 3>({v1[1] * v2[2] - v1[2] * v2[1],
                                     v1[2] * v2[0] - v1[0] * v2[2],
                                     v1[0] * v2[1] - v1[1] * v2[0]});
}

} /* End namespace HSF */

#endif /* end of include guard: TENSOR_TENSOR1_TENSOR1_HPP */
