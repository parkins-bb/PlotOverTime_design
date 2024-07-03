#ifndef REGION_MESH_MESH_HPP
#define REGION_MESH_MESH_HPP
/*! \file mesh.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-11
 *  \modified 2024-02-05
 *  \brief Mesh 基类
 */

#include "cells.hpp"
#include "connectivity.hpp"
#include "subSet.hpp"

namespace HSF {

/*!
 * \class Mesh
 * \brief 网格由Geometry+Topolopy两部分组成，此外还有分布式并行相关信息
 */
class Mesh : public Connectivity {
 protected:
  friend SubSet;
  Communicator* comm_;  // 指向通信器的指针，用于并行计算
  Word name_;   // 网格的名称
  int dim_;     // 网格的维度
  /*! Geometry */
  Nodes nodes_;  // 节点的几何信息
  Edges edges_;  // 边的几何信息
  Faces faces_;  // 面的几何信息
  Cells cells_;  // 单元格的几何信息
  /*! flag 0-nodeCoord 1-edgeLength 2-EdgeCenter 3-EdgeNorm 4-FaceArea
   * 5-FaceCenter 6-FaceNorm 7-CellVol 8-CellCenter */
  bool statusFlag_[9];  // 状态标志数组，用于标记几何信息的计算状态

  /*! length: nProcs+1, determine element global index range for each process */
  std::vector<llabel> elemStartId_;  // 存储每个进程的全局元素索引范围

  /*! attribute for each elem */
  std::vector<int> attribute_;  // 每个单元的属性
  /*! A list of all unique element attributes used by the Mesh. */
  std::vector<int> attributes_;  // 网格使用的所有唯一单元属性列表

  /*! type for each elem */
  std::vector<int> cellTypes_;  // 每个元素的类型

  /*!SubSets */
  std::vector<SubSet> subsets_;  // 子集列表
  map<std::pair<SetType, int>, int> typeId2pos_;  // 用于通过类型和ID查找子集位置的映射
  map<std::pair<SetType, std::string>, int> typeName2pos_;  // 用于通过类型和名称查找子集位置的映射

 public:
  typedef BasicGeometry::Prop<BasicGeometry::SEGMENT> seg_t;
  typedef BasicGeometry::Prop<BasicGeometry::TRIANGLE> tri_t;
  typedef BasicGeometry::Prop<BasicGeometry::SQUARE> quad_t;
  typedef BasicGeometry::Prop<BasicGeometry::TETRAHEDRON> tet_t;
  typedef BasicGeometry::Prop<BasicGeometry::CUBE> hex_t;
  typedef BasicGeometry::Prop<BasicGeometry::PRISM> pri_t;
  typedef BasicGeometry::Prop<BasicGeometry::PYRAMID> pyr_t;

 private:
  void initMesh(int dim, label nVert, label nElem, label nBdrElem);  // 初始化网格函数，设置网格的维度、顶点数、单元数和边界单元数

 public:
  /*! 默认构造 */
  Mesh(Communicator* comm = NULL);  // 默认构造函数，初始化通信器
  /*! 析构，空间回收*/
  virtual ~Mesh();
  /*! Move constructor, useful for using a Mesh as a function return value */
  Mesh(Mesh&& mesh);  // 移动构造函数
  /*! Move assignment operstor. */
  Mesh& operator=(Mesh&& mesh);  // 移动赋值运算符
  /*! Explicitly delete the copy assignment operator. */
  Mesh& operator=(const Mesh& mesh) = delete;   // 显式删除拷贝赋值运算符，防止对象被拷贝

  /*! * * * * * * * * * * * * * * * Data * * * * * * * * * * * * * */
  inline Communicator* comm() const { return comm_; }  // 返回通信器指针
  inline int getDimension() const { return dim_; }     // 返回网格的维度
  inline void setDimension(int dim) { dim_ = dim; }    // 设置网格的维度
  inline Word getName() const { return name_; }        // 返回网格的名称

