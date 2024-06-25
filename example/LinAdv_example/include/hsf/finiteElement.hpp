/*! \file finiteElement.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-17
 *  \brief finite element base class
 */

#ifndef SRC_FEM_FINITEELEMENT_HPP
#define SRC_FEM_FINITEELEMENT_HPP

#include "geometry.hpp"
#include "hsfDenseMatrix.hpp"
#include "hsfVector.hpp"
#include "integrationPoints.hpp"
#include "tensor1.hpp"

namespace HSF {

class BasisType {
 public:
  enum {
    Invalid = -1,
    LagrangeLegendre = 0,       /// Open type
    LagrangeLobatto = 1,        /// Closed type
    LagrangeOpenUniform = 2,    /// Nodes: x_i = (i+1)/(n+1), i=0,...,n-1
    LagrangeClosedUniform = 3,  /// Nodes: x_i = i/(n-1),     i=0,...,n-1
    Bernstein = 4,              /// always positive Bernstein polynomials
    NumBasisTypes = 5
  };
  static int getQuadrature1D(int b_type) {
    switch (b_type) {
      case LagrangeLegendre:
        return Quadrature1D::GAUSSLEGENDRE;
      case LagrangeLobatto:
        return Quadrature1D::GAUSSLOBATTO;
      case Bernstein:
        return Quadrature1D::CLOSEDNEWTONCOTES;
      case LagrangeOpenUniform:
        return Quadrature1D::NEWTONCOTES;
      case LagrangeClosedUniform:
        return Quadrature1D::CLOSEDNEWTONCOTES;
    }
    return Quadrature1D::INVALID;
  }
};

typedef enum {
  Pk,  /// Polynomials of order k
  Qk   /// Tensor products of polynomials of order k
} FunctionSpace;

typedef enum {
  LAGRANGE = 0,   ///< Use barycentric Lagrangian interpolation, O(p) Evals
  BERNSTEIN = 1,  ///< Fast evaluation of Bernstein polynomials, not implemented
  NUMEVALTYPES = 2  ///< Keep count of the number of eval types
} PolyType;         ///...

/*! 点给定，关注多项式类型 */
class Basis1D {
 private:
  int ptype_;
  void *basisPtr_;

 public:
  Basis1D(const int p, const scalar *nodes, int ptype = PolyType::LAGRANGE);
  ~Basis1D();
  void eval(const scalar xi, std::vector<scalar> &u) const;
  /*! Evaluate the basis functions and their derivatives at point x in [0,1] */
  void eval(const scalar xi, std::vector<scalar> &u,
            std::vector<scalar> &d) const;
  /*! Evaluate the basis functions and their first two derivatives at point x in
   * [0,1] */
  void eval(const scalar xi, std::vector<scalar> &u, std::vector<scalar> &d,
            std::vector<scalar> &d2) const;
};

/// class for computing 1D special polynomials and associated basis functions
/*! 关注点的分布 */
class PolyEval {
 public:
  static std::vector<scalar *> points_;
  PolyEval() {}
  ~PolyEval();

  /*! @brief Get the coordinates of the points of the given BasisType
      @param[in] p The polynomial degree; the number of points is `p+1`.
      @return A pointer to an array containing the `p+1` coordinates of the
     points. Returns NULL if the BasisType has no associated set of points. */
  static const scalar *getPoints(const int p, int btype);

  static const int getPolyType(int btype);

  /*! @brief Evaluate the values of a hierarchical 1D basis at point x
    hierarchical = k-th basis function is degree k polynomial */
  static void calcBasis(const int p, const scalar x, scalar *u);
  /// Evaluate the values and derivatives of a hierarchical 1D basis at point x
  static void calcBasis(const int p, const scalar x, scalar *u, scalar *d);
  /// Evaluate the values, derivatives and second derivatives of a hierarchical
  /// 1D basis at point x
  static void calcBasis(const int p, const scalar x, scalar *u, scalar *d,
                        scalar *dd);
};

class FiniteElement {
 protected:
  int dim_;
  Geometry::Type gtype_;
  int dof_, order_;  ///< Order/degree of the shape functions
  int mapType_, derivType_;
  int funcSpace_;
  IntegrationPoints pts_;

 public:
  /*! @brief Enumeration for MapType: defines how reference functions are
      mapped to physical space. */
  enum MapType {
    UNKNOWN_MAP_TYPE = -1, /*!< unset */
    VALUE,                 /**< For scalar fields; preserves point values
                                    \f$ u(x) = \hat u(\hat x) \f$ */
    INTEGRAL,              /**< For scalar fields; preserves volume integrals
                                    \f$ u(x) = (1/w) \hat u(\hat x) \f$ */
    H_DIV, /**< For vector fields; preserves surface integrals of the
                    normal component \f$ u(x) = (J/w) \hat u(\hat x) \f$ */
    H_CURL /**< For vector fields; preserves line integrals of the
                    tangential component
                    \f$ u(x) = J^{-t} \hat u(\hat x) \f$ (square J),
                    \f$ u(x) = J(J^t J)^{-1} \hat u(\hat x) \f$ (general J) */
  };
  /*! @brief Enumeration for DerivType: defines which derivative method
      is implemented. */
  enum DerivType {
    NONE,  ///< No derivatives implemented
    GRAD,  ///< Implements CalcDShape methods
    DIV,   ///< Implements CalcDivShape methods
    CURL   ///< Implements CalcCurlShape methods
  };
  /** @brief Construct FiniteElement with given
      @param d    Reference space dimension
      @param g    Geometry type (of type Geometry::Type)
      @param dof   Number of degrees of freedom in the FiniteElement
      @param order    Order/degree of the FiniteElement
      @param f    FunctionSpace type of the FiniteElement
   */
  FiniteElement(int d, Geometry::Type g, int dof, int order,
                int f = FunctionSpace::Pk);
  /// Deconstruct the FiniteElement
  virtual ~FiniteElement();

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  /// Returns the reference space dimension for the finite element
  int getDim() const { return dim_; }
  /// Returns the Geometry::Type of the reference element
  Geometry::Type getGeomType() const { return gtype_; }
  /// Returns the number of degrees of freedom in the finite element
  int getDof() const { return dof_; }
  /// @brief Returns the order of the finite element
  int getOrder() const { return order_; }
  int getFunctionSpace() const { return funcSpace_; }
  /// Returns the FiniteElement::MapType of the element
  int getMapType() const { return mapType_; }
  void setMapType(int type) {
    mapType_ = type;
    derivType_ = (type == VALUE) ? GRAD : NONE;
  }
  /// @brief Returns the FiniteElement::DerivType of the element
  int getDerivType() const { return derivType_; }
  /// Get a const reference to the nodes of the element
  const IntegrationPoints &getNodes() const { return pts_; }

  /** @brief Return true if the BasisType of @a b_type is closed
      (has Quadrature1D points on the boundary). */
  static bool isClosedType(int b_type);
  /** @brief Return true if the BasisType of @a b_type is open
      (doesn't have Quadrature1D points on the boundary). */
  static bool isOpenType(int b_type);

  /*! for scalar finite elment */
  /*! @brief Evaluate the values of all shape functions of a scalar finite
      element in reference space at the given point @a ip. */
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const {}
  /*! @brief Evaluate the gradients of all shape functions of a scalar finite
      element in reference space at the given point @a ip. */
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const {}
};

} /* End namespace HSF */
#endif /* end of include guard: SRC_FEM_FINITEELEMENT_HPP */
