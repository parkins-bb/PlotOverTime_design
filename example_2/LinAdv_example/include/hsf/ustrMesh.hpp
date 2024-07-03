/*! \file ustrMesh.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-24
 *  \modified 2024-02-01
 *  \brief unstructure mesh class
 */

#ifndef MESH_USTR_USTRMESH_HPP
#define MESH_USTR_USTRMESH_HPP
#include "connectivity.hpp"
#include "edgeTable.hpp"
#include "faceTable.hpp"
#include "graphTopology.hpp"
#include "groupCommunication.hpp"
#include "mesh.hpp"
#include "ustrXdmfIO.hpp"
#include "utilityCommunicator.hpp"

namespace HSF {
class Region;

bool isOwner(int myId, int neighborId, int nProcs);

typedef enum {
  ParMETIS = 0,
  Zoltan_GRAPH = 1,
  Zoltan_HYPERGRAPH = 2,
  Zoltan_RCB = 3,
  Zoltan_RIB = 4,
  Zoltan_HSFC = 5,
  Zoltan_OCTPART = 6,
} meshPartitionMethod;

class USTRMesh : public Mesh {
 private:
  friend class Region;
  friend class CollisionManager;
  /*! mesh IO tool for unstructure mesh */
  // 用于非结构化网格的输入输出操作
  IO::UstrXdmfTool xdmfIO_;
  /*! helper class, used when constructing faces */
  // 构造面时的辅助类
  FaceTable* facesTbl_;
  /*! helper class, used when constructing edges */
  // 构造边时的辅助类
  EdgeTable* edgesTbl_;
  /*! node index mapping from local to global */
  // 本地到全局的节点索引映射
  std::vector<llabel> nodeIdLocal2Global_;
  /*! partition method */
  // 网格划分方法
  meshPartitionMethod partitionMethod_;
  struct FaceInfo {
    label elem1No;
    llabel elem2No;
    int localInd1, localInd2;
    int orient1, orient2;
  };
  // 存储面的信息
  std::vector<FaceInfo> facesInfo_;

  /// <owner/neighbor processID ,Shared to local index mapping >
  /// first build, used mainly for search
  /// 共享实体的映射，共享面、边、节点的映射，用于搜索
  map<int, std::set<label>> sFaceId_;  /// shared faces
  map<int, std::set<label>> sEdgeId_;  /// shared edges
  map<int, std::set<label>> sNodeId_;  /// only vertices
  /*! judge if a face is shared face, non-ghost only
   * return owner procId if true, return -1 if false */
  int isSharedFace(label id) const;
  /*! judge if a edge is shared edge*/
  int isSharedEdge(label id) const;
  /*! judge if a node is shared node*/
  int isSharedNode(label id) const;
  /*! -1 -> interior face
   *  -2 -> physical boundary
   *  >=0 -> shared face, id is face owner procId
   */
  // 存储面的所有者信息
  std::vector<int> faceOwner_;
  std::vector<int> edgeOwner_; /*!< >=0 just mean it's a shared entity */
  std::vector<int> nodeOwner_; /*!< >=0 just mean it's a shared entity */

  /// then build
  /// InterfaceInfo结构体用于存储与共享和ghost单元相关的信息，并包含一些列构造和清理这些信息的方法
  struct InterfaceInfo {
    std::vector<label> sharedFaces_;  /// non-ghost part
    std::vector<label> sharedEdges_;  /// non-ghost part
    std::vector<label> sharedNodes_;  /// non-ghost part
    std::vector<label> ghostCells_;   /// ghost
    std::vector<label> ghostFaces_;   /// ghost part
    std::vector<label> ghostEdges_;   /// ghost part
    std::vector<label> ghostNodes_;   /// ghost part

    std::vector<int> sFaceGroup_;
    std::vector<int> sEdgeGroup_;
    std::vector<int> sNodeGroup_;  /// only vertices
    std::vector<int> gCellGroup_;
    std::vector<int> gFaceGroup_;
    std::vector<int> gEdgeGroup_;
    std::vector<int> gNodeGroup_;  /// only vertices

