/*! \file matrix.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-03-04
 *  \modified 2023-10-07
 *  \brief Matrix class
 */
#ifndef NUMERICS_MATRIX_MATRIX_HPP
#define NUMERICS_MATRIX_MATRIX_HPP

#include "matOpFactory.hpp"
#include "hsfVector.hpp"
namespace HSF {

template <typename T>
class Matrix {
 protected:
 public:
  /*! none */
  Matrix() {}
  virtual ~Matrix() {}
  virtual MatType getType() const { return MatType::DEFAULT; }
  virtual void mult(const Vector<T> &x, Vector<T> &y) const {
    COUT << "Not implemented!" << ENDL;
  }
  virtual void multTranspose(const Vector<T> &x, Vector<T> &y) const {
    COUT << "Not implemented!" << ENDL;
  }
};
/*!  * * * * * * * * * * * * * "virtual" functions * * * * * * * * * * * * */
/// \brief Y+=a*X
template <typename T>
void axpy(Matrix<T> *Y, T a, Matrix<T> *X) {
  ASSERT(Y->getType() == X->getType())
  MatOpFactory<T>::axpy(Y->getType(), Y, a, X);
}

/// \brief Y=a*Y+X
template <typename T>
void aypx(Matrix<T> *Y, T a, Matrix<T> *X) {
  ASSERT(Y->getType() == X->getType())
  MatOpFactory<T>::aypx(Y->getType(), Y, a, X);
}

/// \brief y=A*x
template <typename T>
void mult(Matrix<T> *A, Vector<T> *x, Vector<T> *y) {
  MatOpFactory<T>::mult(A->getType(), A, x, y);
}

/// \brief y=alpha*A*x+beta*y
template <typename T>
void multAdd(T alpha, Matrix<T> *A, Vector<T> *x, T beta, Vector<T> *y) {
  MatOpFactory<T>::multAdd(A->getType(), alpha, A, x, beta, y);
}

/// \brief y=A^T*x
template <typename T>
void multTranspose(Matrix<T> *A, Vector<T> *x, Vector<T> *y) {
  MatOpFactory<T>::multTranspose(A->getType(), A, x, y);
}

/// \brief y=alpha*A^T*x+beta*y
template <typename T>
void multTransposeAdd(T alpha, Matrix<T> *A, Vector<T> *x, T beta,
                      Vector<T> *y) {
  MatOpFactory<T>::multTransposeAdd(A->getType(), alpha, A, x, beta, y);
}

/// \brief B=A^T
template <typename T>
void transpose(Matrix<T> *A, Matrix<T> *B) {
  MatOpFactory<T>::transpose(A->getType(), A, B);
}

/// \brief B=A
template <typename T>
void copy(Matrix<T> *A, Matrix<T> *B) {
  MatOpFactory<T>::copy(A->getType(), A, B);
}

/// \brief A=a*A
template <typename T>
void scale(Matrix<T> *A, T a) {
  MatOpFactory<T>::scale(A->getType(), A, a);
}

/// \brief A=diag(l)*A*diag(r)
template <typename T>
void diagonalScale(Matrix<T> *A, Vector<T> *l, Vector<T> *r) {
  MatOpFactory<T>::diagonalScale(A->getType(), A, l, r);
}

/// \brief diag(A)
template <typename T>
void diag(Matrix<T> *A, Vector<T> *d) {
  MatOpFactory<T>::diag(A->getType(), A, d);
}

/// \brief A+=a*I
template <typename T>
void shift(Matrix<T> *A, T a) {
  MatOpFactory<T>::shift(A->getType(), A, a);
}

/// \brief C=A*B
template <typename T>
void matMult(Matrix<T> *C, Matrix<T> *A, Matrix<T> *B) {
  ASSERT(A->getType() == B->getType())
  MatOpFactory<T>::matMult(A->getType(), C, A, B);
}

/// \brief C=P^T*A*P
template <typename T>
void RAP(Matrix<T> *C, Matrix<T> *A, Matrix<T> *P) {
  ASSERT(A->getType() == P->getType())
  MatOpFactory<T>::RAP(A->getType(), C, A, P);
}

/// \brief C=R^T*A*P
template <typename T>
void RAP(Matrix<T> *C, Matrix<T> *R, Matrix<T> *A, Matrix<T> *P) {
  ASSERT(A->getType() == P->getType() && R->getType() == P->getType())
  MatOpFactory<T>::RAP(A->getType(), C, R, A, P);
}

/*! * * * * * * * * * * * * * * * MatOp * * * * * * * * * * * * * */
/*! DEFAULT  */
template <MatType type>
class MatOp {
 public:
  template <typename T>
  static void axpy(Matrix<T> *Y, T a, Matrix<T> *X) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void aypx(Matrix<T> *Y, T a, Matrix<T> *X) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void mult(Matrix<T> *A, Vector<T> *x, Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void multAdd(T alpha, Matrix<T> *A, Vector<T> *x, T beta,
                      Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void multTranspose(Matrix<T> *A, Vector<T> *x, Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void multTransposeAdd(T alpha, Matrix<T> *A, Vector<T> *x, T beta,
                               Vector<T> *y) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void transpose(Matrix<T> *A, Matrix<T> *B) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void copy(Matrix<T> *A, Matrix<T> *B) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void scale(Matrix<T> *A, T a) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void diagonalScale(Matrix<T> *A, Vector<T> *l, Vector<T> *r) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void diag(Matrix<T> *A, Vector<T> *diag) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void shift(Matrix<T> *A, T a) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void matMult(Matrix<T> *C, Matrix<T> *A, Matrix<T> *B) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void RAP(Matrix<T> *C, Matrix<T> *A, Matrix<T> *P) {
    COUT << "Not implemented!" << ENDL;
  }

  template <typename T>
  static void RAP(Matrix<T> *C, Matrix<T> *R, Matrix<T> *A, Matrix<T> *P) {
    COUT << "Not implemented!" << ENDL;
  }
};

} /* End namespace HSF */
#endif /* end of include guard: NUMERICS_MATRIX_MATRIX_HPP */
