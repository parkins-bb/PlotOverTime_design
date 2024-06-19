/*! \file equation.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-03-08
 *  \brief 方程容器
 */

#ifndef NUMERICS_EQUATION_EQUATION_HPP
#define NUMERICS_EQUATION_EQUATION_HPP
#include "matrix.hpp"

namespace HSF {

/**
 * @brief Equation
 * 存储了方程相关信息：矩阵A、场变量x和右端项系数b的指针。
 * 修改和获取矩阵A、场变量x及右端项系数b
 */
class Field;
template <typename Cmpt>
class Equation : Matrix<Cmpt> {
 private:
  Field *x_;
  Field *b_;
  std::string name_;

 public:
  Equation();
  ~Equation();
  /*! X field对象 */
  Field *x();
  const Field *x() const;

  /*! B field对象 */
  Field *b();
  const Field *b() const;

  void solve(enum class SOLVERNAME);
};

} /* End namespace HSF */

#endif /* end of include guard: NUMERICS_EQUATION_EQUATION_HPP */
