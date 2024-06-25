/*! \file Scalar.H
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-01-19
 *  \brief 定义了Scalar的常用函数，Scalar可以为double/float
 */
namespace HSF {
namespace math {

/*! 定义double/float类型常用函数 */
transFunc(sqrt)
transFunc(cbrt)
transFunc(exp)
transFunc(log)
transFunc(log10)
transFunc(log2)
transFunc(sin)
transFunc(cos)
transFunc(tan)
transFunc(asin)
transFunc(acos)
transFunc(atan)
transFunc(sinh)
transFunc(cosh)
transFunc(tanh)
transFunc(asinh)
transFunc(acosh)
transFunc(atanh)
transFunc(fabs)
transFunc(ceil)
transFunc(floor)
transFunc(round)

/*! 计算绝对值
 *  \param[in] s 输入浮点数
 *  \return Scalar 返回 |s|
 */
inline Scalar mag(const Scalar s) { return ::fabs(s); }

/*! 对于给定的直角三角形的两个直角边，求其斜边的长度
 *  \param[in] x y 输入浮点数
 *  \return Scalar 返回 $\sqrt(x^2+y^2)$
 */
inline Scalar hypot(const Scalar x, const Scalar y) { return ::hypot(x, y); }

/*! 两参数atan函数，可以处理分母为0情况
 *  \param[in] x y 输入浮点数
 *  \return Scalar 返回 $atan(y/x)$
 */
inline Scalar atan2(const Scalar y, const Scalar x) { return ::atan2(y, x); }

/*! Return 1 if s is greater_equal zero, or otherwise -1
 *  \param[in] s 输入浮点数
 *  \return Scalar 返回 1.0 or -1.0
 */
inline Scalar sign(const Scalar s) { return (s >= 0) ? 1 : -1; }

/*! Return 1 if s is greater zero, -1 if s is smaller zero, or otherwise zero
 *  \param[in] s 输入浮点数
 *  \return Scalar 返回 1.0 or -1.0 or 0.0
 */
inline Scalar sgn(const Scalar s) {
  if (s > 0) {
    return 1;
  } else if (s == 0) {
    return 0;
  } else {
    return -1;
  }
}

/*!  Return 1 if s is greater than zero, otherwise 1
 *  \param[in] s 输入浮点数
 *  \return Scalar 返回 1.0 or 0.0
 */
inline Scalar pos(const Scalar s) { return (s > 0) ? 1 : 0; }

/*!  判断两浮点数是否相等，差的绝对值与ScalarVSMALL比较
 *  \param[in] s1 s2 输入浮点数
 *  \return bool 返回 true if 相等
 */
inline bool equal(const Scalar s1, const Scalar s2) {
  return ::fabs(s1 - s2) <= ScalarVSMALL;
}

/*!  判断两浮点数是否相等，差的绝对值与ScalarVSMALL比较
 *  \param[in] s1 s2 输入浮点数
 *  \return bool 返回 true if 不相等
 */
inline bool notEqual(const Scalar s1, const Scalar s2) {
  return ::fabs(s1 - s2) > ScalarVSMALL;
}

/*!  Stabilisation around zero for division
 *  \param[in] s1 tol 输入浮点数
 *  \return Scalar 返回 绝对值增大tol后的数
 */
inline Scalar stabilise(const Scalar s, const Scalar tol) {
  if (s >= 0) {
    return s + tol;
  } else {
    return s - tol;
  }
}

/*!  绝对值（模）的平方
 *  \param[in] s  输入浮点数
 *  \return Scalar 返回 |s|^2
 */
inline Scalar magSqr(const Scalar s) { return s * s; }

/*!  平方
 *  \param[in] s  输入浮点数
 *  \return Scalar 返回 s^2
 */
inline Scalar sqr(const Scalar s) { return s * s; }

/*!  乘（各分量）
 *  \param[in] s1 s2  输入浮点数
 *  \return Scalar 返回s1*s2
 */
inline Scalar cmptMultiply(const Scalar s1, const Scalar s2) {
  return s1 * s2;
}

/*!  指数（各分量）
 *  \param[in] s1 s2  输入浮点数
 *  \return Scalar 返回s1^s2
 */
inline Scalar cmptPow(const Scalar s1, const Scalar s2) {
  return pow(s1, s2);
}

/*!  除（各分量）
 *  \param[in] s1 s2  输入浮点数
 *  \return Scalar 返回s1/s2
 */
inline Scalar cmptDivide(const Scalar s1, const Scalar s2) { return s1 / s2; }

/*!  取绝对值更小的数（各分量）
 *  \param[in] s1 s2  输入浮点数
 *  \return Scalar 返回s1 if |s1|<|s2|
 */
inline Scalar minMod(const Scalar s1, const Scalar s2) {
  return (mag(s1) < mag(s2)) ? s1 : s2;
}

} /* End namespace math */
} /* End namespace HSF */
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
