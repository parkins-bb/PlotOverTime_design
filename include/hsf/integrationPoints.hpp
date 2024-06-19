#ifndef MESH_GEOMETRY_INTEGRATIONPOINTS_HPP
#define MESH_GEOMETRY_INTEGRATIONPOINTS_HPP
/*! \file integrationPoints.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-03-24
 *  \brief integration rules for points
 */

#include <vector>

#include "utilityType.h" /*!< scalar */

namespace HSF {

struct WeightedPoint {
  scalar x_, y_, z_;
  scalar weight_;
  int index_;

  /*! 初始化为0.，index为i */
  void init(const int i);

  /*! set coordinate only */
  void set(const scalar *p, const int dim);
  /*! get coordinate only */
  void get(scalar *p, const int dim);

  /*! set coord and weight of 3D */
  void set3w(const scalar x1, const scalar x2, const scalar x3, const scalar w);
  /*! set coord of 3D */
  void set3(const scalar x1, const scalar x2, const scalar x3);
  /*! set coord and weight of 2D */
  void set2w(const scalar x1, const scalar x2, const scalar w);
  /*! set coord of 2D */
  void set2(const scalar x1, const scalar x2);
  /*! set coord and weight of 1D */
  void set1w(const scalar x1, const scalar w);
  /*! set coord of 1D */
  void set1(const scalar x1);

  /*! set coord and weight of 3D, pointer form */
  void set3w(const scalar *p);
  /*! set coord of 3D, pointer form*/
  void set3(const scalar *p);
  /*! set coord and weight of 2D, pointer form */
  void set2w(const scalar *p);
  /*! set coord of 2D, pointer form */
  void set2(const scalar *p);
  /*! set coord and weight of 1D, pointer form */
  void set1w(const scalar *p);
  /*! set coord of 1D */
  void set1(const scalar *p);
};

class IntegrationPoints : public std::vector<WeightedPoint> {
 private:
  friend class IntegrationRules;
  int order_;
  mutable std::vector<scalar> weights_;

 public:
  /*! default constrctor */
  IntegrationPoints();
  /*! default deconstrctor */
  ~IntegrationPoints();
  /*! nPoints  constrctor */
  explicit IntegrationPoints(int nPts);

  /*! combination of two 1D integration rules */
  IntegrationPoints(IntegrationPoints &irx, IntegrationPoints &iry);
  /*! combination of three 1D integration rules */
  IntegrationPoints(IntegrationPoints &irx, IntegrationPoints &iry,
                    IntegrationPoints &irz);

  /*! set the indices of each quadrature point */
  void setIndices();
  /*! the order of the integration rule */
  int getOrder() const;
  /*! keeping order information */
  void setOrder(const int order);
  /*! the number of the points in the integration rule */
  int getNPts() const;
  /*! reference to the i-th integration point */
  inline WeightedPoint &point(int i) { return (*this)[i]; }
  /*! const reference to the i-th integration point */
  inline const WeightedPoint &point(int i) const { return (*this)[i]; }
  /*! get a contiguous quadrature weights array */
  const std::vector<scalar> &weights() const;
};

/*!
 * \class BarycentricLagrangeBasis
 * \brief helper class to calculate the coefs of lagrange polynomials
 * BERRUT J P, TREFETHEN L N. Barycentric Lagrange Interpolation[J]. SIAM
 Review, 2004, 46(3): 501-517.
 */
class BarycentricLagrangeBasis {
  /*! 使用时会有一定舍入误差 */
 private:
  mutable std::vector<scalar> x_, w_;

 public:
  /*! calculate wj */
  BarycentricLagrangeBasis(const int p, const scalar *nodes);
  /*! Evaluate the basis functions at point x in [0,1]
   * \param[out] u vector: {f_0(xi),f_1(xi),...,f_p(xi)},
   * where f_k(x) is the k th lagrange polynomial
   */
  void eval(const scalar xi, std::vector<scalar> &u) const;
  /*! Evaluate the basis functions and their derivatives at point x in [0,1] */
  void eval(const scalar xi, std::vector<scalar> &u,
            std::vector<scalar> &d) const;
  /*! Evaluate the basis functions and their first two derivatives at point x in
   * [0,1] */
  void eval(const scalar xi, std::vector<scalar> &u, std::vector<scalar> &d,
            std::vector<scalar> &d2) const;
};

/*! defines 1-D numerical quadrature rules on [0,1] */
class Quadrature1D {
 public:
  /*! formulas may be "closed" if the interval [x_1,x_n] is
   *  included in the fit, "open" if the points [x_2,x_(n-1)] are used, or a
   *  variation of these two. */
  enum {
    INVALID = -1,
    GAUSSLEGENDRE = 0,
    GAUSSLOBATTO = 1, /*!< Gauss-Lobatto-Legendre */
    NEWTONCOTES = 2,
    CLOSEDNEWTONCOTES = 3,
    NUMQUADRATURETYPE = 4 /*!< keep track num, easy-coding */
  };

  static void gaussLegendre(const int np, IntegrationPoints &ir);
  // static void closedGaussLegendre(const int np, IntegrationPoints &ir);
  static void gaussLobatto(const int np, IntegrationPoints &ir);
  static void newtonCotes(const int np, IntegrationPoints &ir);
  static void closedNewtonCotes(const int np, IntegrationPoints &ir);

  static void givePoints(const int np, scalar *pts, const int type);
  static int checkClosed(int type);
  static int checkOpen(int type);
};

class IntegrationRules {
 private:
  /*! Taken from the Quadrature1D class anonymous enum Determines the type of
   * numerical quadrature used for segment, square, and cube geometries */
  const int quadratureType_;

  std::vector<IntegrationPoints *> pointIntRules_;
  std::vector<IntegrationPoints *> segmentIntRules_;
  std::vector<IntegrationPoints *> triangleIntRules_;
  std::vector<IntegrationPoints *> squareIntRules_;
  std::vector<IntegrationPoints *> tetrahedronIntRules_;
  std::vector<IntegrationPoints *> pyramidIntRules_;
  std::vector<IntegrationPoints *> prismIntRules_;
  std::vector<IntegrationPoints *> cubeIntRules_;
  std::vector<bool> ownRules_[8];

  IntegrationPoints *generateRule(int geomType, int order);
  IntegrationPoints *pointIntegrationRule(int order);
  IntegrationPoints *segmentIntegrationRule(int order);
  IntegrationPoints *triangleIntegrationRule(int order);
  IntegrationPoints *squareIntegrationRule(int order);
  IntegrationPoints *tetrahedronIntegrationRule(int order);
  IntegrationPoints *pyramidIntegrationRule(int order);
  IntegrationPoints *prismIntegrationRule(int order);
  IntegrationPoints *cubeIntegrationRule(int order);

 public:
  explicit IntegrationRules(int type = Quadrature1D::GAUSSLEGENDRE);
  ~IntegrationRules();

  const IntegrationPoints &get(int GeomType, int order);
  void set(int GeomType, int order, IntegrationPoints &IntRule);
};

/*! A global object with all integration rules */
extern IntegrationRules intRules;

} /* End namespace HSF */
#endif /* end of include guard: MESH_GEOMETRY_INTEGRATIONPOINTS_HPP */
