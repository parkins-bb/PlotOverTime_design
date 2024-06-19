/*! \file tensorSpace.hpp
 *  \author Zhao Chengpeng
 *  \date 2022-01-07
 *  \modified 2012-09-23
 *  \email chengpeng_zhao@foxmail.com
 *  \brief templated tensor space
 */

#ifndef NUMERICS_TENSOR_TENSORSPACE_HPP
#define NUMERICS_TENSOR_TENSORSPACE_HPP

#define DEFAULT_WID 8

#include <iomanip>

// scalar.hpp必须在ops.hpp前面
#include "scalar.hpp"
// #include "indexstd::vector.hpp"
#include "ops.hpp"
#include "zero.hpp"

typedef unsigned int direction;

namespace HSF {

/*! General looping form. Executing at index \<I\> with termination at \<N\> */
template <direction N, direction I>
struct TensorSpaceOps {
  /*! I=0,...,N : ts[I]=Op(s) */
  template <class T, class S, class EqOp>
  static inline void eqOpS(T& ts, const S& s, EqOp eo) {
    eo(ts[I], s);
    TensorSpaceOps<N, I + 1>::eqOpS(ts, s, eo);
  }

  /*! I=0,...,N : s=Op(ts[I]) */
  template <class S, class T, class EqOp>
  static inline void SeqOp(S& s, const T& ts, EqOp eo) {
    eo(s, ts[I]);
    TensorSpaceOps<N, I + 1>::SeqOp(s, ts, eo);
  }

  /*! I=0,...,N : ts1[I]=Op(ts2[I]) */
  template <class T1, class T2, class EqOp>
  static inline void eqOp(T1& ts1, const T2& ts2, EqOp eo) {
    eo(ts1[I], ts2[I]);
    TensorSpaceOps<N, I + 1>::eqOp(ts1, ts2, eo);
  }

  /*! I=0,...,N : ts1[I]=Op(ts1[I],s) */
  template <class T, class T1, class S, class Op>
  static inline void opTS(T& ts, const T1& ts1, const S& s, Op o) {
    ts[I] = o(ts1[I], s);
    TensorSpaceOps<N, I + 1>::opTS(ts, ts1, s, o);
  }

  /*! I=0,...,N : ts1[I]=Op(s,ts1[I]) */
  template <class T, class S, class T1, class Op>
  static inline void opST(T& ts, const S& s, const T1& ts1, Op o) {
    ts[I] = o(s, ts1[I]);
    TensorSpaceOps<N, I + 1>::opST(ts, s, ts1, o);
  }

  /*! I=0,...,N : ts[I]=Op(ts1[I],ts2[I]) */
  template <class T, class T1, class Op>
  static inline void op(T& ts, const T1& ts1, const T1& ts2, Op o) {
    ts[I] = o(ts1[I], ts2[I]);
    TensorSpaceOps<N, I + 1>::op(ts, ts1, ts2, o);
  }
};
/*! Loop termination form, when index and loop count \<N\> are identical */
template <direction N>
struct TensorSpaceOps<N, N> {
  template <class T, class S, class EqOp>
  static inline void eqOpS(T&, const S&, EqOp) {}

  template <class S, class T, class EqOp>
  static inline void SeqOp(S&, const T&, EqOp) {}

  template <class T1, class T2, class EqOp>
  static inline void eqOp(T1&, const T2&, EqOp) {}

  template <class T, class T1, class S, class Op>
  static inline void opTS(T& ts, const T1&, const S&, Op) {}

  template <class T, class S, class T1, class Op>
  static inline void opST(T& ts, const S&, const T1&, Op) {}

  template <class T, class T1, class Op>
  static inline void op(T& ts, const T1&, const T1&, Op) {}
};

} /* End namespace HSF */

namespace HSF {

/*! 张量类，Rank>=1，张量的维度和阶数不限制，张量各个维度是相同的
 * 与OpenFOAM的VectorSpace有些类似
 *  \tparam Cmpt 元素类型(Component Type)
 *  \tparam Rank 张量阶数(Rank)
 *  \tparam Ndim 维度(Dimension)
 *  \tparam Ncmpts 元素个数
 *  三者需要满足关系：(\f$\mathrm{Ncmpts}={\mathrm{Ndim}}^{\mathrm{Rank}}\f$)
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
class TensorSpace {
 protected:
  // 一维数组形式储存张量数据
  Cmpt t_[Ncmpts];
  //- Tensor Rank
  static const direction rank_ = Rank;
  //- Dimensionality of space
  static const direction dim_ = Ndim;
  //- Number of components in this tensor space
  static const direction nComponents_ = Ncmpts;

 public:
  // * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * *

  /*! 不进行赋值 */
  inline TensorSpace() {
#ifdef DEBUG
    // 检查Rank、Ndim与Ncmpts是否正确
    if (math::notEqual(Ncmpts, ::pow(Ndim, Rank))) {
      std::cerr << "Template parameter Error!" << std::endl;
      exit(1);
    }
#endif
  }

