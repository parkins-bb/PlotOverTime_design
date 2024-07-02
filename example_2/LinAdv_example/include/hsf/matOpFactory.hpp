/*! \file matOpFactory.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-07
 *  \brief matrix operation factory
 */
#ifndef NUMERICS_MATRIX_MATOPFACTORY_HPP
#define NUMERICS_MATRIX_MATOPFACTORY_HPP

#include "utilities.h"
#include "hsfVector.hpp"
namespace HSF {
template <typename T>
class Matrix;

enum class MatType { DEFAULT, HYPRE_PARCSR, UNAP_CSR, UNAP_LDU };

#define SWITCHMATOP(opName, ...)                         \
  switch (type) {                                        \
    case MatType::UNAP_CSR:                              \
      MatOp<MatType::UNAP_CSR>::opName(__VA_ARGS__);     \
      break;                                             \
    case MatType::UNAP_LDU:                              \
      MatOp<MatType::UNAP_LDU>::opName(__VA_ARGS__);     \
      break;                                             \
    case MatType::HYPRE_PARCSR:                          \
      MatOp<MatType::HYPRE_PARCSR>::opName(__VA_ARGS__); \
      break;                                             \
    case MatType::DEFAULT:                               \
    default:                                             \
      MatOp<MatType::DEFAULT>::opName(__VA_ARGS__);      \
      break;                                             \
  }

template <typename T>
class MatOpFactory {
 public:
  /// \brief Y+=a*X
  void axpy(MatType type, Matrix<T> *Y, T a, Matrix<T> *X);

  /// \brief Y=a*Y+X
  void aypx(MatType type, Matrix<T> *Y, T a, Matrix<T> *X);

  /// \brief y=A*x
  void mult(MatType type, Matrix<T> *A, Vector<T> *x, Vector<T> *y);

  /// \brief y=alpha*A*x+beta*y
  void multAdd(MatType type, T alpha, Matrix<T> *A, Vector<T> *x, T beta,
               Vector<T> *y);

  /// \brief y=A^T*x
  void multTranspose(MatType type, Matrix<T> *A, Vector<T> *x, Vector<T> *y);

  /// \brief y=alpha*A^T*x+beta*y
  void multTransposeAdd(MatType type, T alpha, Matrix<T> *A, Vector<T> *x,
                        T beta, Vector<T> *y);

  /// \brief B=A^T
  void transpose(MatType type, Matrix<T> *A, Matrix<T> *B);

  /// \brief B=A
  void copy(MatType type, Matrix<T> *A, Matrix<T> *B);

  /// \brief A=a*A
  void scale(MatType type, Matrix<T> *A, T a);

  /// \brief A=diag(l)*A*diag(r)
  void diagonalScale(MatType type, Matrix<T> *A, Vector<T> *l, Vector<T> *r);

  /// \brief diag(A)
  void diag(MatType type, Matrix<T> *A, Vector<T> *d);

  /// \brief A+=a*I
  void shift(MatType type, Matrix<T> *A, T a);

  /// \brief C=A*B
  void matMult(MatType type, Matrix<T> *C, Matrix<T> *A, Matrix<T> *B);

  /// \brief C=P^T*A*P
  void RAP(MatType type, Matrix<T> *C, Matrix<T> *A, Matrix<T> *P);

  /// \brief C=R^T*A*P
  void RAP(MatType type, Matrix<T> *C, Matrix<T> *R, Matrix<T> *A,
           Matrix<T> *P);
};

} /* End namespace HSF */
#endif /* end of include guard: NUMERICS_MATRIX_MATOPFACTORY_HPP */
