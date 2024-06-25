/*! \file coordTransform.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-23
 *  \brief element coordinate transformation
 */

#ifndef SRC_FEM_COORDTRANSFORM_HPP
#define SRC_FEM_COORDTRANSFORM_HPP

#include "finiteElement.hpp"
#include "hsfDenseMatrix.hpp"
#include "integrationPoints.hpp"
#include "utilityExceptions.h"

namespace HSF {

class CoordinateTransformation {
 protected:
  scalar w_;                   /*!< jacobian determinant  */
  int state_;                  /*!< binary state mask */
  DenseMatrix<scalar> points_; /*!< dim x dof */
  DenseMatrix<scalar> J_, adjJ_, invJ_, adjJT_;

  const FiniteElement *fe_;          /*!< element to do transformation  */
  const Tensor1<scalar> *evalPoint_; /*!< point to eval Jacobian matrix */

  Vector<scalar> tmp_shape_;
  DenseMatrix<scalar> tmp_dshape_;

  /*! binary state mask enum */
  enum STATE {
    JACOBIAN = 1,
    WEIGHT = 2,
    ADJUGATE = 4,
    INVERSE = 8,
    ADJUGATE_T = 16
  };

  /*! evaluate the Jacobian of the transformation at the evalPoint and
      store it in J_. */
  const DenseMatrix<scalar> &evalJacobian();
  scalar evalWeight();
  const DenseMatrix<scalar> &evalInverseJ();
  const DenseMatrix<scalar> &evalAdjugateJ();
  const DenseMatrix<scalar> &evalTransAdjugateJ();

 public:
  CoordinateTransformation();
  CoordinateTransformation(const FiniteElement *fe);
  virtual ~CoordinateTransformation();

  /*! Force the reevaluation of the Jacobian */
  void reset() { state_ = 0; }

  void setFE(const FiniteElement *fe) {
    ASSERT(fe != NULL);
    state_ = (fe_ != fe) ? 0 : state_;
    fe_ = fe;
  }
  const FiniteElement *getFE() const { return fe_; }

  void setPoints(const DenseMatrix<scalar> &pmat) {
    points_ = pmat;
    state_ = 0;
  }
  /*! not implemented yet */
  void setPoints(const std::vector<Tensor1<scalar> > &parray);
  const DenseMatrix<scalar> &getPoints() const { return points_; }
  DenseMatrix<scalar> &getPoints() { return points_; }

  /*! TODO:
   * 对于固定阶数的每种单元，能否存储所有这些积分点的Jacobi矩阵值，避免多次计算？
   */
  /*! set the integration point @a ip that weights and Jacobians will be
   *  evaluated at. */
  void setPoint(const Tensor1<scalar> *ip) {
    evalPoint_ = ip;
    state_ = 0;
  }
  /*! get a const reference to the currently set integration point.  */
  const Tensor1<scalar> &getPoint() { return *evalPoint_; }
  /*! Return the Geometry::Type of the reference element. */
  Geometry::Type getGeometryType() const { return fe_->getGeomType(); }
  /*! Return the topological dimension of the reference element.*/
  const int getDimension() const { return fe_->getDim(); }
  /*! Get the dimension of the target (physical) space. */
  const int getSpaceDim() const { return points_.rows(); }

  /*! Return the order of the current element we are using for the
   * transformation. */
  inline int order() const;
  /*! Return the order of the elements of the Jacobian of the transformation. */
  inline int orderJ() const;
  /*! Return the order of the determinant of the Jacobian (weight) of the
   *  transformation. */
  inline int orderW() const;
  /*! Return the order of \f$ adj(J)^T \nabla fi \f$ */
  inline int orderGrad(const FiniteElement *fe) const;

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  /*! Transform integration point from reference coordinates to
      physical coordinates and store them in the vector. */
  inline void transform(const Tensor1<scalar> &from, Tensor1<scalar> &to);
  /*! nDim x nPoints  */
  inline void transform(const IntegrationPoints &from, DenseMatrix<scalar> &to);
  inline void transform(const DenseMatrix<scalar> &from,
                        DenseMatrix<scalar> &to);

  /*! Return the Jacobian matrix of the transformation at the currently
      point.  The dimensions of the Jacobian matrix are
      physical-space-dim by reference-space-dim. The first column contains the x
      derivatives of the transformation, the second -- the y derivatives, etc.
   */
  inline const DenseMatrix<scalar> &jacobian();

  /*! sqrt(J^T*J) */
  inline scalar weight();

  /*!  Return the adjugate of the Jacobian matrix of the transformation
       at the currently point  */
  inline const DenseMatrix<scalar> &adjugateJacobian();

  /*!  Return the transpose of the adjugate of the Jacobian matrix of
       the transformation at the currently point. */
  inline const DenseMatrix<scalar> &transposeAdjugateJacobian();

  /*!  Return the inverse of the Jacobian matrix of the transformation
       at the currently point. */
  inline const DenseMatrix<scalar> &inverseJacobian();
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
/*! calculate the determine of small dense matrix */
scalar calcDet(const DenseMatrix<scalar> &mat);
/*! sqrt(J^T*J) */
scalar calcWeight(const DenseMatrix<scalar> &mat);
/*! A* */
void calcAdjugate(const DenseMatrix<scalar> &A, DenseMatrix<scalar> &adjA);
/*! A*^T */
void calcAdjugateTranspose(const DenseMatrix<scalar> &A,
                           DenseMatrix<scalar> &adjTA);
/*! A^(-1), left inverse when not square */
void calcInverse(const DenseMatrix<scalar> &A, DenseMatrix<scalar> &invA);

} /* End namespace HSF */
#endif /* end of include guard: SRC_FEM_COORDTRANSFORM_HPP */
