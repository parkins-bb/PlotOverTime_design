/*! \file geometries.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-10
 *  \modified 2023-12-20
 *  \brief abstract class Geometries
 */

#ifndef MESH_CONNECTIVITY_GEOMETRIES_HPP
#define MESH_CONNECTIVITY_GEOMETRIES_HPP
#include "hexahedron.hpp"
#include "point.hpp"
#include "pyramid.hpp"
#include "quadrilateral.hpp"
#include "scalar.hpp"
#include "segment.hpp"
#include "tensor.hpp"
#include "tetrahedron.hpp"
#include "triangle.hpp"
#include "utilityBasicFunction.h"
#include "utilityExceptions.h"
#include "wedge.hpp"

namespace HSF {

template <typename T>
static void checkEnlarge(std::vector<T> &array, label size) {
  if (size >= (label)array.size()) {
    array.resize(size + 1);
  }
}

/*!
 * \class Geometries
 * \brief Nodes、Edges等几何元素的基类
 */
class Geometries {
 protected:
  static int spaceDim_; /*!< 元素所在空间维度 */
  label num_;           /*!< 元素数量，包含ghost */
  label realNum_;       /*!< 元素数量，不包含ghost */
  label ghostNum_;      /*!< ghost元素数量 */
  // label sharedNum_;         /*!< shared元素数量 */
  llabel globalNum_; /*!< 总元素数量,各进程realNum_之和减去重复的部分 */
  std::vector<Geometry *> geoms_; /*!< all geometries */

 public:
  Geometries(label size = 0);
  virtual ~Geometries();
  Geometries(const Geometries &copy);

  Geometries(Geometries &&geo);
  Geometries &operator=(Geometries &&geo);

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  inline int getSpaceDimension() const { return spaceDim_; }
  inline label getNum() const { return num_; }
  inline label getRealNum() const { return realNum_; }
  inline label getGhostNum() const { return ghostNum_; }
  // inline label getSharedNum() const { return sharedNum_; }
  inline llabel getGlobalNum() const { return globalNum_; }
  // inline static void setSpaceDimension(int d) { Geometries::spaceDim_ = d; }
  inline virtual void setNum(label size) {
    ASSERT(size >= 0);
    if (!geoms_.empty())
      for (label i = size; i < num_; i++) DELETE_POINTER(geoms_[i]);
    num_ = size;
    geoms_.resize(size);
  }
  inline void setRealNum(label size) { realNum_ = size; }
  inline void setGhostNum(label size) { ghostNum_ = size; }
  // inline void setSharedNum(label size) { sharedNum_ = size; }
  inline void setGlobalNum(llabel size) { globalNum_ = size; }

  inline bool isGhost(label id) {
    ASSERT(id < num_);
    return id >= realNum_;
  }

  Geometry *getGeometry(label i) const { return geoms_[i]; }
  const std::vector<Geometry *> &getGeometry() const { return geoms_; }
  void setGeometry(label i, Geometry *geom) {
    freeGeometry(i);
    geoms_[i] = geom;
  }
  void freeGeometry(int i) { DELETE_POINTER(geoms_[i]); }

  inline label addGeo(Geometry *geom) {
    checkEnlarge(geoms_, num_);
    geoms_[num_] = geom;
    return num_++;
  }

  static Geometry *newGeometry(int geom);
  static void freeGeometry(Geometry *geom);
  void reorder(const std::vector<label> &perm);
  // static void printGeo(const Geometry *geo, std::ostream *out =
  // POUT.stream());

  void clear();
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_CONNECTIVITY_GEOMETRIES_HPP */
