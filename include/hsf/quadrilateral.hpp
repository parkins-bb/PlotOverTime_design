#ifndef MESH_GEOMETRY_QUADRILATERAL_HPP
#define MESH_GEOMETRY_QUADRILATERAL_HPP
/*! \file quadrilateral.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-16
 *  \brief  class Quadrilateral
 */

#include "geometry.hpp"

namespace HSF {

class Quadrilateral : public Geometry {
 public:
  typedef BasicGeometry::Prop<BasicGeometry::SQUARE> geom_t;

  Quadrilateral();
  Quadrilateral(llabel *ind, int *attr = NULL);
  virtual ~Quadrilateral() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void getVertices(std::vector<int> &v) const override;
  /// Returns geometry's type
  virtual Type getType() const override { return Geometry::QUADRILATERAL; }
  virtual Quadrilateral *duplicate() const override {
    return new Quadrilateral(indices_, attribute_);
  }
  virtual int getNVertices() const override { return 4; }
  virtual int getNEdges() const override { return 4; }
  virtual int getNFaces() const override { return 0; }
  virtual int getNFaceVertices(int fi) const override { return 0; }
  virtual const int *getFaceVertices(int fi) const override { return NULL; }
  virtual const int *getEdgeVertices(int ei) const override {
    return geom_t::edges[ei];
  }
};

} /* End namespace HSF */
#endif /* end of include guard: MESH_GEOMETRY_QUADRILATERAL_HPP */
