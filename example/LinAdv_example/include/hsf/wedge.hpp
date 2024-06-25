#ifndef MESH_GEOMETRY_WEDGE_HPP
#define MESH_GEOMETRY_WEDGE_HPP
/*! \file wedge.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-16
 *  \brief  class Wedge
 */

#include "geometry.hpp"

namespace HSF {

class Wedge : public Geometry {
 public:
  typedef BasicGeometry::Prop<BasicGeometry::PRISM> geom_t;

  Wedge();
  Wedge(llabel *ind, int *attr = NULL);
  virtual ~Wedge() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void getVertices(std::vector<int> &v) const override;
  /// Returns geometry's type
  virtual Type getType() const override { return Geometry::WEDGE; }
  virtual Wedge *duplicate() const override {
    return new Wedge(indices_, attribute_);
  }
  virtual int getNVertices() const override { return 6; }
  virtual int getNEdges() const override { return 9; }
  virtual int getNFaces() const override { return 5; }
  virtual int getNFaceVertices(int fi) const override {
    return (fi < 2) ? 3 : 4;
  }
  virtual const int *getFaceVertices(int fi) const override {
    return geom_t::faceVert[fi];
  }
  virtual const int *getEdgeVertices(int ei) const override {
    return geom_t::edges[ei];
  }
};

} /* End namespace HSF */
#endif /* end of include guard: MESH_GEOMETRY_WEDGE_HPP */
