#ifndef USTR_CONTACT_COLLISIONDETECTION_HPP
#define USTR_CONTACT_COLLISIONDETECTION_HPP
/*! \file collisionDetection.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2024-04-01
 *  \modified 2024-04-18
 *  \brief collision detection for ustr mesh
 */
#include <utility>

#include "AABBTree.hpp"
#include "connectivity.hpp"
#include "ustrMesh.hpp"
namespace HSF {

/*! TODO:  2. 将通信拓扑相关量加入成员变量； 3. 构建影像区通信算子  */
class CollisionDetection {
 private:
  USTRMesh* mesh_; /*!< ustr mesh */
  AABBTree* master_;
  AABBTree* slave_;
  AABBTree* globalMaster_;
  AABBTree* globalSlave_;
  // AABBTree* bboxTree_;   /*!< local AABBTree */
  // AABBTree* globalTree_; /*!< global bounding box tree */
  scalar padding_; /*!< contact gap */
  bool hasGhost_;  /*!< whether AABBTree contains ghost or not */
  std::pair<int, int> contactPair_; /*!< subset id */
  // std::vector<label> points_;
  std::vector<label>
      slavePoints_; /*!< points to determine ownership(node/edge/face id) */
  std::vector<label> masterPoints_;
  SetType mode_; /*!< tree build mode, default is node */
  int loopNum_;  /*!< when master is slave, only loop once */
  static map<int, std::set<label>> contactGhostCellId_;
  /*! boundary cell: 1, others: 0 */
  static std::vector<int> ifBdrCell_;

 public:
  CollisionDetection();
  /*! default: build AABBTree from boundary face */
  CollisionDetection(USTRMesh* mesh, scalar padding = 0);
  ~CollisionDetection();

  /*! init contactPair_, only call it once! */
  void initContactPair(int master, int slave);
  /*! build tree and point array after set contactPair_ */
  void build(bool hasGhost = 1);

  /*! get* functions */
  // AABBTree* getTree() { return bboxTree_; }
  // const AABBTree* getTree() const { return bboxTree_; }
  // AABBTree* getGlobalTree() { return globalTree_; }
  // const AABBTree* getGlobalTree() const { return globalTree_; }
  scalar getPadding() const { return padding_; }
  void setPadding(scalar p) { padding_ = p; }
  void setMode(SetType mode) { mode_ = mode; }
  SetType getMode() { return mode_; }
  static map<int, std::set<label>>& getContactGhostCell() {
    return contactGhostCellId_;
  }
  void destroy(); /*!< destroy AABBtree */

  /*! @param[in] mesh Mesh for building the bounding box tree.
   *  @param[in] tdim Topological dimension of the mesh entities to build the
   *  bounding box tree for.
   *  @param[in] entities List of entity indices (local to process) to compute
   *  the bounding box for
   *  @param[in] padding Value to pad (extend) the the bounding box of each
   *  entity by.
   *  @param[out] tree constructed AABBTree
   *  entity by.
   */
  static AABBTree* buildAABBTree(USTRMesh* mesh, int tdim, label num,
                                 const label* entities, scalar padding = 0);
  static AABBTree* buildAABBTree(USTRMesh* mesh, int tdim, bool hasGhost = 0,
                                 scalar padding = 0);
  /*! bulild AABBTree from midpoints of entities set(node/edge/face/cell) */
  static AABBTree* buildPointsAABBTree(USTRMesh* mesh, int tdim, label num,
                                       const label* entities);

  /*! build ifBdrCell_ vector */
  static void judgeBdrCell(USTRMesh* mesh);

  /*! debug: output points to vtk format */
  static void printPoints(std::string fileName, const std::vector<label>& ptIds,
                          Tensor1<scalar>* coord);

  /*! build contact ghost cells */
  void determineCollision(bool boundary_only = true);
};

} /* End namespace HSF */
#endif /* end of include guard: USTR_CONTACT_COLLISIONDETECTION_HPP */