  /*! 赋值为0 */
  inline TensorSpace(const HSF::zero) {
#ifdef DEBUG
    if (math::notEqual(Ncmpts, ::pow(Ndim, Rank))) {
      std::cerr << "Template parameter Error!" << std::endl;
      exit(1);
    }
#endif
    TensorSpaceOps<Ncmpts, 0>::eqOpS(*this, 0, eqOp<Cmpt>());
  }

  /*! const Cmpt (&t)[Ncmpts] 而不是 const Cmpt t[Ncmpts]
   * 这样可以像下面这样直接传数组进去：
   * HSF::TensorSpace<scalar, 2, 3, 9> t1({1, 0, -2, -3, 1, 4, 2, -3, 4});
   */
  inline TensorSpace(const Cmpt (&t)[Ncmpts]) {
#ifdef DEBUG
    if (math::notEqual(Ncmpts, ::pow(Ndim, Rank))) {
      std::cerr << "Template parameter Error!" << std::endl;
      exit(1);
    }
#endif
    std::copy(t, t + Ncmpts, t_);
  }

  /*! 所有元素赋值为s */
  inline TensorSpace(const Cmpt s) {
    TensorSpaceOps<Ncmpts, 0>::eqOpS(*this, s, eqOp<Cmpt>());
  }

  /*! Copy construct */
  inline TensorSpace(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
    TensorSpaceOps<Ncmpts, 0>::eqOp(*this, ts, eqOp<Cmpt>());
  }

  // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * *
  static direction rank() { return rank_; }
  static direction dim() { return dim_; }
  static direction nComponents() { return nComponents_; }

  /*! Return const pointer to the first data element */
  inline const Cmpt* cdata() const { return t_; }

  /*! Return pointer to the first data element */
  inline Cmpt* data() { return t_; }

  /*! iterator */
  inline Cmpt* begin() { return t_; }
  inline Cmpt* end() { return (t_ + Ncmpts); }
  inline const Cmpt* cbegin() { return t_; }
  inline const Cmpt* cend() { return (t_ + Ncmpts); }

  /*! 元素值全部设为0 */
  inline void clear() {
    TensorSpaceOps<Ncmpts, 0>::eqOpS(*this, 0, eqOp<Cmpt>());
  }

  /*! Return a TensorSpace with all elements = s */
  inline static TensorSpace<Cmpt, Rank, Ndim, Ncmpts> uniform(const Cmpt s) {
    TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
    TensorSpaceOps<Ncmpts, 0>::eqOpS(t, s, eqOp<Cmpt>());
    return t;
  }

  // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * *

  /*! 元素访问运算符 const */
  inline const Cmpt& operator[](const direction d) const {
#ifdef DEBUG
    if (d >= Ncmpts) {
      std::cerr << "Index out of range" << std::endl;
    }
#endif
    return t_[d];
  }

  /*! 元素访问运算符  */
  inline Cmpt& operator[](const direction d) {
#ifdef DEBUG
    if (d >= Ncmpts) {
      std::cerr << "Index out of range" << std::endl;
    }
#endif
    return t_[d];
  }

  /*! 通过不定参数的index访问元素 const */
  template <typename... Indices>
  inline const Cmpt& operator()(const direction& v1, const direction& v2,
                                const Indices&... ind) const {
    return (*this)(v1 * Ndim + v2, ind...);
  }
  inline const Cmpt& operator()(const direction& v) const {
    return t_[v];
#ifdef DEBUG
    if (v >= Ncmpts) {
      std::cerr << "Index out of range" << std::endl;
    }
#endif
  }
  /*! 通过不定参数的index访问元素 const */
  template <typename... Indices>
  inline Cmpt& operator()(const direction& v1, const direction& v2,
                          const Indices&... ind) {
    return (*this)(v1 * Ndim + v2, ind...);
  }
  inline Cmpt& operator()(const direction& v) {
    return t_[v];
#ifdef DEBUG
    if (v >= Ncmpts) {
      std::cerr << "Index out of range" << std::endl;
    }
#endif
  }

  /*! I=0,...,Ncmpts : (*this)[I]=ts[I] */
  inline void operator=(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
    TensorSpaceOps<Ncmpts, 0>::eqOp(*this, ts, eqOp<Cmpt>());
  }
  /*! I=0,...,Ncmpts : (*this)[I]=0 */
  inline void operator=(const HSF::zero) {
    TensorSpaceOps<Ncmpts, 0>::eqOpS(*this, 0, eqOp<Cmpt>());
  }
  /*! I=0,...,Ncmpts : (*this)[I]+=ts[I] */
  inline void operator+=(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
    TensorSpaceOps<Ncmpts, 0>::eqOp(*this, ts, plusEqOp<Cmpt>());
  }
  /*! I=0,...,Ncmpts : (*this)[I]-=ts[I] */
  inline void operator-=(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
    TensorSpaceOps<Ncmpts, 0>::eqOp(*this, ts, minusEqOp<Cmpt>());
  }
  /*! I=0,...,Ncmpts : (*this)[I]*=s */
  inline void operator*=(const scalar s) {
    TensorSpaceOps<Ncmpts, 0>::eqOpS(*this, s, multiplyEqOp2<Cmpt, scalar>());
  }
  /*! I=0,...,Ncmpts : (*this)[I]/=s */
  inline void operator/=(const scalar s) {
    TensorSpaceOps<Ncmpts, 0>::eqOpS(*this, s, divideEqOp2<Cmpt, scalar>());
  }

  // inline bool operator==(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts) {
  //   bool eq = true;
  //   for (direction i = 0; i < Ncmpts; ++i) {
  //     if (!(eq &= (equal((*this)[i], ts[i])))) break;
  //   }
  //   return eq;
  // }
  // inline bool operator!=(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts) {
  //   return !(*this == ts);
  // }

};  // End class TensorSpace

} /* End namespace HSF */