    ProcGroup groups_;
    GroupCommunicator* sfaceGc_;
    GroupCommunicator* sedgeGc_;
    GroupCommunicator* snodeGc_;
    GroupCommunicator* sdofGc_; /*!< not implemented yet */

    GroupCommunicator* gcellGc_;
    GroupCommunicator* gfaceGc_;
    GroupCommunicator* gedgeGc_;
    GroupCommunicator* gnodeGc_;
    GroupCommunicator* gdofGc_; /*!< not implemented yet */
    bool hasGhost_;

    std::vector<llabel> ghostElemGId_; /*!< my ghost element global id */
    map<int, std::set<llabel>>
        neighborProc2GhostElemId_; /*!< my neighbors' ghostCell global id*/
    InterfaceInfo();
    InterfaceInfo(Communicator* comm);
    ~InterfaceInfo();
    void buildSharedFaceCommunicator();
    void buildSharedEdgeCommunicator();
    void buildSharedNodeCommunicator();
    void buildSharedDofGroupCommunicator();
    void buildGhostCellCommunicator();
    void buildGhostFaceCommunicator();
    void buildGhostEdgeCommunicator();
    void buildGhostNodeCommunicator();
    void buildGhostDofGroupCommunicator();
    /*! clear to rebuild */
    void clearGhostEntities();
  } interface_;

  // 返回xdmfIO_对象的引用，用于进行输入和输出操作
  IO::UstrXdmfTool& getIOTool() { return xdmfIO_; }

  /*! * * * * * * * * * * * * * * * Topology  * * * * * * * * * * * * * */
  /*! elem2node data migration after mesh partition */
  // 在网格划分后分配元素到处理器
  void distributeElemToProcs(const std::vector<label>& partition);

  /*! 3D:  */
  /*! calculate cell2face topo from cell2node topo,
   *  local face index, ignore process boundary */
  // 从cell2node的拓扑计算cell2face的拓扑
  void calcCell2Face();
  /*! calculate face2cell and face2node table from cell2face topo */
  // 从cell2face的拓扑计算face2cell和face2node表
  void calcFace2Cell();
  /*! communicate boundary face2cell and boundary face2node, may cost a lot */
  // 
  void distributeBdrFaceTopo();
  /*!  calculate cell2cell topo from cell2xx and xx2cell topo */
  void calcCell2Cell(SetType type = SetType::FACE);
  /*!  calculate edge2cell topo from face2cell topo */
  void calcEdge2Cell();
  /*!  calculate node2cell topo from edge2cell */
  void calcNode2Cell();
  /*! buid face2node topo from FaceTable */
  void calcFace2Node();

  /*! 2D: */
  /*! calculate cell2edge/face2edge topo from face2node topo,
   *  local edge index, ignore process boundary */
  void calcElem2Edge();
  /*! when 2D, the same as calcElem2Edge */
  void calcFace2Edge();
  /*!  calculate  edge2face and edge2node table from face2edge topo */
  void calcEdge2Face();
  /*! communicate boundary edge2face and boundary edge2node, may cost a lot */
  void distributeBdrEdgeTopo();
  /*! calculate node2face from edge2face, 2D */
  void calcNode2Face();
  /*!  calculate face2face topo from face2edge and edge2face topo */
  void calcFace2Face(SetType type = SetType::EDGE);
  /*! buid edge2node topo from EdgeTable */
  void calcEdge2Node();

  /*! 1D: */
  /*! build nodes and calculate node2edge topo from edge2node topo,
   *  global edge index, ignore process boundary */
  void calcNode2Edge();
  /*! communicate boundary  node2edge and boundary nodeIndex */
  void distributeBdrNodeTopo();
  /*!  calculate edge2edge topo from edge2node and node2edge topo */
  void calcEdge2Edge();
  /*!  calculate node2node topo from edge2node topo */
  void calcNode2Node();

  /*! Used in GenerateFaces() */
  void addPointFaceElement(label lf, label gf, label el);
  void addSegmentFaceElement(label lf, label gf, label el, label v0, label v1);
  void addTriangleFaceElement(label lf, label gf, label el, label v0, label v1,
                              label v2);
  void addQuadFaceElement(label lf, label gf, label el, label v0, label v1,
                          label v2, label v3);

