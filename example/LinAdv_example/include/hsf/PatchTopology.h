#ifndef SOURCE_WRAPPER_HSFPATCHTOPOLOGY_H
#define SOURCE_WRAPPER_HSFPATCHTOPOLOGY_H
/*! \file PatchTopology.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include "Array.h"
#include "EntityUtilities.h"
#include "PatchData.h"
#include "hsfRegion.hpp"
#include "ustrMesh.hpp"

namespace HSF {

namespace hier {

template <int DIM>
class Patch;

/**
 * @brief 网格片拓扑类，描述一个网格片上网格实体之间的拓扑关系。
 *
 * 两类网格实体A与B之间的拓扑关系表示为<A,
 *B>，意思是每个A类网格实体与哪些B类网格实体邻接。
 *
 * 某两种网格实体之间的拓扑关系包含“索引范围”和“索引”两部分。
 * 这两部分用两个数组描述：1、索引范围数组；2、索引数组。
 * 例如，网格单元与网格结点之间的拓扑关系包含如下两个数组：
 * cell_adj_nodes_extent和cell_adj_nodes_indices。其中，
 * {cell_adj_nodes_indices[id]: cell_adj_nodes_extent[i] <= id <
 *cell_adj_nodes_extent[i+1]}
 * 是所有与第i个单元相邻的结点的索引。
 */
template <int DIM>
class PatchTopology {
 public:
  friend Patch<DIM>;
  PatchTopology(Region* mesh) { d_mesh = mesh; }

  ~PatchTopology() {}
  /**
   * @brief 获取<单元，单元>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getCellAdjacencyCells(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = dynamic_cast<GraphTopology*>(uMesh->getElem2Elem());

    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<单元，结点>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getCellAdjacencyNodes(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = DIM == 2 ? uMesh->getFace2Node() : uMesh->getCell2Node();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<单元，边>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getCellAdjacencyEdges(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = DIM == 2 ? uMesh->getFace2Edge() : uMesh->getCell2Edge();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<单元，面>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getCellAdjacencyFaces(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getCell2Face();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<结点，单元>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getNodeAdjacencyCells(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = DIM == 2 ? uMesh->getNode2Face() : uMesh->getNode2Cell();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<结点，结点>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getNodeAdjacencyNodes(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getNode2Node();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<结点，边>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getNodeAdjacencyEdges(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getNode2Edge();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<结点，面>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getNodeAdjacencyFaces(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getNode2Face();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<边，单元>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getEdgeAdjacencyCells(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = DIM == 2 ? uMesh->getEdge2Face() : uMesh->getEdge2Cell();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<边，结点>拓扑关系。
   * @param extent  输出参数，整型数组，存储边索引范围。
   * @param indices 输出参数，整型数组，存储边索引。
   */
  void getEdgeAdjacencyNodes(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getEdge2Node();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<边，边>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getEdgeAdjacencyEdges(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getEdge2Edge();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<边，面>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getEdgeAdjacencyFaces(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getEdge2Face();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<面，单元>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getFaceAdjacencyCells(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getFace2Cell();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<面，结点>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getFaceAdjacencyNodes(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getFace2Node();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<面，边>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getFaceAdjacencyEdges(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getFace2Edge();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 获取<面，面>拓扑关系。
   * @param extent  输出参数，整型数组，存储索引范围。
   * @param indices 输出参数，整型数组，存储索引。
   */
  void getFaceAdjacencyFaces(HSF::tbox::Array<int>& extent,
                             HSF::tbox::Array<int>& indices) {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    GraphTopology* x = uMesh->getFace2Face();
    extent = tbox::Array<int>(x->getCprRowVector());
    indices = tbox::Array<int>(x->getColVector());
  }

  /**
   * @brief 设置<单元，结点>拓扑关系。
   *
   * @param can_extent 输入参数，整型数组，存储索引范围。
   * @param can_index  输入参数，整型数组，存储索引。
   *
   * @note <单元，结点>是基本的拓扑关系，其它拓扑关系均有该关系计算得到。
   */
  void setCellAdjacencyNodes(const HSF::tbox::Array<int>& can_extent,
                             const HSF::tbox::Array<int>& can_index) {}

  /**
   * @brief 获取网格片中单元的数目。
   */
  // int getNumberOfCells() const {}

  /**
   * @brief 获取网格片中结点的数目。
   */
  // int getNumberOfNodes() {}

  /**
   * @brief 获取网格片中边的数目。
   */
  // int getNumberOfEdges() {}

  /**
   * @brief 获取网格片中面的数目。
   */
  // int getNumberOfFaces() {}

  // void setCellType(HSF::tbox::Pointer<pdat::CellData<DIM, int> >
  // cell_type); HSF::tbox::Pointer<pdat::CellData<DIM, int> > getCellType();

 private:
  Region* d_mesh;
  // HSF::tbox::Array<int> d_cell_adjacency_cells_extent;
  // HSF::tbox::Array<int> d_cell_adjacency_cells_index;
  // HSF::tbox::Array<int> d_cell_adjacency_nodes_extent;
  // HSF::tbox::Array<int> d_cell_adjacency_nodes_index;
  // HSF::tbox::Array<int> d_cell_adjacency_edges_extent;
  // HSF::tbox::Array<int> d_cell_adjacency_edges_index;
  // HSF::tbox::Array<int> d_cell_adjacency_faces_extent;
  // HSF::tbox::Array<int> d_cell_adjacency_faces_index;
  // HSF::tbox::Array<int> d_face_adjacency_cells_extent;
  // HSF::tbox::Array<int> d_face_adjacency_cells_index;
  // HSF::tbox::Array<int> d_face_adjacency_nodes_extent;
  // HSF::tbox::Array<int> d_face_adjacency_nodes_index;
  // HSF::tbox::Array<int> d_face_adjacency_edges_extent;
  // HSF::tbox::Array<int> d_face_adjacency_edges_index;
  // HSF::tbox::Array<int> d_face_adjacency_faces_extent;
  // HSF::tbox::Array<int> d_face_adjacency_faces_index;
  // HSF::tbox::Array<int> d_edge_adjacency_cells_extent;
  // HSF::tbox::Array<int> d_edge_adjacency_cells_index;
  // HSF::tbox::Array<int> d_edge_adjacency_nodes_extent;
  // HSF::tbox::Array<int> d_edge_adjacency_nodes_index;
  // HSF::tbox::Array<int> d_edge_adjacency_edges_extent;
  // HSF::tbox::Array<int> d_edge_adjacency_edges_index;
  // HSF::tbox::Array<int> d_edge_adjacency_faces_extent;
  // HSF::tbox::Array<int> d_edge_adjacency_faces_index;
  // HSF::tbox::Array<int> d_node_adjacency_cells_extent;
  // HSF::tbox::Array<int> d_node_adjacency_cells_index;
  // HSF::tbox::Array<int> d_node_adjacency_nodes_extent;
  // HSF::tbox::Array<int> d_node_adjacency_nodes_index;
  // HSF::tbox::Array<int> d_node_adjacency_edges_extent;
  // HSF::tbox::Array<int> d_node_adjacency_edges_index;
  // HSF::tbox::Array<int> d_node_adjacency_faces_extent;
  // HSF::tbox::Array<int> d_node_adjacency_faces_index;
};

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFPATCHTOPOLOGY_H */
