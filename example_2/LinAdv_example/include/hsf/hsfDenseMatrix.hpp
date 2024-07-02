/*! \file denseMatrix.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-07
 *  \brief dense matrix using column-major storage(from UNAP)
 */

#ifndef NUMERICS_MATRIX_DENSEMATRIX_HPP
#define NUMERICS_MATRIX_DENSEMATRIX_HPP

#include <vector>

#include "utilities.h"

namespace HSF {

/*!
 * 对矩阵的处理，BLAS函数的常见参数
 * N: No transpose
 * C: Complex conjugate
 * T: Transpose
 */
typedef enum { N, C, T } trans;
inline char Trans(trans t) {
  switch (t) {
    case N:
      return 'N';
    case C:
      return 'C';
    case T:
      return 'T';
  }
  return ' ';
}

/*!
 * 在哪边执行操作，BLAS函数的常见参数
 * L: Left side
 * R: Right side
 */
typedef enum { L, R } side;
inline char Side(side t) {
  switch (t) {
    case L:
      return 'L';
    case R:
      return 'R';
  }
  return ' ';
}

/*!
 * 取矩阵的哪个部分，BLAS函数的常见参数
 * U: Upper triangle
 * L: Lower triangle
 */
typedef enum { UP, LO } upLo;
inline char UpLo(upLo t) {
  switch (t) {
    case UP:
      return 'U';
    case LO:
      return 'L';
  }
  return ' ';
}

/*!
 * 矩阵对角是否全为1，BLAS函数的常见参数
 * U:  Unit diagonal
 * N:  Non-unit diagonal
 */
typedef enum { UNIT, NONU } diag;
inline char Diag(diag t) {
  switch (t) {
    case UNIT:
      return 'U';
    case NONU:
      return 'N';
  }
  return ' ';
}

/*!
 * \class DenseMatrix
 * \brief 2维矩阵，列优先存储，可直接使用BLAS/LAPACK函数
 * 自行管理空间的分配与销毁，提供深拷贝与浅拷贝两种拷贝方式
 * \tparam Cmpt 元素类型，可以是float, double, std::complex<float>等
 */
template <typename Cmpt>
class DenseMatrix {
 private:
  template <typename T>
  friend class DistributedMatrix;
  Cmpt* data_;
  int rows_;
  int cols_;
  /*! LDA(leading dimension of array A): define the distance in memory between
   * elements of two consecutive columns which have the same row index */
  int leadingDim_;
  /*! 默认为true，不分配内存时指定为false */
  bool allocated_;

 public:
  /*! 默认构造，0x0 empty matrix, leading dimension 1. */
  DenseMatrix();

  /*!
   * 构造一个m x n dense matrix并分配空间，LDA=max(1, m)
   * \param m Number of rows
   * \param n Number of columns
   */
  DenseMatrix(int m, int n);

  /*! 删除原有空间并为矩阵分配m x n 空间，LDA=max(1, m) */
  void init(int m, int n);

  /*!
   * 构造一个m x n dense matrix并拷贝数组D中的内容到data，默认为深拷贝
   * \param m Number of rows
   * \param n Number of columns
   * \param D pointer to data to be copied
   * \param ld Leading dimension of logically 2D matrix pointed to by D
   */
  DenseMatrix(int m, int n, Cmpt* D, int ld, bool allocate = true);

  /*!
   * 构造一个m x n dense matrix并拷贝矩阵D的子矩阵D(i:i+m,j:j+n)中的内容
   * 默认为深拷贝 i+m <= D.rows(), j+n <= D.cols().
   * \param m Number of rows
   * \param n Number of columns
   * \param D Matrix to be copied
   * \param i Row-offset in D, denoting top side of submatrix
   * \param j Column-offset in D, denoting left side of submatrix
   */
  DenseMatrix(int m, int n, DenseMatrix<Cmpt>& D, int i, int j,
              bool allocated = true);

  /*! 拷贝构造 */
  DenseMatrix(const DenseMatrix<Cmpt>& D);
  /*! 析构 */
  ~DenseMatrix();

