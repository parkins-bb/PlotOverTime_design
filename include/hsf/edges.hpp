/*! \file edges.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-13
 *  \modified 2023-04-17
 *  \brief class Edges
 */

#ifndef MESH_CONNECTIVITY_EDGES_HPP
#define MESH_CONNECTIVITY_EDGES_HPP

#include "graphTopology.hpp"
#include "nodes.hpp"

namespace HSF {

/*!
 * \class Edges
 * \brief Edges是Geometries的子类，包含该进程所有网格边信息
 */
class Edges : public Geometries {
 private:
  std::vector<scalar> length_;          /*!< 边长数组 */
  std::vector<Tensor1<scalar>> norm_;   /*!< 单位矢数组 */
  std::vector<Tensor1<scalar>> center_; /*!< 边中心数组 */

 public:
  /*! 根据边数量进行构造，为数组分配空间 */
  Edges(label size = 0);
  /*! 拷贝构造 */
  Edges(const Edges& edges);
  /*! 移动构造 */
  Edges(Edges&& edges);
  Edges& operator=(Edges&& edges);
  /*! 析构，空间回收 */
  virtual ~Edges();

  /*! 设置长度 */
  virtual void setNum(label size);

  /*! 根据点的几何与内在拓扑信息（需要先建立）计算出各边单位矢 */
  void calcEdgeNorm(const GraphTopology& etbl, const Nodes& nodes);
  void calcEdgeNorm(label i, const GraphTopology& etbl, const Nodes& nodes);

  /*! 返回局部编号为index的边单位矢 */
  inline Tensor1<scalar> norm(label index) const { return norm_[index]; }
  /*! 返回单位矢数组 */
  inline const Tensor1<scalar>* norm() const { return norm_.data(); }
  inline Tensor1<scalar>* norm() { return norm_.data(); }

  /*! 根据点的几何与内在拓扑信息（需要先建立）计算出各边长 */
  void calcEdgeLength(const GraphTopology& etbl, const Nodes& nodes);
  void calcEdgeLength(label i, const GraphTopology& etbl, const Nodes& nodes);

  /*! 返回局部编号为index的边长度 */
  inline scalar length(label index) const { return length_[index]; }
  /*! 返回边长数组 */
  inline const scalar* length() const { return length_.data(); }
  inline scalar* length() { return length_.data(); }

  /*! 根据点的几何与内在拓扑信息（需要先建立）计算出各边中心 */
  void calcEdgeCenter(const GraphTopology& etbl, const Nodes& nodes);
  void calcEdgeCenter(label i, const GraphTopology& etbl, const Nodes& nodes);

  /*! 返回局部编号为index的边中心坐标 */
  inline Tensor1<scalar> center(label index) const { return center_[index]; }
  /*! 返回边中心坐标数组 */
  inline const Tensor1<scalar>* center() const { return center_.data(); }
  inline Tensor1<scalar>* center() { return center_.data(); }
  void clear();
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_CONNECTIVITY_EDGES_HPP */
