#ifndef MESH_GEOMETRY_TRIANGLE_HPP
#define MESH_GEOMETRY_TRIANGLE_HPP
/*! \file triangle.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-16
 *  \brief  class Triangle
 */

#include "geometry.hpp"

namespace HSF {

class Triangle : public Geometry {
 public:
  typedef BasicGeometry::Prop<BasicGeometry::TRIANGLE> geom_t;

  Triangle();
  Triangle(llabel *ind, int *attr = NULL);
  virtual ~Triangle() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void getVertices(std::vector<int> &v) const override;
  /// Returns geometry's type
  virtual Type getType() const override { return Geometry::TRIANGLE; }
  virtual Triangle *duplicate() const override {
    return new Triangle(indices_, attribute_);
  }
  virtual int getNVertices() const override { return 3; }
  virtual int getNEdges() const override { return 3; }
  virtual int getNFaces() const override { return 0; }
  virtual int getNFaceVertices(int fi) const override { return 0; }
  virtual const int *getFaceVertices(int fi) const override { return NULL; }
  virtual const int *getEdgeVertices(int ei) const override {
    return geom_t::edges[ei];
  }
};

} /* End namespace HSF */
#endif /* end of include guard: MESH_GEOMETRY_TRIANGLE_HPP */
