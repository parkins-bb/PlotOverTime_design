/*! \file faces.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-15
 *  \modified 2023-04-17
 *  \brief class Faces
 */

#ifndef MESH_CONNECTIVITY_FACES_HPP
#define MESH_CONNECTIVITY_FACES_HPP
#include "edges.hpp"
#include "graphTopology.hpp"

namespace HSF {

/*!
 * \class Faces
 * \brief Faces是Geometries的子类，包含该进程所有网格面信息
 */
class Faces : public Geometries {
 private:
  std::vector<scalar> area_;            /*!< 面积数组 */
  std::vector<Tensor1<scalar>> norm_;   /*!< 面法向数组 */
  std::vector<Tensor1<scalar>> center_; /*! 面中心坐标数组 */

 public:
  /*! 根据面数量进行构造，为数组分配空间 */
  Faces(label size = 0);
  /*! 拷贝构造 */
  Faces(const Faces& faces);
  /*! 移动构造 */
  Faces(Faces&& faces);
  Faces& operator=(Faces&& faces);

  /*! 析构，空间回收 */
  virtual ~Faces();

  /*! 设置长度 */
  virtual void setNum(label size);

  /*! 根据点的几何与拓扑信息计算出各面面积 */
  void calcFaceArea(const std::vector<int>& faceTypes, const GraphTopology& ftbl,
                    const Nodes& nodes);
  void calcFaceArea(label i, int faceType, const GraphTopology& ftbl,
                    const Nodes& nodes);

  /*! 返回局部编号为index的面的面积 */
  inline scalar area(label index) const { return area_[index]; }
  /*! 返回面的面积数组 */
  inline const scalar* area() const { return area_.data(); }
  inline scalar* area() { return area_.data(); }

  /*! 根据点的几何与拓扑信息计算出各面法向 */
  void calcFaceNorm(const GraphTopology& ftbl, const Nodes& nodes);
  void calcFaceNorm(label i, const GraphTopology& ftbl, const Nodes& nodes);

  /*! 返回局部编号为index的面的面法向 */
  inline Tensor1<scalar> norm(label index) const { return norm_[index]; }
  /*! 返回面的面法向数组 */
  inline const Tensor1<scalar>* norm() const { return norm_.data(); }
  inline Tensor1<scalar>* norm() { return norm_.data(); }

  /*! 根据点的几何与拓扑信息计算出各面中心坐标 */
  void calcFaceCenter(const std::vector<int>& faceTypes, const GraphTopology& ftbl,
                      const Nodes& nodes);
  void calcFaceCenter(label i, int faceType, const GraphTopology& ftbl,
                      const Nodes& nodes);

  /*! 返回局部编号为index的面的面中心坐标 */
  inline Tensor1<scalar> center(label index) const { return center_[index]; }
  /*! 返回面的面中心坐标数组 */
  inline const Tensor1<scalar>* center() const { return center_.data(); }
  inline Tensor1<scalar>* center() { return center_.data(); }
  void clear();
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_CONNECTIVITY_FACES_HPP */
