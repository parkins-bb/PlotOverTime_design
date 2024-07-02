/*! \file matrix_unapcsr.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2013-10-07
 *  \brief unap csr matrix wrapper
 */

#ifndef NUMERICS_MATRIX_MATRIX_UNAPCSR_HPP
#define NUMERICS_MATRIX_MATRIX_UNAPCSR_HPP

#include "csrMatrix.hpp"
#include "hsfMatrix.hpp"
namespace HSF {

template <typename T>
class UnapCsrMatrix : public Matrix<T> {
 protected:
  bool own_;
  /* actual object */
  UNAP::CSRMatrix<T> *x_;
  friend MatOp<MatType::UNAP_CSR>;

 public:
  UnapCsrMatrix() : own_(false), x_(NULL) {}
  ~UnapCsrMatrix() {
    if (own_) delete x_;
  }

  explicit UnapCsrMatrix(UNAP::CSRMatrix<T> *y)
      : Matrix<T>(), own_(false), x_(y) {}

  /*! Typecasting to unap::csrmatrix */
  operator UNAP::CSRMatrix<T> *() const { return x_; }
  virtual MatType getType() const { return MatType::UNAP_CSR; }
};

/*! * * * * * * * * * * * * * * * MatOp * * * * * * * * * * * * * */
/*! UNAP_CSR  */
template <>
class MatOp<MatType::UNAP_CSR> {
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
#endif /* end of include guard: NUMERICS_MATRIX_MATRIX_UNAPCSR_HPP */
