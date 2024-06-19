/*! \file nodes.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-13
 *  \modified 2023-12-21
 *  \brief class Nodes(Vertices)
 */

#ifndef MESH_CONNECTIVITY_NODES_HPP
#define MESH_CONNECTIVITY_NODES_HPP
#include "geometries.hpp"

namespace HSF {

/*!
 * \class Nodes
 * \brief Nodes是Geometries的子类，包含该进程所有网格点信息
 */
class Nodes : public Geometries {
 private:
  std::vector<Tensor1<scalar>> coord_; /*!< 坐标数组 */

 public:
  /*! 根据点数量进行构造，为数组分配空间 */
  Nodes(label size = 0);
  /*! xyz坐标值构造，x、y、z均为数组 */
  Nodes(label size, scalar* x, scalar* y = NULL, scalar* z = NULL);
  /*! 矢量场形式构造 */
  Nodes(label size, Tensor1<scalar>* vec);
  /*! 拷贝构造 */
  Nodes(const Nodes& nodes);
  /*! 移动构造 */
  Nodes(Nodes&& nodes);
  Nodes& operator=(Nodes&& nodes);
  /*! 析构，空间回收 */
  virtual ~Nodes();

  /*! xyz坐标值初始化，x、y、z均为数组 */
  void setCoord(scalar* x, scalar* y = NULL, scalar* z = NULL);
  /*! 矢量场形式初始化 */
  void setCoord(Tensor1<scalar>* vec);
  void setCoord(label i, const Tensor1<scalar>& vec);

  /*! 改变coord_长度 */
  virtual void setNum(label size);

  /*! 返回局部编号为index的Node的x坐标 */
  inline scalar x(label index) const { return coord_[index][0]; }
  /*! 返回局部编号为index的Node的y坐标 */
  inline scalar y(label index) const { return coord_[index][1]; }
  /*! 返回局部编号为index的Node的z坐标 */
  inline scalar z(label index) const { return coord_[index][2]; }

  /*! 返回局部编号为index的Node的坐标 */
  inline Tensor1<scalar> coord(label index) const { return coord_[index]; }
  /*! 返回该进程所有点的坐标数组 */
  // Tensor1<scalar>* coord();
  inline const Tensor1<scalar>* coord() const { return coord_.data(); }
  inline Tensor1<scalar>* coord() { return coord_.data(); }

  /*! node(i)由(x,y,z)移动到(x+dx,y+dy,z+dz) */
  void moveRelatively(label i, scalar dx, scalar dy = 0., scalar dz = 0.);
  /*! node(i)由(x,y,z)移动到(newx,newy,newz) */
  void moveTo(label i, scalar newx, scalar newy = 0., scalar newz = 0.);
  void moveNodes(const std::vector<Tensor1<scalar>>& displ);

  /*! 返回局部编号为index1与index2的两Node间的距离 */
  scalar distance(label index1, label index2) const {
    return mag(coord_[index1] - coord_[index2]);
  }

  void clear();

  /*! determine trueDim based on min/max differences detected each dimension */
  int calcTrueDim();
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_CONNECTIVITY_NODES_HPP */
