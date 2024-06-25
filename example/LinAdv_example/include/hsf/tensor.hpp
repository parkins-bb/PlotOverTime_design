/*! \file tensor.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-01-19
 *  \brief brief description
 */

#ifndef NUMERICS_TENSOR_TENSOR_HPP
#define NUMERICS_TENSOR_TENSOR_HPP

#include "complex.hpp"
#include "tensor1.hpp"
#include "tensor2.hpp"
#include "tensorSpace.hpp"

namespace HSF {

// C++11 feature
template <class Cmpt, direction Ndim = 3>
using Vec = Tensor1<Cmpt, Ndim>;
template <class Cmpt, direction Ndim = 3>
using Tensor = Tensor2<Cmpt, Ndim>;

template <class Cmpt, direction Ndim = 3>
class Tensor3 : public TensorSpace<Cmpt, 3, Ndim, Ndim * Ndim * Ndim> {
 public:
  // * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * *

  /*! 不进行赋值 */
  inline Tensor3() : TensorSpace<Cmpt, 3, Ndim, Ndim * Ndim * Ndim>() {}
  /*! 赋值为0 */
  inline Tensor3(const HSF::zero Zero)
      : TensorSpace<Cmpt, 3, Ndim, Ndim * Ndim * Ndim>(Zero) {}
  /*! 所有元素赋值为s */
  inline Tensor3(const Cmpt s)
      : TensorSpace<Cmpt, 3, Ndim, Ndim * Ndim * Ndim>(s) {}
  /*! 用数组赋值 */
  inline Tensor3(const Cmpt (&v)[Ndim * Ndim * Ndim])
      : TensorSpace<Cmpt, 3, Ndim, Ndim * Ndim * Ndim>(v) {}
  /*! 拷贝赋值 */
  inline Tensor3(const TensorSpace<Cmpt, 3, Ndim, Ndim * Ndim * Ndim>& t)
      : TensorSpace<Cmpt, 3, Ndim, Ndim * Ndim * Ndim>(t) {}
};

template <class Cmpt, direction Ndim = 3>
class Tensor4 : public TensorSpace<Cmpt, 4, Ndim, Ndim * Ndim * Ndim * Ndim> {
 public:
  // * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * *

  /*! 不进行赋值 */
  inline Tensor4() : TensorSpace<Cmpt, 4, Ndim, Ndim * Ndim * Ndim * Ndim>() {}
  /*! 赋值为0 */
  inline Tensor4(const HSF::zero Zero)
      : TensorSpace<Cmpt, 4, Ndim, Ndim * Ndim * Ndim * Ndim>(Zero) {}
  /*! 所有元素赋值为s */
  inline Tensor4(const Cmpt s)
      : TensorSpace<Cmpt, 4, Ndim, Ndim * Ndim * Ndim * Ndim>(s) {}
  /*! 用数组赋值 */
  inline Tensor4(const Cmpt (&v)[Ndim * Ndim * Ndim * Ndim])
      : TensorSpace<Cmpt, 4, Ndim, Ndim * Ndim * Ndim * Ndim>(v) {}
  /*! 拷贝赋值 */
  inline Tensor4(const TensorSpace<Cmpt, 4, Ndim, Ndim * Ndim * Ndim * Ndim>& t)
      : TensorSpace<Cmpt, 4, Ndim, Ndim * Ndim * Ndim * Ndim>(t) {}
};

} /* End namespace HSF */

namespace HSF {
// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * *

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * *

} /* End namespace HSF */

#endif /* end of include guard: NUMERICS_TENSOR_TENSOR_HPP */