  /*! 拷贝赋值 */
  DenseMatrix<Cmpt>& operator=(const DenseMatrix<Cmpt>& D);

  /*! Number of rows of the matrix */
  inline int rows() const { return rows_; }
  /*! Number of columns of the matrix */
  inline int cols() const { return cols_; }
  /*! Leading dimension used to store the matrix, typically = max(1, rows()) */
  inline int ld() const { return leadingDim_; }
  /*! Const pointer to the raw data */
  const Cmpt* data() const { return data_; }
  /*! Pointer to the raw data  */
  inline Cmpt* data() { return data_; }

  /*! Pointer to the element after the last one in this matrix */
  inline Cmpt* end() { return data_ + leadingDim_ * cols_; }

  /*! Pointer to the element after the last one in this matrix */
  inline const Cmpt* end() const {
    if (rows_ == 0 || cols_ == 0) return data_;
    return data_ + leadingDim_ * cols_;
  }

  /*!
   * Const reference to element (i,j) in the matrix
   * \param i Row index, i < rows()
   * \param j Column index, j < cols()
   */
  inline const Cmpt& operator()(int i, int j) const {
    // #ifdef DEBUG
    //     ASSERT(i <= rows() && j <= cols())
    // #endif
    return data_[i + leadingDim_ * j];
  }
  /*!
   * Reference to element (i,j) in the matrix
   * \param i Row index, i < rows()
   * \param j Column index, j < cols()
   */
  inline Cmpt& operator()(int i, int j) {
    // #ifdef DEBUG
    //     ASSERT(i <= rows() && j <= cols())
    // #endif
    return data_[i + leadingDim_ * j];
  }

  /*!
   * Const pointer to element (i,j) in the matrix
   * \param i Row index, i < rows()
   * \param j Column index, j < cols()
   */
  inline const Cmpt* ptr(int i, int j) const {
    // #ifdef DEBUG
    //     ASSERT(i <= rows() && j <= cols())
    // #endif
    return data_ + i + leadingDim_ * j;
  }
  /*!
   * Pointer to element (i,j) in the matrix
   * \param i Row index, i < rows()
   * \param j Column index, j < cols()
   */
  inline Cmpt* ptr(int i, int j) {
    // #ifdef DEBUG
    //     ASSERT(i <= rows() && j <= cols())
    // #endif
    return data_ + i + leadingDim_ * j;
  }

  /*! Print the matrix to POUT(utilities), useful for debugging */
  void print() const { print("A"); }
  /*!
   * Print the matrix to POUT(utilities), useful for debugging
   * \param name Name to use when printing.
   * \param all If true, print all values, if false, only print all
   * values when not too big. Defaults to false.
   * \param width Specifies how many digits to use for
   * printing floating point values, defaults to 8.
   */
  void print(std::string name, bool all = false, int width = 8) const;

  /*! Fill matrix with a constant value v */
  void fill(Cmpt v);

  /*! Set all matrix elements to zero */
  void zero();

  /*! Set the matrix to the identity matrix */
  void eye();

  /*! Clear the matrix. Resets the number of rows and columns to 0 */
  void clear();

  /*!
   * Resize the matrix. The relevant parts of the original matrix
   * will be copied to the new matrix. new parts are undefined.
   * \param m Number of rows after resizing.
   * \param m Number of columns after resizing.
   */
  void resize(int m, int n);