 public:
  /*! 有些函数为了调试暂时作为public放着 */
  /*! ghost layer create */
  int createGhostLayer(int bridge_dim, int num_layers = 1);
  /*! 内部接口，协调ghostLayer生成后的各数据 */
  void updateAfterCreateGhost(int bridge_dim);
  /*! 内部接口，创建ghostFace/Edge/Node，createGhostLayer中调用 */
  void buildGhostEntities(const std::vector<int>& group,
                          const std::vector<label>& ids);
  /*! 内部接口，增加ghostFace/Edge/Node，影像区扩展时调用
   *  必须知道邻居的拓扑影像区*/
  void appendGhostEntities(
      const std::vector<int>& group, const std::vector<label>& ids,
      const map<int, std::set<llabel>>& neighborProc2TopoGhostElemId);
  //  template <typename T>
  //  void updateCellData(T* data, int dataLength = 1, bool rebuild = false);

  GraphTopology* getAdjacency(SetType from, SetType to);
  GraphTopology* getAdjacency(int from, int to);
  GraphTopology* getElem2Elem(SetType type = SetType::FACE);

  GraphTopology* getCell2Cell(SetType type = SetType::FACE);
  GraphTopology* getCell2Face();
  GraphTopology* getCell2Edge();

  GraphTopology* getFace2Cell();
  GraphTopology* getFace2Face(SetType type = SetType::EDGE);
  GraphTopology* getFace2Edge();

  GraphTopology* getEdge2Cell();
  GraphTopology* getEdge2Face();
  GraphTopology* getEdge2Edge();

  GraphTopology* getNode2Cell();
  GraphTopology* getNode2Face();
  GraphTopology* getNode2Edge();

  GraphTopology* getCell2Node();
  GraphTopology* getFace2Node();
  GraphTopology* getEdge2Node();
  GraphTopology* getNode2Node();

 public:
  /*! construct necessary geom, topo,boundary and processInfo after partition */
  void construct();
  /*! generate boundary subset(node and face(3D)/edge(2D)), set_id=-1 */
  void buildBoundary();
  /*! build sFaceId(3D) or sEdgeId(2D) */
  void buildSharedFaces();
  /*! build sEdgeId(3D)  */
  void buildSharedEdges(std::vector<llabel>* e2c = NULL);
  /*! build sVertId (3/2/1D) */
  void buildSharedNodes(std::vector<llabel>* n2c = NULL);
  /*! change local node index to global node index for a xx2node Topology*/
  void restoreNodeGlobalIndex(GraphTopology* e2n, bool rebuildMapping = false);
  /*! change global node index to local node index for a xx2node Topology*/
  void restoreNodeLocalIndex(GraphTopology* e2n, bool rebuildMapping = false);
  /*! change local elem index to global elem index for a xx2elem Topology*/
  void restoreElemGlobalIndex(GraphTopology* x2e, bool rebuildMapping = false);
  /*! change global elem index to local elem index for a xx2elem Topology*/
  void restoreElemLocalIndex(GraphTopology* x2e, bool rebuildMapping = false);
  /*! * * * * * * * * * * * * * * * utils * * * * * * * * * * * * * */
  /*! tool to change node id from global to local */
  std::unordered_map<llabel, label>& nodeMapG2L(bool rebuild = false) {
    buildNodeIdMapping(rebuild);
    /*! node index mapping from global to local */
    static std::unordered_map<llabel, label> nodeIdGlobal2Local_;
    if (nodeIdGlobal2Local_.empty() || !rebuild) {
      nodeIdGlobal2Local_.clear();
      for (std::size_t i = 0; i < nodeIdLocal2Global_.size(); i++)
        nodeIdGlobal2Local_[nodeIdLocal2Global_[i]] = i;
    }
    return nodeIdGlobal2Local_;
  }
  /*! node index mapping from local to global */
  std::vector<llabel>& nodeMapL2G(bool rebuild = false) {
    buildNodeIdMapping(rebuild);
    return nodeIdLocal2Global_;
  }

