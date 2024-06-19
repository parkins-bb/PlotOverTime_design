#ifndef MESH_GEOMETRY_HEXAHEDRON_HPP
#define MESH_GEOMETRY_HEXAHEDRON_HPP
/*! \file hexahedron.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-16
 *  \brief  class Hexahedron
 */

#include "geometry.hpp"

namespace HSF {

class Hexahedron : public Geometry {
 public:
  typedef BasicGeometry::Prop<BasicGeometry::CUBE> geom_t;

  Hexahedron();
  Hexahedron(llabel *ind, int *attr = NULL);
  virtual ~Hexahedron() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void getVertices(std::vector<int> &v) const override;
  /// Returns geometry's type
  virtual Type getType() const override { return Geometry::HEXAHEDRON; }
  virtual Hexahedron *duplicate() const override {
    return new Hexahedron(indices_, attribute_);
  }
  virtual int getNVertices() const override { return 8; }
  virtual int getNEdges() const override { return 12; }
  virtual int getNFaces() const override { return 6; }
  virtual int getNFaceVertices(int fi) const override { return 4; }
  virtual const int *getFaceVertices(int fi) const override {
    return geom_t::faceVert[fi];
  }
  virtual const int *getEdgeVertices(int ei) const override {
    return geom_t::edges[ei];
  }
};

} /* End namespace HSF */
#endif /* end of include guard: MESH_GEOMETRY_HEXAHEDRON_HPP */