  /*!
   * Add matrix B to this matrix. Return a reference
   * \param B matrix to add to this matrix.
   */
  DenseMatrix<Cmpt>& add(const DenseMatrix<Cmpt>& B);
  /*!
   * Subtract matrix B from this matrix. Return a reference
   * \param B matrix to subtract from this matrix
   */
  DenseMatrix<Cmpt>& sub(const DenseMatrix<Cmpt>& B);
  /*!
   * Scale this matrix by a constant factor
   * \param alpha scaling factor
   */
  DenseMatrix<Cmpt>& scale(Cmpt alpha);
  /*!
   * this += alpha * B.
   * \param alpha scalar factor
   * \param B matrix to add, should be the same size of this matrix
   */
  DenseMatrix<Cmpt>& scaledAdd(Cmpt alpha, const DenseMatrix<Cmpt>& B);
  /*!
   * this = alpha * this + B.
   * \param alpha scalar factor
   * \param B matrix to add, should be the same size of this matrix
   */
  DenseMatrix<Cmpt>& scaleAndAdd(Cmpt alpha, const DenseMatrix<Cmpt>& B);
  /*!
   * Scale the rows of this matrix with the scalar values from the
   * vector D. Row i in this matrix is scaled with D[i].
   * \param D scaling vector, D.size() == rows()
   */
  DenseMatrix<Cmpt>& scaleRows(const std::vector<Cmpt>& D);
  /*!
   * Scale the rows of this matrix with the scalar values from the
   * vector D. Row i in this matrix is scaled with D[i].
   * \param D scaling vector
   */
  DenseMatrix<Cmpt>& scaleRows(const Cmpt* D);
  /*!
   * Scale the rows of this matrix with the inverses of the scalar
   * values in vector D, this->operator()(i,j) /= D[i].
   * \param D scalar factors, D.size() == rows()
   */
  DenseMatrix<Cmpt>& divRows(const std::vector<Cmpt>& D);

  /*!
   * Return the (approximate) amount of memory taken by this matrix,
   * in bytes. Metadata is not counted.
   */
  int memory() const { return sizeof(Cmpt) * rows() * cols(); }

  /*!
   * Return the number of nonzeros in this matrix
   * simply rows()*cols()
   */
  int nonzeros() const { return rows() * cols(); }

  /*!
   * Shift the diagonal with a value sigma
   * add a scaled identity matrix
   * \param sigma scalar value to add to diagonal
   */
  void shift(Cmpt sigma);

  void transpose(const DenseMatrix<Cmpt>& A);

  /*-------------------- member LAPACK ROUTINES ------------------------*/

  /*! Return default norm of this matrix(the Frobenius norm) */
  scalar norm() const;
  /*! Return the Frobenius norm of this matrix */
  scalar normF() const;
  /*! Return the 1-norm of this matrix */
  scalar norm1() const;
  /*! Return the infinity norm of this matrix */
  scalar normI() const;

  /*!
   * LAPACK xLASWP performs a series of row interchanges on the matrix
   * One row interchange is initiated for each of rows in the vector P
   * \param P vector with row interchanges, size rows()
   * \param fwd when false, the pivots are applied in reverse order
   */
  void laswp(const std::vector<int>& P, bool fwd);
  /*!
   * LAPACK xLASWP performs a series of row interchanges on the matrix
   * \param P array with row interchanges, size rows()
   * \param fwd when false, the pivots are applied in reverse order
   */
  void laswp(const int* P, bool fwd);

  /*!
   * LAPACK xLAPMR rearranges the rows by the permutation P(1),P(2),...,P(M)
   * If fwd == true, forward permutation:
   *    A(K(I),*) is moved to A(I,*) for I = 1,2,...,M.
   * If fwd == false, backward permutation:
   *    A(I,*) is moved to A(K(I),*) for I = 1,2,...,M.
   * \param P permutation vector, should be of size rows()
   * \param fwd apply permutation, or inverse permutation
   */
  // void lapmr(const std::vector<int>& P, bool fwd);
  /*!
   * LAPACK xLAPMT rearranges the columns by the permutation P(1),P(2),...,P(M)
   * If fwd == true, forward permutation:
   *     A(*,K(J)) is moved A(*,J) for J = 1,2,...,N.
   * If fwd == false, backward permutation:
   *     A(*,J) is moved to A(*,K(J)) for J = 1,2,...,N.
   * \param P permutation vector, should be of size cols()
   * \param fwd apply permutation, or inverse permutation
   */
  void lapmt(const std::vector<int>& P, bool fwd);

