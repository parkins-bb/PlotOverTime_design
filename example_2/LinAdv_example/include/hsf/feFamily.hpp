/*! \file feFamily.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-24
 *  \brief finite element family
 */
#ifndef SRC_FEM_FEFAMILY_HPP
#define SRC_FEM_FEFAMILY_HPP

#include "H1element.hpp"

namespace HSF {
class FEFamily {
 protected:
  int order_;

 public:
  enum {
    CONTINUOUS,   /*!< Field is continuous across element interfaces */
    TANGENTIAL,   /*!< Tangential components of vector field */
    NORMAL,       /*!< Normal component of vector field */
    DISCONTINUOUS /*!< Field is discontinuous across element interfaces */
  };
  FEFamily() : order_(0) {}
  FEFamily(int p) : order_(p) {}
  virtual ~FEFamily() {}
  virtual const FiniteElement* getFiniteElement(Geometry::Type geom) const = 0;
  virtual int getDof(Geometry::Type geom) const = 0;
  virtual const int* getInteriorDofOrdering(Geometry::Type geom,
                                            int orientation) const = 0;
  virtual int getType() const = 0;
  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  int getOrder() const { return order_; }
};

class H1FEFamily : public FEFamily {
 protected:
  int dim_, btype_;
  FiniteElement* elements_[BasicGeometry::NUMBASICGEOTYPE];
  int intrDofs_[BasicGeometry::NUMBASICGEOTYPE]; /*!< interior dof num */
  int *intrSegOrdering_[2], *intrTriOrdering_[6], *intrQuadOrdering_[8];
  /*!< interior dof ordering */

 public:
  H1FEFamily(int p, int dim = 3, int btype = BasisType::LagrangeLobatto);
  virtual ~H1FEFamily();

  virtual const FiniteElement* getFiniteElement(Geometry::Type geom) const;
  virtual int getDof(Geometry::Type geom) const { return intrDofs_[geom]; }
  virtual const int* getInteriorDofOrdering(Geometry::Type geom,
                                            int orientation) const;
  virtual int getType() const { return CONTINUOUS; }
  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  int getBasisType() const { return btype_; }
};

} /* End namespace HSF */
#endif /* end of include guard: SRC_FEM_FEFAMILY_HPP */