  /*! 待完善补充 */
  inline label getNNodes() const { return nodes_.getNum(); }   // 返回节点数量
  inline label getNEdges() const { return edges_.getNum(); }   // 返回边的数量
  inline label getNFaces() const { return faces_.getNum(); }   // 返回面的数量
  inline label getNCells() const { return cells_.getNum(); }   // 返回体单元的数量
  inline label getNElem() const {   // 返回单元的数量，根据维度来确定是否返回边、面或体的数量
    switch (dim_) {
      case 1:
        return getNEdges();
      case 2:
        return getNFaces();
      case 3:
      default:
        return getNCells();
    }
  }

  /*! get mesh dofs */
  virtual label getNDofs() const { return 0; }    // 返回mesh自由度数量

  /*! if this processor has attribute id */
  bool hasAttribute(int id) const;   // 检查当前进程是否包含某个id属性
  const std::vector<int>& getAttribute() const { return attribute_; }   // 返回属性列表

  inline void setBdrNum(label size) { size = size <= 0 ? 0 : size; }    // 设置边界单元的数量

  int elemOwnerRank(label index) const {    // 返回指定索引的元素所属进程的rank
    int rank = 0;
    while (index >= elemStartId_[rank + 1] || index < elemStartId_[rank])
      rank++;
    return rank;
  }

  int elemStartId(label procId) const { return elemStartId_[procId]; }   // 返回指定进程的元素的起始索引
  /*! * * * * * * * * * * * * * * * Geometry * * * * * * * * * * * * * */
  /*! 几何元素访问 */
  const std::vector<int>& getCellTypes() const { return cellTypes_; }    // 返回元素类型列表
  inline const Nodes& node() const { return nodes_; }   // 返回node, edge, face, cell的引用
  inline const Edges& edge() const { return edges_; }
  inline const Faces& face() const { return faces_; }
  inline const Cells& cell() const { return cells_; }
  inline Nodes& node() { return nodes_; }
  inline Edges& edge() { return edges_; }
  inline Faces& face() { return faces_; }
  inline Cells& cell() { return cells_; }
  inline Geometries* getGeometry(int dim) {
    switch (dim) {
      case 0:
        return &nodes_;
      case 1:
        return &edges_;
      case 2:
        return &faces_;
      case 3:
      default:
        return &cells_;
    }
  }
  inline Geometries* getElems() {
    switch (dim_) {
      case 1:
        return &edges_;
      case 2:
        return &faces_;
      case 3:
      default:
        return &cells_;
    }
  }
  inline Geometries* getFaces() {
    switch (dim_) {
      case 1:
        return &nodes_;
      case 2:
        return &edges_;
      case 3:
        return &faces_;
    }
    return 0;
  }
  inline label getNumElems() const {
    switch (dim_) {
      case 1:
        return edges_.getNum();
      case 2:
        return faces_.getNum();
      case 3:
        return cells_.getNum();
    }
    return 0;
  }

  virtual void updateNodeCoord() {   // 更新节点坐标
    statusFlag_[0] = true;
    for (int i = 1; i < 9; i++) statusFlag_[i] = false;
  }
  virtual void calcEdgeLength() { statusFlag_[1] = true; }    // 计算边长度
  virtual void calcEdgeCenter() { statusFlag_[2] = true; }    // 计算边中心
  virtual void calcEdgeNorm() { statusFlag_[3] = true; }      // 计算边的法向量
  virtual void calcFaceArea() { statusFlag_[4] = true; }      // 计算面的面积
  virtual void calcFaceCenter() { statusFlag_[5] = true; }    // 计算面的中心
  virtual void calcFaceNorm() { statusFlag_[6] = true; }      // 计算面的法向量
  virtual void calcCellVol() { statusFlag_[7] = true; }       // 计算体单元体积
  virtual void calcCellCenter() { statusFlag_[8] = true; }    // 计算体单元中心

