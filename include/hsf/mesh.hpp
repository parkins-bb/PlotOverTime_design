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
  Communicator* comm_;
  Word name_;
  int dim_;
  /*! Geometry */
  Nodes nodes_;
  Edges edges_;
  Faces faces_;
  Cells cells_;
  /*! flag 0-nodeCoord 1-edgeLength 2-EdgeCenter 3-EdgeNorm 4-FaceArea
   * 5-FaceCenter 6-FaceNorm 7-CellVol 8-CellCenter */
  bool statusFlag_[9];

  /*! length: nProcs+1, determine element global index range for each process */
  std::vector<llabel> elemStartId_;

  /*! attribute for each elem */
  std::vector<int> attribute_;
  /*! A list of all unique element attributes used by the Mesh. */
  std::vector<int> attributes_;

  /*! type for each elem */
  std::vector<int> cellTypes_;

  /*!SubSets */
  std::vector<SubSet> subsets_;
  map<std::pair<SetType, int>, int> typeId2pos_;
  map<std::pair<SetType, std::string>, int> typeName2pos_;

 public:
  typedef BasicGeometry::Prop<BasicGeometry::SEGMENT> seg_t;
  typedef BasicGeometry::Prop<BasicGeometry::TRIANGLE> tri_t;
  typedef BasicGeometry::Prop<BasicGeometry::SQUARE> quad_t;
  typedef BasicGeometry::Prop<BasicGeometry::TETRAHEDRON> tet_t;
  typedef BasicGeometry::Prop<BasicGeometry::CUBE> hex_t;
  typedef BasicGeometry::Prop<BasicGeometry::PRISM> pri_t;
  typedef BasicGeometry::Prop<BasicGeometry::PYRAMID> pyr_t;

 private:
  void initMesh(int dim, label nVert, label nElem, label nBdrElem);

 public:
  /*! 默认构造 */
  Mesh(Communicator* comm = NULL);
  /*! 析构，空间回收*/
  virtual ~Mesh();
  /*! Move constructor, useful for using a Mesh as a function return value */
  Mesh(Mesh&& mesh);
  /*! Move assignment operstor. */
  Mesh& operator=(Mesh&& mesh);
  /*! Explicitly delete the copy assignment operator. */
  Mesh& operator=(const Mesh& mesh) = delete;

  /*! * * * * * * * * * * * * * * * Data * * * * * * * * * * * * * */
  inline Communicator* comm() const { return comm_; }
  inline int getDimension() const { return dim_; }
  inline void setDimension(int dim) { dim_ = dim; }
  inline Word getName() const { return name_; }

  /*! 待完善补充 */
  inline label getNNodes() const { return nodes_.getNum(); }
  inline label getNEdges() const { return edges_.getNum(); }
  inline label getNFaces() const { return faces_.getNum(); }
  inline label getNCells() const { return cells_.getNum(); }
  inline label getNElem() const {
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
  virtual label getNDofs() const { return 0; }

  /*! if this processor has attribute id */
  bool hasAttribute(int id) const;
  const std::vector<int>& getAttribute() const { return attribute_; }

  inline void setBdrNum(label size) { size = size <= 0 ? 0 : size; }

  int elemOwnerRank(label index) const {
    int rank = 0;
    while (index >= elemStartId_[rank + 1] || index < elemStartId_[rank])
      rank++;
    return rank;
  }

  int elemStartId(label procId) const { return elemStartId_[procId]; }
  /*! * * * * * * * * * * * * * * * Geometry * * * * * * * * * * * * * */
  /*! 几何元素访问 */
  const std::vector<int>& getCellTypes() const { return cellTypes_; }
  inline const Nodes& node() const { return nodes_; }
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

  virtual void updateNodeCoord() {
    statusFlag_[0] = true;
    for (int i = 1; i < 9; i++) statusFlag_[i] = false;
  }
  virtual void calcEdgeLength() { statusFlag_[1] = true; }
  virtual void calcEdgeCenter() { statusFlag_[2] = true; }
  virtual void calcEdgeNorm() { statusFlag_[3] = true; }
  virtual void calcFaceArea() { statusFlag_[4] = true; }
  virtual void calcFaceCenter() { statusFlag_[5] = true; }
  virtual void calcFaceNorm() { statusFlag_[6] = true; }
  virtual void calcCellVol() { statusFlag_[7] = true; }
  virtual void calcCellCenter() { statusFlag_[8] = true; }

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
  inline Topology* getElem2ElemTopo() const {
    return getConnectivity(dim_, dim_);
  }
  inline Topology* getElem2NodeTopo() const { return getConnectivity(dim_, 0); }
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
  const SubSet* getSubSet(int index, SetType t) const;
  const SubSet* getSubSet(string name, SetType t) const;

  /*! * * * * * * * * * * * * * * * IO * * * * * * * * * * * * * */
  /*! 并行IO接口 */
  // virtual void readFromXdmf(Word fileName) = 0;
  // virtual void writeToXdmf(Word fileName) = 0;
};

} /* End namespace HSF */

#endif /* end of include guard: REGION_MESH_MESH_HPP */
