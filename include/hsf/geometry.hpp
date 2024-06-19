#ifndef MESH_GEOMETRY_GEOMETRY_HPP
#define MESH_GEOMETRY_GEOMETRY_HPP
/*! \file geometry.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-14
 *  \modified 2023-12-15
 *  \brief abstract class Geometry
 */

#include "basicGeo.hpp"
#include "utilities.h"

namespace HSF {

class Geometry {
 protected:
  /// Geometry's attribute (specifying material property, etc).
  int *attribute_;
  /// Geometry's type from the BasicGeometry's perspective
  BasicGeometry::Type baseGeom_;
  /// vertex index of this geometry, don't own data itself
  llabel *indices_;

 public:
  /// Constants for the classes derived from Geometry.
  enum Type {
    POINT,
    SEGMENT,
    TRIANGLE,
    QUADRILATERAL,
    TETRAHEDRON,
    HEXAHEDRON,
    WEDGE,
    PYRAMID
  };

  /// Default geometry constructor.
  explicit Geometry(BasicGeometry::Type bg = BasicGeometry::INVALID)
      : attribute_(NULL), baseGeom_(bg), indices_(NULL) {}
  /// Destroys geometry.
  virtual ~Geometry() {}

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  inline BasicGeometry::Type getBasicGeometryType() const { return baseGeom_; }
  inline void setBasicGeometryType(BasicGeometry::Type type) {
    baseGeom_ = type;
  }

  /// Return geometry's attribute.
  inline int getAttribute() const { return attribute_ ? *attribute_ : -1; }
  /// set geometry's attribute.
  inline void setAttribute(int *attr) { attribute_ = attr; }
  /// set geometry's attribute.
  inline void setAttribute(const int attr) {
    if (attribute_) *attribute_ = attr;
  }

  /// set the indices_ the geometry according to the input.
  inline void setVertices(llabel *ind) { indices_ = ind; }
  inline const llabel *getVertices() const { return indices_; }
  inline llabel *getVertices() { return indices_; }

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  /// Returns geometry's type
  virtual void getVertices(std::vector<int> &v) const = 0;
  virtual Type getType() const = 0;
  virtual Geometry *duplicate() const = 0;
  virtual int getNVertices() const = 0;
  virtual int getNEdges() const = 0;
  virtual int getNFaces() const = 0;
  virtual int getNFaceVertices(int fi) const = 0;
  virtual const int *getFaceVertices(int fi) const = 0;
  virtual const int *getEdgeVertices(int ei) const = 0;
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_GEOMETRY_GEOMETRY_HPP */