  /*! elem index mapping from local to global */
  std::vector<llabel>& elemMapL2G(bool rebuild = false);
  /*! elem index mapping from global to local */
  std::unordered_map<llabel, label>& elemMapG2L(bool rebuild = false);

  /*! judge if ghostLayer has been built */
  bool hasGhost() const { return interface_.hasGhost_; }

  /*! return mesh interface info */
  const InterfaceInfo* getInterface() const { return &interface_; }

  /*! calculate new perm after build <owner/neighbor processID ,Shared to
   * local index> mapping >, put shared entities to end, return real num */
  static label calcPerm(Communicator* comm, label size,
                        const map<int, std::vector<label>>& procMap,
                        std::vector<label>& perm);

  /*! set partition method */
  void setPartitionMethod(meshPartitionMethod method) {
    partitionMethod_ = method;
  }

 private:
  /// Returns the orientation of "test" relative to "base"
  static int getTriOrientation(const int* base, const int* test);
  /// Returns the orientation of "test" relative to "base"
  static int getQuadOrientation(const int* base, const int* test);

 public:
  /*! 默认构造 */
  USTRMesh(Communicator* comm = NULL);
  /*! 析构，空间回收*/
  virtual ~USTRMesh();
  /*! Move constructor, useful for using a Mesh as a function return value */
  USTRMesh(USTRMesh&& mesh) = default;
  /*! Move assignment operstor. */
  USTRMesh& operator=(USTRMesh&& mesh) = default;
  /*! Explicitly delete the copy assignment operator. */
  USTRMesh& operator=(const USTRMesh& mesh) = delete;

  /*! * * * * * * * * * * * * * * Partition * * * * * * * * * * * * */
  /*!< use ParMETIS  */
  void meshPartition(std::vector<label>& parts,
                     meshPartitionMethod type = ParMETIS);

  /*! * * * * * * * * * * * * *  Geometry Properties * * * * * * * * * * * */

  /*! build elements from elem2node Topology */
  virtual void buildElements();
  /*! update cell2node
   * (global index replaced by local index)
   * don't build nodes before ghost layer constructed
   */
  virtual void readNodeCoords();

  /*! build nodeId local to global map */
  void buildNodeIdMapping(bool rebuild = false);

  /*! build faces_/edges_, generate facesInfo from cell2face and face2node */
  void generateFaces();

  inline label getNumFaces() const {
    switch (dim_) {
      case 1:
        return nodes_.getNum();
      case 2:
        return edgesTbl_->numberOfElements();
      case 3:
        return facesTbl_->numberOfElements();
    }
    return 0;
  }

  /*! calculate before use(calclate by geometry) */
  virtual void updateNodeCoord() override;
  virtual void calcEdgeLength() override;
  virtual void calcEdgeCenter() override;
  virtual void calcEdgeNorm() override;
  virtual void calcFaceArea() override;
  virtual void calcFaceCenter() override;
  virtual void calcFaceNorm() override;
  virtual void calcCellVol() override;
  virtual void calcCellCenter() override;

  /*! delete constructed topology and Geometries */
  void clear();

  /*! * * * * * * * * * * * * * * * IO  * * * * * * * * * * * * * */
  /*! read unpartitioned mesh file */
  void initFromXdmf(const Word fileName, bool doPartition = true,
                    int bridge_dim = -1, int num_layers = 1);
  /*! export mesh file (not be divided by processors) */
  void exportToXdmf(const Word fileName, bool writeProcessInfo = false,
                    bool mixedTopo = false);
  /*! read partitioned mesh file, must run with particular process num */
  virtual void readFromXdmf(const Word fileName);
  /*! write partitioned mesh file (processor independent)*/
  virtual void writeToXdmf(const Word fileName, bool writeGhostLayer = false);
  /*! write subset info after write internalMesh */
  void writeSubSet(bool seqWrite = false);
  /*! read subset info after read internalMesh and construct */
  void readSubSet(bool seqRead = false);
};

void generalUSTRMeshPartition(USTRMesh* mesh, std::vector<label>& parts,
                              meshPartitionMethod type = ParMETIS);

} /* End namespace HSF */

#endif /* end of include guard: MESH_USTR_USTRMESH_HPP */