namespace HSF {
// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * *

/*! 计算张量所有元素的平方和
 *  I=0,...,Ncmpts : ms+=(ts[I])^2
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline scalar magSqr(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  scalar ms = math::magSqr(ts[0]);
  TensorSpaceOps<Ncmpts, 1>::SeqOp(ms, ts, plusEqMagSqrOp2<scalar, Cmpt>());
  return ms;
}

/*! 计算张量所有元素的平方和开根号
 *  I=0,...,Ncmpts : ms+=(ts[I])^2, ms=sqrt(ms)
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline scalar mag(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  return ::sqrt(magSqr(ts));
}

/*! 使张量所有元素的平方和变为1
 *  tsNew=ts/mag(ts)
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> normalise(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  const scalar s(mag(ts));
  return s < math::ROOTVSMALL ? Zero : ts / s;
}

/*! 两个阶数和维度相同的张量，返回元素各自相乘后得到的张量
 *  I=0,...,Ncmpts : tsNew[I]=ts1[I]*ts2[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> cmptMultiply(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts2) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::op(t, ts1, ts2, cmptMultiplyOp<Cmpt>());
  return t;
}

/*! 两个阶数和维度相同的张量，返回元素各自求指数后得到的张量
 *  I=0,...,Ncmpts : tsNew[I]=(ts1[I])^ts2[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> cmptPow(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts2) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::op(t, ts1, ts2, cmptPowOp<Cmpt>());
  return t;
}

/*! 两个阶数和维度相同的张量，返回元素各自相除后得到的张量
 *  I=0,...,Ncmpts : tsNew[I]=ts1[I]/ts2[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> cmptDivide(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts2) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::op(t, ts1, ts2, cmptDivideOp<Cmpt>());
  return t;
}

/*! 对张量每个元素各自执行stabilise函数
 *  I=0,...,Ncmpts : tsNew[I]=ts[I]+small, if ts[I]>=0
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> stabilise(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts, const Cmpt small) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::opTS(t, ts, small, stabiliseOp<Cmpt>());
  return t;
}

/*! 找出张量元素的最大值
 *  cMax=max(ts[I]), I=0,...,Ncmpts
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline Cmpt cmptMax(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  Cmpt cMax = ts[0];
  TensorSpaceOps<Ncmpts, 1>::SeqOp(cMax, ts, maxEqOp<Cmpt>());
  return cMax;
}

/*! 找出张量元素的最小值
 *  cMin=min(ts[I]), I=0,...,Ncmpts
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline Cmpt cmptMin(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  Cmpt cMin = ts[0];
  TensorSpaceOps<Ncmpts, 1>::SeqOp(cMin, ts, minEqOp<Cmpt>());
  return cMin;
}

/*! 求张量各元素之和
 *  sum+=(ts[I]), I=0,...,Ncmpts
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline Cmpt cmptSum(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  Cmpt sum = ts[0];
  TensorSpaceOps<Ncmpts, 1>::SeqOp(sum, ts, plusEqOp<Cmpt>());
  return sum;
}

/*! 求张量各元素平均值
 *  avg+=(ts[I])/Ncmpts, I=0,...,Ncmpts
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline Cmpt cmptAvg(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  return cmptSum(ts) / Ncmpts;
}

/*! 求张量各元素之积
 *  product*=(ts[I]), I=0,...,Ncmpts
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline Cmpt cmptProduct(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  Cmpt product = ts[0];
  TensorSpaceOps<Ncmpts, 1>::SeqOp(product, ts, multiplyEqOp<Cmpt>());
  return product;
}

/*! 对张量各元素执行Sqr函数
 *  I=0,...,Ncmpts : tsNew[I]=ts[I]*ts[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> cmptSqr(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::eqOp(t, ts, eqSqrOp<Cmpt>());
  return t;
}

/*! 对张量各元素执行Mag函数
 *  I=0,...,Ncmpts : tsNew[I]=|ts[I]|
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> cmptMag(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::eqOp(t, ts, eqMagOp<Cmpt>());
  return t;
}

/*! 对张量各元素执行MagSqr函数
 *  I=0,...,Ncmpts : tsNew[I]=|ts[I]|^2
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> cmptMagSqr(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::eqOp(t, ts, eqMagSqrOp<Cmpt>());
  return t;
}

/*! 对两张量的各元素执行max函数
 *  I=0,...,Ncmpts : tsNew[I]=max(ts1[I],ts2[I])
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> max(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts2) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::op(t, ts1, ts2, maxOp<Cmpt>());
  return t;
}

/*! 对两张量的各元素执行min函数
 *  I=0,...,Ncmpts : tsNew[I]=min(ts1[I],ts2[I])
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> min(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts2) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::op(t, ts1, ts2, minOp<Cmpt>());
  return t;
}

/*! 对两张量的各元素执行minMod函数
 *  I=0,...,Ncmpts : tsNew[I]=ts1[I], if |ts1[I]|<|ts2[I]|
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> minMod(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts2) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::op(t, ts1, ts2, minModOp<Cmpt>());
  return t;
}

/*! 张量积，并乘, dyadic/dyad product
 *  t_ik=v_i*v_k
 */
template <class Cmpt, direction Rank1, direction Rank2, direction Ndim,
          direction Ncmpts1, direction Ncmpts2>
TensorSpace<Cmpt, Rank1 + Rank2, Ndim, Ncmpts1 * Ncmpts2> tensorProduct(
    const TensorSpace<Cmpt, Rank1, Ndim, Ncmpts1>& ts1,
    const TensorSpace<Cmpt, Rank2, Ndim, Ncmpts2>& ts2) {
  TensorSpace<Cmpt, Rank1 + Rank2, Ndim, Ncmpts1 * Ncmpts2> ts;
  for (direction i = 0; i < Ncmpts1; ++i) {
    for (direction j = 0; j < Ncmpts2; ++j) {
      ts[i * Ncmpts2 + j] = ts1[i] * ts2[j];
    }
  }
  return ts;
}

namespace INTERNAL {
// 协助进行缩并等操作
template <direction Rank, direction Ndim>
struct Linearization {
  static inline direction get(const direction* indices) {
    return indices[Rank - 1] +
           Ndim * Linearization<Rank - 1, Ndim>::get(indices);
  }
};
template <direction Ndim>
struct Linearization<0, Ndim> {
  static inline direction get(const direction* indices) { return 0; }
};
template <direction Rank, direction Ndim>
struct LinearizationRemove2 {
  static inline direction get(const direction* indices, direction i,
                              direction j) {
    if (Rank - 1 == i || Rank - 1 == j) {
      return LinearizationRemove2<Rank - 1, Ndim>::get(indices, i, j);
    } else {
      return indices[Rank - 1] +
             Ndim * LinearizationRemove2<Rank - 1, Ndim>::get(indices, i, j);
    }
  }
};
template <direction Ndim>
struct LinearizationRemove2<0, Ndim> {
  static inline direction get(const direction* indices, direction i,
                              direction j) {
    return 0;
  }
};

}  // namespace INTERNAL

/*! Rank>2的张量缩并
 *  t_ijj
 *  \pram [in] i j 需要缩并的张量指标，范围是0 ~ Rank-1，默认缩并最开始两个指标
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
TensorSpace<Cmpt, Rank - 2, Ndim, Ncmpts / Ndim / Ndim> reduction(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts, direction i = 0,
    direction j = 1) {
  if (i > j) {
    direction tmp = j;
    j = i;
    i = tmp;
  } else if (i == j) {
    std::cerr << "Reduction requires two different index!" << std::endl;
    exit(1);
  }
  TensorSpace<Cmpt, Rank - 2, Ndim, Ncmpts / Ndim / Ndim> ts1(Zero);
  direction index[Rank + 1] = {0};
  // Note: there is an extra element at the end of the array, in order to keep
  // track of whether to exit the array.

  direction p = 0;  // Used to increment all of the indicies correctly, at the
                    // end of each loop.
  while (index[Rank] == 0) {
    // DO STUFF HERE. Pretend you're inside your nested for loops. The more
    // usual i,j,k,... have been replaced here with index[0], index[1], ...,
    // index[n-1]. Now, after you've done your stuff, we need to increment all
    // of the indicies correctly.
    index[j] = index[i];  // 缩并
    ts1(INTERNAL::LinearizationRemove2<Rank, Ndim>::get(index, i, j)) +=
        ts(INTERNAL::Linearization<Rank, Ndim>::get(index));
    index[0]++;
    while (index[p] == Ndim) {
      //(Initially index[0]) If this is true, then index[p] is reset to 0, and
      // index[p+1] is incremented.
      index[p] = 0;
      index[++p]++;  // increase p by 1, and increase the next (p+1)th index
      if (index[p] != Ndim) p = 0;
      // resets p when it actually needs to be reset
    }
  }
  return ts1;
}

/*! Rank=2的张量缩并，由于返回标量，这里单独写成一个函数
 *  \pram [in] i j 需要缩并的张量指标，范围是0 ~ Rank-1，默认缩并最开始两个指标
 */
template <class Cmpt, direction Ndim, direction Ncmpts>
Cmpt reduction(const TensorSpace<Cmpt, 2, Ndim, Ncmpts>& ts,
               const direction i = 0, const direction j = 1) {
  Cmpt res = 0;
  for (direction m = 0; m < Ndim; ++m) {
    res += ts(m, m);
  }
  return res;
}

/*! 两个张量的缩并，相当于先进行了并乘运算
 *  \pram [in] i ts1需要缩并的张量指标，范围 0~Rank1-1
 *  \pram [in] j ts2需要缩并的张量指标，范围 0~Rank2-1
 */
template <class Cmpt, direction Rank1, direction Rank2, direction Ndim,
          direction Ncmpts1, direction Ncmpts2>
TensorSpace<Cmpt, Rank1 + Rank2 - 2, Ndim, Ncmpts1 * Ncmpts2 / Ndim / Ndim>
reduction(const TensorSpace<Cmpt, Rank1, Ndim, Ncmpts1>& ts1, const direction i,
          const TensorSpace<Cmpt, Rank2, Ndim, Ncmpts2>& ts2,
          const direction j) {
  TensorSpace<Cmpt, Rank1 + Rank2, Ndim, Ncmpts1* Ncmpts2> ts =
      tensorProduct(ts1, ts2);
  return reduction(ts, i, Rank1 + j);
}

/*!
 * 两个张量的内积，相当于先进行了并乘运算，
 * 再对ts1的最后一个指标及ts2的第一个指标进行缩并
 */
template <class Cmpt, direction Rank1, direction Rank2, direction Ndim,
          direction Ncmpts1, direction Ncmpts2>
inline TensorSpace<Cmpt, Rank1 + Rank2 - 2, Ndim,
                   Ncmpts1 * Ncmpts2 / Ndim / Ndim>
dotProduct(const TensorSpace<Cmpt, Rank1, Ndim, Ncmpts1>& ts1,
           const TensorSpace<Cmpt, Rank2, Ndim, Ncmpts2>& ts2) {
  TensorSpace<Cmpt, Rank1 + Rank2, Ndim, Ncmpts1* Ncmpts2> ts =
      tensorProduct(ts1, ts2);
  return reduction(ts, Rank1 - 1, Rank1);
}

/*! 两个矢量的内积，返回一个标量 */
template <class Cmpt, direction Ndim, direction Ncmpts1, direction Ncmpts2>
inline Cmpt dotProduct(const TensorSpace<Cmpt, 1, Ndim, Ncmpts1>& ts1,
                       const TensorSpace<Cmpt, 1, Ndim, Ncmpts2>& ts2) {
  TensorSpace<Cmpt, 2, Ndim, Ncmpts1* Ncmpts2> ts = tensorProduct(ts1, ts2);
  return reduction(ts);
}

/*! 交换两个index，相当于转置的一般情况，要求Rank>=2
 *  \pram [in] i 需要交换的指标，范围 0~Rank-1
 *  \pram [in] j 另一个需要交换的指标，范围 0~Rank-1
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
TensorSpace<Cmpt, Rank, Ndim, Ncmpts> exchange(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts, const direction i = 0,
    const direction j = 1) {
  if (i == j) {
    std::cerr << "Exchange requires two different index!" << std::endl;
    exit(1);
  }
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts1(Zero);
  direction index[Rank + 1] = {0};
  direction p = 0;
  direction tmp;
  Cmpt res;
  while (index[Rank] == 0) {
    // 相当于Rank个for循环，index[0],...,index[Rank-1]分别从0到Ndim
    tmp = index[j];
    res = ts(INTERNAL::Linearization<Rank, Ndim>::get(index));
    index[j] = index[i];
    index[i] = tmp;
    ts1(INTERNAL::Linearization<Rank, Ndim>::get(index)) = res;  // 交换后的位置
    index[i] = index[j];
    index[j] = tmp;  // 需要将index还原
    index[0]++;
    while (index[p] == Ndim) {
      index[p] = 0;
      index[++p]++;
      if (index[p] != Ndim) p = 0;
    }
  }
  return ts1;
}

/*! 二阶各向同性张量
 * alpha=1时，为二阶单位张量，即\f$ \delta _{\mathrm{ij}} \f$ 符号
 */
template <class Cmpt, direction Ndim = 3>
inline TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> kronecker(const Cmpt alpha) {
  TensorSpace<Cmpt, 2, Ndim, Ndim * Ndim> ts;
  for (direction i = 0; i < Ndim; ++i) {
    for (direction j = 0; j < Ndim; ++j) {
      if (i == j)
        ts(i, j) = alpha;
      else
        ts(i, j) = 0;
    }
  }
  return ts;
}
/*! 三阶各向同性张量
 *  alpha=1时，为三阶单位张量，即Levi-Civita symbol
 *  \f$ \epsilon _{\mathrm{ijk}} \f$
 */
template <class Cmpt, direction Ndim = 3>
TensorSpace<scalar, 3, Ndim, Ndim * Ndim * Ndim> permutation(const Cmpt alpha) {
  TensorSpace<scalar, 3, Ndim, Ndim * Ndim * Ndim> ts(Zero);
  for (direction i = 0; i < Ndim; ++i) {
    for (direction j = 0; j < Ndim; ++j) {
      for (direction k = 0; k < Ndim; ++k) {
        ts(i, j, k) =
            alpha * math::sgn(static_cast<scalar>(j) - static_cast<scalar>(i)) *
            math::sgn(static_cast<scalar>(k) - static_cast<scalar>(i)) *
            math::sgn(static_cast<scalar>(k) - static_cast<scalar>(j));
      }
    }
  }
  return ts;
}
/*! 四阶各向同性张量
 * 与弹性四阶张量有关的四阶单位张量：
 * Iso : alpha=1/3, beta=0, gamma=0
 * 体积四阶单位张量（ volumetric fourth order unit tensor）
 * Sym : alpha=0, beta=1/2, gamma=1/2
 * 对称四阶单位张量（ symmetric fourth order unit tensor）
 * SymDev : alpha=-1/3, beta=1/2, gamma=1/2
 * 偏差四阶单位张量（deviatoric fourth order unit tensor）
 * Skew : alpha=0, beta=1/2, gamma=-1/2
 * 反对称四阶单位张量（ skew-symmetric fourth order unit tensor）
 */
template <class Cmpt, direction Ndim = 3>
TensorSpace<scalar, 4, Ndim, Ndim * Ndim * Ndim * Ndim> rank4GeneralIsotropic(
    const Cmpt alpha, const Cmpt beta, const Cmpt gamma) {
  TensorSpace<scalar, 4, Ndim, Ndim * Ndim * Ndim * Ndim> ts4;
  TensorSpace<scalar, 2, Ndim, Ndim* Ndim> ts2 = kronecker<Cmpt, Ndim>(1.0);
  for (direction i = 0; i < Ndim; ++i) {
    for (direction j = 0; j < Ndim; ++j) {
      for (direction k = 0; k < Ndim; ++k) {
        for (direction l = 0; l < Ndim; ++l) {
          ts4(i, j, k, l) = alpha * ts2(i, j) * ts2(k, l) +
                            beta * ts2(i, k) * ts2(j, l) +
                            gamma * ts2(i, l) * ts2(j, k);
        }
      }
    }
  }
  return ts4;
}

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

/*! 输入流 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
std::istream& operator>>(std::istream& is,
                         const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  for (direction i = 0; i < Ncmpts; ++i) {
    is >> ts[i];
  }
  return is;
};

/*! 一般形式输出 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
std::ostream& operator<<(std::ostream& os,
                         const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts) {
  os << "{" << ts[0];
  for (direction i = 1; i < Ncmpts; ++i) {
    os << " , " << ts[i];
  }
  os << "}";
  return os;
};

/*! 矢量特殊形式输出 */
template <class Cmpt, direction Ndim, direction Ncmpts>
std::ostream& operator<<(std::ostream& os,
                         const TensorSpace<Cmpt, 1, Ndim, Ncmpts>& ts) {
  for (direction i = 0; i < Ndim; ++i) {
    os << std::setw(DEFAULT_WID) << ts[i] << " ";
  }
  return os;
};
/*! 二阶张量特殊形式输出 */
template <class Cmpt, direction Ndim, direction Ncmpts>
std::ostream& operator<<(std::ostream& os,
                         const TensorSpace<Cmpt, 2, Ndim, Ncmpts>& ts) {
  for (direction i = 0; i < Ndim; ++i) {
    for (direction j = 0; j < Ndim; ++j)
      os << std::setw(DEFAULT_WID) << ts(i, j) << " ";
    if (i != Ndim - 1) os << std::endl;
  }
  return os;
};
/*! 三阶张量特殊形式输出 */
template <class Cmpt, direction Ndim, direction Ncmpts>
std::ostream& operator<<(std::ostream& os,
                         const TensorSpace<Cmpt, 3, Ndim, Ncmpts>& ts) {
  for (direction j = 0; j < Ndim; ++j) {
    for (direction i = 0; i < Ndim; ++i) {
      for (direction k = 0; k < Ndim; ++k)
        os << std::setw(DEFAULT_WID) << ts(i, j, k) << " ";
    }
    if (j != Ndim - 1) os << std::endl;
  }
  return os;
};
/*! 四阶张量特殊形式输出 */
template <class Cmpt, direction Ndim, direction Ncmpts>
std::ostream& operator<<(std::ostream& os,
                         const TensorSpace<Cmpt, 4, Ndim, Ncmpts>& ts) {
  for (direction i = 0; i < Ndim; ++i) {
    for (direction k = 0; k < Ndim; ++k) {
      for (direction j = 0; j < Ndim; ++j) {
        for (direction l = 0; l < Ndim; ++l)
          os << std::setw(DEFAULT_WID) << ts(i, j, k, l) << " ";
        if (j != Ndim - 1) os << "| ";
      }
      if (!(i == Ndim - 1 && k == Ndim - 1)) os << std::endl;
    }
    if (i != Ndim - 1) {
      for (direction p = 0;
           p < Ndim * Ndim * (DEFAULT_WID + 1) - 1 + (Ndim - 1) * 2; ++p)
        os << "-";
      os << std::endl;
    }
  }
  return os;
};

/*! 张量取负号
 * I=0,...,Ncmpts : tsNew[I]=-ts[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> operator-(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::eqOp(t, ts, eqMinusOp<Cmpt>());
  return t;
}

/*! 两个阶数和维度相同的张量相加
 * I=0,...,Ncmpts : tsNew[I]=ts1[I]+ts2[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> operator+(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts>& ts1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts2) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::op(t, ts1, ts2, plusOp<Cmpt>());
  return t;
}

/*! 两个阶数和维度相同的张量相减
 * I=0,...,Ncmpts : tsNew[I]=ts1[I]-ts2[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> operator-(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts1,
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts2) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::op(t, ts1, ts2, minusOp<Cmpt>());
  return t;
}

/*! 张量乘标量
 * I=0,...,Ncmpts : tsNew[I]=s*ts[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> operator*(
    scalar s, const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::opST(t, s, ts, multiplyOp3<Cmpt, scalar, Cmpt>());
  return t;
}

/*! 张量乘标量2
 * I=0,...,Ncmpts : tsNew[I]=ts[I]*s
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> operator*(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts, scalar s) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::opTS(t, ts, s, multiplyOp3<Cmpt, Cmpt, scalar>());
  return t;
}

/*! 两维度相同的张量并乘，返回更高阶的张量(Rank1+Rank2)
 * 采用 * 表示并乘，与OpenFOAM保持一致
 * 标量乘张量也相当于并乘，用 * 符合习惯
 */
template <class Cmpt, direction Rank1, direction Rank2, direction Ndim,
          direction Ncmpts1, direction Ncmpts2>
inline TensorSpace<Cmpt, Rank1 + Rank2, Ndim, Ncmpts1 * Ncmpts2> operator*(
    const TensorSpace<Cmpt, Rank1, Ndim, Ncmpts1>& ts1,
    const TensorSpace<Cmpt, Rank2, Ndim, Ncmpts2>& ts2) {
  return tensorProduct(ts1, ts2);
}

/*! 张量除标量
 * I=0,...,Ncmpts : tsNew[I]=ts[I]/s
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline TensorSpace<Cmpt, Rank, Ndim, Ncmpts> operator/(
    const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts, scalar s) {
  TensorSpace<Cmpt, Rank, Ndim, Ncmpts> t;
  TensorSpaceOps<Ncmpts, 0>::opTS(t, ts, s, divideOp3<Cmpt, Cmpt, scalar>());
  return t;
}

/*! 两维度相同的张量进行内积，返回Rank1+Rank2-2的张量
 * 采用 & 表示内积，与OpenFOAM保持一致
 * 注意 & 在C/C++中的默认优先级较低，有时需要加括号
 */
template <class Cmpt, direction Rank1, direction Rank2, direction Ndim,
          direction Ncmpts1, direction Ncmpts2>
inline TensorSpace<Cmpt, Rank1 + Rank2 - 2, Ndim,
                   Ncmpts1 * Ncmpts2 / Ndim / Ndim>
operator&(const TensorSpace<Cmpt, Rank1, Ndim, Ncmpts1>& ts1,
          const TensorSpace<Cmpt, Rank2, Ndim, Ncmpts2>& ts2) {
  return dotProduct(ts1, ts2);
}

/*! 两个矢量的内积，返回一个标量 */
template <class Cmpt, direction Ndim, direction Ncmpts1, direction Ncmpts2>
inline Cmpt operator&(const TensorSpace<Cmpt, 1, Ndim, Ncmpts1>& ts1,
                      const TensorSpace<Cmpt, 1, Ndim, Ncmpts2>& ts2) {
  return dotProduct(ts1, ts2);
}

/*! 两维度相同的张量进行双内积，返回Rank1+Rank2-4的张量
 * 采用 && 表示双内积，与OpenFOAM保持一致
 */
template <class Cmpt, direction Rank1, direction Rank2, direction Ndim,
          direction Ncmpts1, direction Ncmpts2>
inline TensorSpace<Cmpt, Rank1 + Rank2 - 4, Ndim,
                   Ncmpts1 * Ncmpts2 / Ndim / Ndim / Ndim / Ndim>
operator&&(const TensorSpace<Cmpt, Rank1, Ndim, Ncmpts1>& ts1,
           const TensorSpace<Cmpt, Rank2, Ndim, Ncmpts2>& ts2) {
  TensorSpace<Cmpt, Rank1 + Rank2, Ndim, Ncmpts1* Ncmpts2> ts =
      tensorProduct(ts1, ts2);
  return reduction(reduction(ts, Rank1 - 2, Rank1 + 0), Rank1 - 2, Rank1 - 1);
}
/*! 两维度相同的二阶张量进行双内积，返回标量 */
template <class Cmpt, direction Ndim, direction Ncmpts1, direction Ncmpts2>
inline Cmpt operator&&(const TensorSpace<Cmpt, 2, Ndim, Ncmpts1>& ts1,
                       const TensorSpace<Cmpt, 2, Ndim, Ncmpts2>& ts2) {
  TensorSpace<Cmpt, 4, Ndim, Ncmpts1* Ncmpts2> ts = tensorProduct(ts1, ts2);
  return reduction(reduction(ts, 0, 2));
}

/*! 两阶数和维度相同的张量判断是否相等
 * ts1=ts2 ,if I=0,...,Ncmpts : ts1[I]=ts2[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
bool operator==(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts1,
                const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts2) {
  bool eq = true;
  for (direction i = 0; i < Ncmpts; ++i) {
    // a&=b  ->  a=a&b
    if (!(eq &= (math::equal(ts1[i], ts2[i])))) break;
  }
  return eq;
}

/*! 两阶数和维度相同的张量判断是否不等
 * ts1!=ts2 ,if ts1[I]!=ts2[I]
 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline bool operator!=(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts1,
                       const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts2) {
  return !(ts1 == ts2);
}

/*! 两阶数和维度相同的张量元素判断大小 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
bool operator>(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts1,
               const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts2) {
  // bool gt = true;
  // for (direction i = 0; i < Ncmpts; ++i) {
  //  if (!(gt &= ts1[i] > ts2[i])) break;
  //}
  // return gt;
  return (mag(ts1) > mag(ts2));
}
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline bool operator<=(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts1,
                       const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts2) {
  return !(ts1 > ts2);
}

/*! 两阶数和维度相同的张量元素判断大小 */
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
bool operator<(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts1,
               const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts2) {
  // bool lt = true;
  // for (direction i = 0; i < Ncmpts; ++i) {
  //  if (!(lt &= ts1[i] < ts2[i])) break;
  //}
  // return lt;
  return (mag(ts1) < mag(ts2));
}
template <class Cmpt, direction Rank, direction Ndim, direction Ncmpts>
inline bool operator>=(const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts1,
                       const TensorSpace<Cmpt, Rank, Ndim, Ncmpts> ts2) {
  return !(ts1 < ts2);
}

} /* End namespace HSF */

#endif /* end of include guard: NUMERICS_TENSOR_TENSORSPACE_HPP */
