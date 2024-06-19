#ifndef MESH_GEOMETRY_SEGMENT_HPP
#define MESH_GEOMETRY_SEGMENT_HPP
/*! \file segment.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-16
 *  \brief  class Segment
 */

#include "geometry.hpp"

namespace HSF {

class Segment : public Geometry {
 public:
  typedef BasicGeometry::Prop<BasicGeometry::SEGMENT> geom_t;

  Segment();
  Segment(llabel *ind, int *attr = NULL);
  virtual ~Segment() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void getVertices(std::vector<int> &v) const override;
  /// Returns geometry's type
  virtual Type getType() const override { return Geometry::SEGMENT; }
  virtual Segment *duplicate() const override {
    return new Segment(indices_, attribute_);
  }
  virtual int getNVertices() const override { return 2; }
  virtual int getNEdges() const override { return 0; }
  virtual int getNFaces() const override { return 0; }
  virtual int getNFaceVertices(int fi) const override { return 0; }
  virtual const int *getFaceVertices(int fi) const override { return NULL; }
  virtual const int *getEdgeVertices(int ei) const override { return NULL; }
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_GEOMETRY_SEGMENT_HPP */
