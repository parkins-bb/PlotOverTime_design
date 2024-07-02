#ifndef MESH_GEOMETRY_POINT_HPP
#define MESH_GEOMETRY_POINT_HPP
/*! \file point.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-14
 *  \brief  class Point
 */

#include "geometry.hpp"

namespace HSF {

class Point : public Geometry {
 public:
  typedef BasicGeometry::Prop<BasicGeometry::POINT> geom_t;

  Point();
  Point(llabel *ind, int *attr = NULL);
  virtual ~Point() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void getVertices(std::vector<int> &v) const override;
  /// Returns geometry's type
  virtual Type getType() const override { return Geometry::POINT; }
  virtual Point *duplicate() const override {
    return new Point(indices_, attribute_);
  }
  virtual int getNVertices() const override { return 1; }
  virtual int getNEdges() const override { return 0; }
  virtual int getNFaces() const override { return 0; }
  virtual int getNFaceVertices(int fi) const override { return 0; }
  virtual const int *getFaceVertices(int fi) const override { return NULL; }
  virtual const int *getEdgeVertices(int ei) const override { return NULL; }
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_GEOMETRY_POINT_HPP */