  /*!
   * LAPACK DGETRF compute an LU factorization using partial
   * pivoting with row interchanges
   * A = P * L * U, where P is a permutation matrix, L is lower
   * triangular with unit diagonal elements, and U is upper triangular
   * L and U factors are stored in place
   * \param piv pivot vector, will be resized if necessary
   * \return if nonzero, the pivot in this column was exactly zero
   */
  int LU(std::vector<int>& piv);
  /*!
   * LAPACK DGETRF compute an LU factorization using partial
   * pivoting with row interchanges
   * A = P * L * U, where P is a permutation matrix, L is lower
   * triangular with unit diagonal elements, and U is upper triangular
   * L and U factors are stored in place
   * \return the pivot vector
   */
  std::vector<int> LU();

  /*!
   * LAPACK DPOTRF compute a Cholesky factorization of this matrix in-place
   * Only the lower triangle is written
   * Only the lower triangle is referenced/stored
   * \return info from xpotrf
   * \see LU, LDLt
   */
  int Cholesky();

  /*!
   * LAPACK SYTRF compute an LDLt factorization of this matrix in-place
   * Only the lower triangle is referenced/stored
   * \return info from xsytrf
   * \see LU, Cholesky, solve_LDLt
   */
  std::vector<int> LDLt();

  /*!
   * Solve a linear system AX=B where A is an LU-factored matrix
   * There can be multiple right hand side vectors
   * The solution is returned by value
   * \param b input, right hand side vector/matrix
   * \param piv pivot vector returned by LU factorization
   * \return the solution x
   */
  DenseMatrix<Cmpt> solve(const DenseMatrix<Cmpt>& b,
                          const std::vector<int>& piv) const;
  void solve(const Cmpt* b, Cmpt* x, const std::vector<int>& piv) const;
  void solve(const DenseMatrix<Cmpt>& b, DenseMatrix<Cmpt>& x,
             const std::vector<int>& piv) const;

  /*!
   * Solve a linear system Ax=b with this matrix
   * There can be multiple right hand side vectors
   * \param b input, right hand side vector/matrix. On output this
   * will be the solution
   * \param piv pivot vector returned by LU factorization
   */
  void solveLUReplace(DenseMatrix<Cmpt>& b, const std::vector<int>& piv) const;

  /*!
   * Solve a linear system Ax=b with this matrix
   * There can be multiple right hand side vectors
   * \param b input, right hand side vector/matrix. On output this
   * will be the solution
   * \param piv pivot vector returned by LU factorization
   */
  void solveLUReplace(DenseMatrix<Cmpt>& b, const int* piv) const;

  /*!
   * Solve a linear system Ax=b with this matrix, LDLt factorization
   * There can be multiple right hand side vectors
   * \param b input, right hand side vector/matrix. On output this
   * will be the solution.
   * \param piv pivot vector returned by LDLt factorization
   */
  void solveLDLtReplace(DenseMatrix<Cmpt>& b,
                        const std::vector<int>& piv) const;

