/*! \file indexstd::vector.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-01-13
 *  \modified 2012-09-23
 *  \brief tensor index array
 */
#ifndef NUMERICS_TENSOR_INDEXARRAY_HPP
#define NUMERICS_TENSOR_INDEXARRAY_HPP

//不需要再用到
#if 0
#include <cstdarg>
#include <iostream>

#include "scalarType.hpp"
#include "zero.hpp"

namespace HSF {
namespace INTERNAL {

/*! 不同阶张量的循环层数不一样，需要借助一个array来遍历 */
template <direction N>
struct std::vector {
  /*! data */
  direction values[N];
  /*! access */
  inline direction& operator[](direction index) { return values[index]; }
  inline const direction& operator[](direction index) const {
    return values[index];
  }
  static direction size() { return N; }

  /*! constructor */
  inline std::vector() /*: values{0}*/ {}
  inline std::vector(zero) : values{0} {}

  /*! 不定参数方式初始化 */
  std::vector(const direction v1, ...) {
    values[0] = v1;
    va_list args;
    va_start(args, v1);
    for (direction i = 1; i < N; ++i) {
      values[i] = va_arg(args, direction);
    }
    va_end(args);
  }

  /*! 直接用array初始化，可以代替不定参数初始化的方式，只需多加一个{} */
  std::vector(const direction (&a)[N]) {
    for (direction i = 0; i < N; ++i) {
      values[i] = a[i];
    }
  }

  /*! 移除其中一个index，进行张量的缩并等操作时需要用到 */
  std::vector<N - 1> remove(const direction i) {
    std::vector<N - 1> res;
    direction index = 0;
    for (direction k = 0; k < N; k++) {
      if (k != i) {
        res[index] = values[k];
        index++;
      }
    }
    return res;
  }
  /*! 移除其中两个index，进行张量的缩并等操作时需要用到 */
  std::vector<N - 2> remove(const direction i, const direction j) {
    std::vector<N - 2> res;
    direction index = 0;
    for (direction k = 0; k < N; k++) {
      if ((k != i) && (k != j)) {
        res[index] = values[k];
        index++;
      }
    }
    return res;
  }
};  // namespace INTERNAL

/*! 将std::vector转化为一维下标，Rank从Nrank到0循环 */
/*! 例如：对于3维3阶张量， {1,1,0} -> 12 */
template <direction Rank, direction Ndim, direction Ncmpts>
struct IndexLinearization {
  template <direction Nrank>
  static inline direction get(std::vector<Nrank> const& indices) {
    return indices[Rank - 1] +
           Ndim * IndexLinearization<Rank - 1, Ndim, Ncmpts>::get(indices);
  }
};
template <direction Ndim, direction Ncmpts>
struct IndexLinearization<0, Ndim, Ncmpts> {
  template <direction Nrank>
  static inline direction get(std::vector<Nrank> const& indices) {
    return 0;
  }
};

}  // namespace INTERNAL

} /* End namespace HSF */

#endif  // 0
#endif  /* end of include guard: NUMERICS_TENSOR_INDEXARRAY_HPP */
