/*! \file connectivity.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-10
 *  \modified 2023-05-05
 *  \brief 包含几何元素间的连接关系
 */

#ifndef MESH_CONNECTIVITY_CONNECTIVITY_HPP
#define MESH_CONNECTIVITY_CONNECTIVITY_HPP
#include "topology.hpp"

namespace HSF {
enum class DataLocation;

/*! field data location */
// enum class SetType : int { NODE = 0, EDGE = 1, FACE = 2, CELL = 3, DOF = 4 };
typedef enum { NODE = 0, EDGE = 1, FACE = 2, CELL = 3, DOF = 4 } SetType;
template <typename TYPE_TO>
TYPE_TO enum_cast(int n);

std::string setType2str(SetType t);
SetType str2SetType(std::string str);
SetType str2SetType(const char* str);
DataLocation setType2DataLoc(SetType t, int dim);

/*!
 * \class Connectivity
 * \brief 存储连接关系，不提供构造函数，对用户透明
 */
class Connectivity {
 protected:
  /*! 连接关系，非结构网格时储存邻接/关联矩阵、提供迭代接口
   * 结构网格时只需提供迭代接口 */

  Topology* c2cTopo_; /*!< cell to cell connectivity */
  Topology* c2fTopo_; /*!< cell to face connectivity */
  Topology* c2eTopo_; /*!< cell to edge connectivity */
  Topology* c2nTopo_; /*!< cell to node connectivity */

  Topology* f2cTopo_; /*!< face to cell connectivity , */
  Topology* f2fTopo_; /*!< face to face connectivity , */
  Topology* f2eTopo_; /*!< face to edge connectivity */
  Topology* f2nTopo_; /*!< face to node connectivity , */

  Topology* e2cTopo_; /*!< edge to cell connectivity */
  Topology* e2fTopo_; /*!< edge to face connectivity */
  Topology* e2eTopo_; /*!< edge to edge connectivity */
  Topology* e2nTopo_; /*!< edge to node connectivity */

  Topology* n2cTopo_; /*!< node to cell connectivity */
  Topology* n2fTopo_; /*!< node to face connectivity */
  Topology* n2eTopo_; /*!< node to edge connectivity */
  Topology* n2nTopo_; /*!< node to node connectivity */

 public:
  /*! 默认构造，指针置null */
  Connectivity();
  /*! 移动构造 */
  Connectivity(Connectivity&& conn);
  Connectivity& operator=(Connectivity&& conn);
  /*! 空析构 */
  virtual ~Connectivity();

  Topology* getConnectivity(SetType from, SetType to) const;
  Topology* getConnectivity(int from, int to) const;
  void setConnectivity(SetType from, SetType to, Topology* topo);
  void setConnectivity(int from, int to, Topology* topo);
  /*! 返回相应拓扑的指针 */
  inline Topology* getCell2CellTopo() const { return c2cTopo_; }
  inline Topology* getCell2FaceTopo() const { return c2fTopo_; }
  inline Topology* getCell2EdgeTopo() const { return c2eTopo_; }
  inline Topology* getCell2NodeTopo() const { return c2nTopo_; }
  inline Topology* getFace2CellTopo() const { return f2cTopo_; }
  inline Topology* getFace2FaceTopo() const { return f2fTopo_; }
  inline Topology* getFace2EdgeTopo() const { return f2eTopo_; }
  inline Topology* getFace2NodeTopo() const { return f2nTopo_; }
  inline Topology* getEdge2CellTopo() const { return e2cTopo_; }
  inline Topology* getEdge2FaceTopo() const { return e2fTopo_; }
  inline Topology* getEdge2EdgeTopo() const { return e2eTopo_; }
  inline Topology* getEdge2NodeTopo() const { return e2nTopo_; }
  inline Topology* getNode2CellTopo() const { return n2cTopo_; }
  inline Topology* getNode2FaceTopo() const { return n2fTopo_; }
  inline Topology* getNode2EdgeTopo() const { return n2eTopo_; }
  inline Topology* getNode2NodeTopo() const { return n2nTopo_; }
  inline void setCell2CellTopo(Topology* topo) { c2cTopo_ = topo; }
  inline void setCell2FaceTopo(Topology* topo) { c2fTopo_ = topo; }
  inline void setCell2EdgeTopo(Topology* topo) { c2eTopo_ = topo; }
  inline void setCell2NodeTopo(Topology* topo) { c2nTopo_ = topo; }
  inline void setFace2CellTopo(Topology* topo) { f2cTopo_ = topo; }
  inline void setFace2FaceTopo(Topology* topo) { f2fTopo_ = topo; }
  inline void setFace2EdgeTopo(Topology* topo) { f2eTopo_ = topo; }
  inline void setFace2NodeTopo(Topology* topo) { f2nTopo_ = topo; }
  inline void setEdge2CellTopo(Topology* topo) { e2cTopo_ = topo; }
  inline void setEdge2FaceTopo(Topology* topo) { e2fTopo_ = topo; }
  inline void setEdge2EdgeTopo(Topology* topo) { e2eTopo_ = topo; }
  inline void setEdge2NodeTopo(Topology* topo) { e2nTopo_ = topo; }
  inline void setNode2CellTopo(Topology* topo) { n2cTopo_ = topo; }
  inline void setNode2FaceTopo(Topology* topo) { n2fTopo_ = topo; }
  inline void setNode2EdgeTopo(Topology* topo) { n2eTopo_ = topo; }
  inline void setNode2NodeTopo(Topology* topo) { n2nTopo_ = topo; }
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_CONNECTIVITY_CONNECTIVITY_HPP */