  /*!
   * Return a vector with the singular values of this matrix
   * Used only for debugging
   */
  // std::vector<Cmpt> singularValues() const;
};

/*-------------------- non-member LAPACK ROUTINES ------------------------*/
template <typename Cmpt>
void mult(const DenseMatrix<Cmpt>& a, const DenseMatrix<Cmpt>& b,
          DenseMatrix<Cmpt>& c);

/*!
 * GEMM, defined for DenseMatrix objects
 *
 * LAPACK DGEMM  performs one of the matrix-matrix operations
 * C := alpha*op( A )*op( B ) + beta*C,
 * where  op( X ) is one of
 * op( X ) = X   or   op( X ) = X*!T,
 * alpha and beta are scalars, and A, B and C are matrices, with op( A )
 * an m by k matrix,  op( B )  a  k by n matrix and  C an m by n matrix.
 */

/*! 最常用的形式，输入与输出都为DenseMatrix
 * C := alpha*op( A )*op( B ) + beta*C,
 * \param ta op(A)
 * \param tb op(B)
 */
template <typename Cmpt>
void gemm(trans ta, trans tb, Cmpt alpha, const DenseMatrix<Cmpt>& a,
          const DenseMatrix<Cmpt>& b, Cmpt beta, DenseMatrix<Cmpt>& c);
/*! 此处的指针b相当于常用形式中的b.data(), ldb相当于b.ld() */
template <typename Cmpt>
void gemm(trans ta, trans tb, Cmpt alpha, const DenseMatrix<Cmpt>& a,
          const Cmpt* b, int ldb, Cmpt beta, DenseMatrix<Cmpt>& c);
/*! 此处的指针c相当于常用形式中的c.data(), ldc相当于c.ld() */
template <typename Cmpt>
void gemm(trans ta, trans tb, Cmpt alpha, const DenseMatrix<Cmpt>& a,
          const DenseMatrix<Cmpt>& b, Cmpt beta, Cmpt* c, int ldc);

/*!
 * LAPACK TRMM performs one of the matrix-matrix operations
 * B := alpha*op(A)*B,   or   B := alpha*B*op(A),
 * where alpha is a scalar, B is an m by n matrix, A is a unit, or
 * non-unit, upper or lower triangular matrix and op( A ) is one of
 * op( A ) = A   or   op( A ) = A*!T.
 * \param s  op(A)*B or B*op(A)
 * \param ul upper or lower triangular part of A
 * \param ta A or A transpose
 * \param d  unit or non-unit diagonal
 */
template <typename Cmpt>
void trmm(side s, upLo ul, trans ta, diag d, Cmpt alpha,
          const DenseMatrix<Cmpt>& a, DenseMatrix<Cmpt>& b);

/*!
 * LAPACK DTRSM solves one of the matrix equations
 * op( A )*X = alpha*B,   or   X*op( A ) = alpha*B,
 * where alpha is a scalar, X and B are m by n matrices, A is a
 * unit, or non-unit, upper or lower triangular matrix and op( A )
 * is one of
 * op( A ) = A   or   op( A ) = A*!T.
 * The matrix X is overwritten on B.
 */
template <typename Cmpt>
void trsm(side s, upLo ul, trans ta, diag d, Cmpt alpha,
          const DenseMatrix<Cmpt>& a, DenseMatrix<Cmpt>& b);

/*!
 * LAPACK DTRSV solves one of the systems of equations
 * A*x = b,   or   A*!T*x = b,
 * where b and x are n element vectors and A is an n by n unit, or
 * non-unit, upper or lower triangular matrix.
 */
template <typename Cmpt>
void trsv(upLo ul, trans ta, diag d, const DenseMatrix<Cmpt>& a,
          DenseMatrix<Cmpt>& b);

/*!
 * LAPACK DGEMV performs one of the matrix-vector operations
 * y := alpha*A*x + beta*y,   or   y := alpha*A*!T*x + beta*y
 * where alpha and beta are scalars, x and y are vectors
 * A is an m by n matrix.
 */
template <typename Cmpt>
void gemv(trans ta, Cmpt alpha, const DenseMatrix<Cmpt>& a,
          const DenseMatrix<Cmpt>& x, Cmpt beta, DenseMatrix<Cmpt>& y);

/*!
 * \param x Buffer holding input/output vector x. The length len of vector x is
 * m, if A(mxn) is not transposed, and n if A is transposed. The buffer must be
 * of size at least (1 + (len - 1)*abs(incx)) where len is this length.
 * \parm incx the stride of vector x
 */
template <typename Cmpt>
void gemv(trans ta, Cmpt alpha, const DenseMatrix<Cmpt>& a, const Cmpt* x,
          int incx, Cmpt beta, DenseMatrix<Cmpt>& y);
template <typename Cmpt>
void gemv(trans ta, Cmpt alpha, const DenseMatrix<Cmpt>& a,
          const DenseMatrix<Cmpt>& x, Cmpt beta, Cmpt* y, int incy);
template <typename Cmpt>
void gemv(trans ta, Cmpt alpha, const DenseMatrix<Cmpt>& a, const Cmpt* x,
          int incx, Cmpt beta, Cmpt* y, int incy);

} /* End namespace HSF */

#endif /* end of include guard: NUMERICS_MATRIX_DENSEMATRIX_HPP */
