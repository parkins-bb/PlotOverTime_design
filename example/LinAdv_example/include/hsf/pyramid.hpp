#ifndef MESH_GEOMETRY_PYRAMID_HPP
#define MESH_GEOMETRY_PYRAMID_HPP
/*! \file pyramid.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-16
 *  \brief  class Pyramid
 */

#include "geometry.hpp"

namespace HSF {

class Pyramid : public Geometry {
 public:
  typedef BasicGeometry::Prop<BasicGeometry::PYRAMID> geom_t;

  Pyramid();
  Pyramid(llabel *ind, int *attr = NULL);
  virtual ~Pyramid() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void getVertices(std::vector<int> &v) const override;
  /// Returns geometry's type
  virtual Type getType() const override { return Geometry::PYRAMID; }
  virtual Pyramid *duplicate() const override {
    return new Pyramid(indices_, attribute_);
  }
  virtual int getNVertices() const override { return 5; }
  virtual int getNEdges() const override { return 8; }
  virtual int getNFaces() const override { return 5; }
  virtual int getNFaceVertices(int fi) const override {
    return (fi < 1) ? 4 : 3;
  }
  virtual const int *getFaceVertices(int fi) const override {
    return geom_t::faceVert[fi];
  }
  virtual const int *getEdgeVertices(int ei) const override {
    return geom_t::edges[ei];
  }
};

} /* End namespace HSF */
#endif /* end of include guard: MESH_GEOMETRY_PYRAMID_HPP */
