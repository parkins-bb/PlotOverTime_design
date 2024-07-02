#ifndef MESH_GEOMETRY_TETRAHEDRON_HPP
#define MESH_GEOMETRY_TETRAHEDRON_HPP
/*! \file tetrahedron.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-16
 *  \brief  class Tetrahedron
 */

#include "geometry.hpp"

namespace HSF {

class Tetrahedron : public Geometry {
 public:
  typedef BasicGeometry::Prop<BasicGeometry::TETRAHEDRON> geom_t;

  Tetrahedron();
  Tetrahedron(llabel *ind, int *attr = NULL);
  virtual ~Tetrahedron() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void getVertices(std::vector<int> &v) const override;
  /// Returns geometry's type
  virtual Type getType() const override { return Geometry::TETRAHEDRON; }
  virtual Tetrahedron *duplicate() const override {
    return new Tetrahedron(indices_, attribute_);
  }
  virtual int getNVertices() const override { return 4; }
  virtual int getNEdges() const override { return 6; }
  virtual int getNFaces() const override { return 4; }
  virtual int getNFaceVertices(int fi) const override { return 3; }
  virtual const int *getFaceVertices(int fi) const override {
    return geom_t::faceVert[fi];
  }
  virtual const int *getEdgeVertices(int ei) const override {
    return geom_t::edges[ei];
  }
};

} /* End namespace HSF */
#endif /* end of include guard: MESH_GEOMETRY_TETRAHEDRON_HPP */
