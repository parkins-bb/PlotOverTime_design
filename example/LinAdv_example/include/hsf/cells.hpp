#ifndef MESH_CONNECTIVITY_CELLS_HPP
#define MESH_CONNECTIVITY_CELLS_HPP
/*! \file cells.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-15
 *  \modified 2023-04-18
 *  \brief class Cells
 */

#include "faces.hpp"

namespace HSF {

/*!
 * \class Cells
 * \brief Cells是Geometry的子类，包含该进程所有网格单元信息

 */
class Cells : public Geometries {
 private:
  std::vector<scalar> vol_;             /*!< 体积数组 */
  std::vector<Tensor1<scalar>> center_; /*!< 体心坐标数组 */

 public:
  /*! 根据单元数量进行构造，为数组分配空间 */
  Cells(label size = 0);
  /*! 拷贝构造 */
  Cells(const Cells& cells);
  /*! 移动构造 */
  Cells(Cells&& cells);
  Cells& operator=(Cells&& cells);

  /*! 析构，空间回收 */
  virtual ~Cells();

  /*! 设置长度 */
  virtual void setNum(label size);

  /*! 根据点的几何与拓扑信息计算出各单元体积 */
  // void calcCellVol(const Faces& faces, const std::vector<std::vector<label> >&
  // cell2face);
  /*! 根据点的几何信息、单元类型以及构成单元的点集计算该单元的体积 */
  void calcCellVol(const Nodes& nodes);
  void calcCellVol(label i, const Nodes& nodes);
  /*! 返回局部编号为index的单元的体积 */
  inline scalar vol(label index) const { return vol_[index]; }
  /*! 返回单元的体积数组 */
  inline const scalar* vol() const { return vol_.data(); }
  inline scalar* vol() { return vol_.data(); }

  /*! 根据点的几何与拓扑信息计算出各单元中心坐标 */
  void calcCellCenter(const Nodes& nodes);
  void calcCellCenter(label i, const Nodes& nodes);
  /*! 返回局部编号为index的单元的中心坐标 */
  inline Tensor1<scalar> center(label index) const { return center_[index]; }
  /*! 返回单元中心坐标数组 */
  inline const Tensor1<scalar>* center() const { return center_.data(); }
  inline Tensor1<scalar>* center() { return center_.data(); }
  void clear();
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_CONNECTIVITY_CELLS_HPP */