  /*! 几何信息直接访问 */
  inline Tensor1<scalar>* nodeCoords() {
    if (!statusFlag_[0]) updateNodeCoord();
    return nodes_.coord();
  }
  inline Tensor1<scalar> nodeCoords(label nodeIndex) {
    if (!statusFlag_[0]) updateNodeCoord();
    return nodes_.coord(nodeIndex);
  }
  inline scalar edgeLength(label edgeIndex) {
    if (!statusFlag_[1]) calcEdgeLength();
    return edges_.length(edgeIndex);
  }
  inline scalar* edgeLengths() {
    if (!statusFlag_[1]) calcEdgeLength();
    return edges_.length();
  }
  inline Tensor1<scalar> edgeNorm(label edgeIndex) {
    if (!statusFlag_[3]) calcEdgeNorm();
    return edges_.norm(edgeIndex);
  }
  inline Tensor1<scalar>* edgeNorms() {
    if (!statusFlag_[3]) calcEdgeNorm();
    return edges_.norm();
  }
  inline Tensor1<scalar> edgeCenter(label edgeIndex) {
    if (!statusFlag_[2]) calcEdgeCenter();
    return edges_.center(edgeIndex);
  }
  inline Tensor1<scalar>* edgeCenters() {
    if (!statusFlag_[2]) calcEdgeCenter();
    return edges_.center();
  }
  inline scalar faceArea(label faceIndex) {
    if (!statusFlag_[4]) calcFaceArea();
    return faces_.area(faceIndex);
  }
  inline scalar* faceAreas() {
    if (!statusFlag_[4]) calcFaceArea();
    return faces_.area();
  }
  inline Tensor1<scalar> faceNorm(label faceIndex) {
    if (!statusFlag_[6]) calcFaceNorm();
    return faces_.norm(faceIndex);
  }
  inline Tensor1<scalar>* faceNorms() {
    if (!statusFlag_[6]) calcFaceNorm();
    return faces_.norm();
  }
  inline Tensor1<scalar> faceCenter(label faceIndex) {
    if (!statusFlag_[5]) calcFaceCenter();
    return faces_.center(faceIndex);
  }
  inline Tensor1<scalar>* faceCenters() {
    if (!statusFlag_[5]) calcFaceCenter();
    return faces_.center();
  }

  inline scalar cellVol(label cellIndex) {
    if (!statusFlag_[7]) calcCellVol();
    return cells_.vol(cellIndex);
  }
  inline scalar* cellVols() {
    if (!statusFlag_[7]) calcCellVol();
    return cells_.vol();
  }
  inline Tensor1<scalar> cellCenter(label cellIndex) {
    if (!statusFlag_[8]) calcCellCenter();
    return cells_.center(cellIndex);
  }
  inline Tensor1<scalar>* cellCenters() {
    if (!statusFlag_[8]) calcCellCenter();
    return cells_.center();
  }

  /*! * * * * * * * * * * * * * * * Topolopy * * * * * * * * * * * * * */
  /*! Dim	体	    面	    线	    点
        3	Cell	Face	Edge	Node
        2	Face	Edge	x   	Node
        1	Edge	Node	x   	Node */
  inline Topology* getElem2ElemTopo() const {   // 返回单元到单元的拓扑结构
    return getConnectivity(dim_, dim_);
  }
  inline Topology* getElem2NodeTopo() const { return getConnectivity(dim_, 0); }   // 返回单元到节点的拓扑结构
  inline Topology* getElem2EdgeTopo() const {
    ASSERT(dim_ > 1);
    return getConnectivity(dim_, 1);
  }
  inline void setElem2ElemTopo(Topology* topo) {
    setConnectivity(dim_, dim_, topo);
  }
  inline void setElem2EdgeTopo(Topology* topo) {
    ASSERT(dim_ > 1);
    setConnectivity(dim_, 1, topo);
  }
  inline void setElem2NodeTopo(Topology* topo) {
    setConnectivity(dim_, 0, topo);
  }

  /*! * * * * * * * * * * * * * * * Subset * * * * * * * * * * * * * */
  const SubSet* getSubSet(int index, SetType t) const;     // 通过索引和类型返回子集
  const SubSet* getSubSet(string name, SetType t) const;   // 通过名称和类型返回子集

  /*! * * * * * * * * * * * * * * * IO * * * * * * * * * * * * * */
  /*! 并行IO接口 */
  // virtual void readFromXdmf(Word fileName) = 0;
  // virtual void writeToXdmf(Word fileName) = 0;
};

} /* End namespace HSF */

#endif /* end of include guard: REGION_MESH_MESH_HPP */
