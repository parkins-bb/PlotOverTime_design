/*! \file groupCommunication.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-25
 *  \modified 2024-01-11
 *  \brief shared entities groups by processors
 */

#ifndef MESH_USTR_GROUPCOMMUNICATION_HPP
#define MESH_USTR_GROUPCOMMUNICATION_HPP

#include "graphTopology.hpp"
#include "utilities.h"

namespace HSF {
class ProcGroup {
 private:
  Communicator *comm_;

  std::vector<std::vector<int>> g2lprocId_; /*!< group id to processor id */
  std::vector<int>
      masterlprocId_; /*!< master local processor id for each group */
  std::vector<int>
      lproc2procId_;             /*!< local processor id to real processor id */
  std::vector<int> g2mastergId_; /*!< this group's id in master processor */

  int num_;
  /*! useless after construction   */
  /*! procIds+masterId => groupId */
  map<std::pair<set<int>, int>, int> procSet2gId_;
  std::vector<std::set<int>> groups_;

 public:
  /*! constructor */
  ProcGroup();
  ProcGroup(Communicator *comm);
  ProcGroup(Communicator *comm, std::vector<std::set<int>> &groups);
  ~ProcGroup();

  /*! init ProcGroup(add a group with one processor(myId)) */
  void init();
  /*! add a new group, won't add if both group set and masterProcId are the same
   * with existed group
   * /return  group id
   */
  int addAGroup(const std::set<int> &group, int masterProcId);
  /*! add a new group, automatically choose master(simplest method), won't add
   * if the group already exists(won't check master)
   * /return  group id
   */
  int addAGroup(const std::set<int> &group);
  int addAGroup(const std::vector<int> &group, int masterProcId);
  int addAGroup(const std::vector<int> &group);
  /*! query the set belongs to which group */
  int whichGroup(const std::set<int> &group, int masterProcId);
  int whichGroup(const std::set<int> &group);

  /*! simplest own choose method*/
  int simplestPickOwner(const std::set<int> &group);

  /*!  finish the construction of Procgroup*/
  void create(const std::vector<std::set<int>> &groups);
  void create();

  /*! set and return communicatior */
  void setComm(Communicator *comm) { comm_ = comm; }
  Communicator *getComm() { return comm_; }
  Communicator *getComm() const { return comm_; }
  /*! Return the number of groups.*/
  int nGroups() const { return num_; }
  /*! Return the number of neighbors including the local processor.*/
  int nNeighbors() const { return lproc2procId_.size(); }
  /*! Return the MPI rank of neighbor 'i'.*/
  int getNeighborRank(int i) const { return lproc2procId_[i]; }
  /*! Return true if I am master for group 'g'.*/
  bool isMaster(int g) const { return (masterlprocId_[g] == 0); }
  /*! Return the neighbor index of the group master for a given group.
      Neighbor 0 is the local processor. */
  int getGroupMaster(int g) const { return masterlprocId_[g]; }
  /*! Return the rank of the group master for group 'g'.*/
  int getGroupMasterRank(int g) const {
    return lproc2procId_[masterlprocId_[g]];
  }
  /*! Return the group number in the master for group 'g'.*/
  int getGroupMasterGroup(int g) const { return g2mastergId_[g]; }
  /*! Get the number of processors in a group*/
  int getGroupSize(int g) const { return g2lprocId_[g].size(); }
  /*! @brief Return a pointer to a list of neighbors for a given group.
      Neighbor 0 is the local processor */
  const int *getGroup(int g) const { return g2lprocId_[g].data(); }
  const std::set<int> &getGroupRankSet(int g) const { return groups_[g]; }
};

class GroupCommunicator {
 public:
  enum MODE { byGroup, byNeighbor };

 protected:
  const ProcGroup &gtopo;
  MODE mode;
  GraphTopology *group_ldof;
  GraphTopology *group_ltdof;
  label group_buf_size;
  mutable std::vector<std::vector<char>> group_buf;
  mutable std::vector<MPI_Request *> requests;
  mutable std::vector<int *> request_marker;
  int request_counter_;
  // comm_lock: 0 - no lock, 1 - locked for Bcast, 2 - locked for Reduce
  mutable int comm_lock;
  mutable std::vector<int> num_requests;
  mutable int begin_message; /*!< 处理多条发送消息 */
  mutable int end_message;   /*!< 处理多条接收消息 */
  label *buf_offsets;  // size = max(number of groups, number of neighbors)
  GraphTopology nbr_send_groups, nbr_recv_groups;  // nbr 0 = me
  bool own_ldof_, own_ltdof_;

 public:
  /*! The object group_ldof must be initialized before it can be used to perform
   * any operations. To initialize the object, either call create() or
   * initialize groupLDofTable() and then call finalize().
   */
  GroupCommunicator(const ProcGroup &gt, MODE m = byNeighbor);

  /*! @brief Initialize the communicator from a local-dof to group map.
      finalize() is called internally. */
  void create(const std::vector<int> &ldof_group);

  /*! @brief Initialize the communicator from a shared local-dof to group map.
      finalize() is called internally. */
  void create(const std::vector<int> &sldof_group,
              const std::vector<label> &sharedId);

  /** @brief Fill-in the returned Table reference to initialize the
      GroupCommunicator then call finalize(). */
  GraphTopology *getGroupLDofTable() { return group_ldof; }
  GraphTopology *getGroupLTDofTable() { return group_ltdof; }
  void setGroupLDofTable(GraphTopology *tbl) {
    if (own_ldof_) delete group_ldof;
    group_ldof = tbl;
    own_ldof_ = false;
  }
  void setGroupLTDofTable(GraphTopology *tbl) {
    if (own_ltdof_) delete group_ltdof;
    group_ltdof = tbl;
    own_ltdof_ = false;
  }

  /// Allocate internal buffers after the groupLDofTable is defined
  void finalize();

  /// Initialize the internal group_ltdof Table.
  /** This method must be called before performing operations that use local
      data layout 2, see copyGroupToBuffer() for layout descriptions. */
  void buildLTDofTable(const std::vector<label> &ldof_ltdof);

  /// Get a reference to the associated GroupTopology object
  const ProcGroup &GetGroupTopology() { return gtopo; }

  /// Get a const reference to the associated GroupTopology object
  const ProcGroup &GetGroupTopology() const { return gtopo; }

  /** @brief Data structure on which we define reduce operations.
      The data is associated with (and the operation is performed on) one
      group at a time. */
  template <class T>
  struct OpData {
    label nldofs, nb;
    int length;
    const llabel *ldofs;
    T *ldata, *buf;
  };

  /** @brief Copy the entries corresponding to the group @a group from the
      local array @a ldata to the buffer @a buf. */
  /** The @a layout of the local array can be:
      - 0 - @a ldata is an array on all ldofs: copied indices:
        `{ J[j] : I[group] <= j < I[group+1] }` where `I,J=group_ldof.{I,J}`
      - 1 - @a ldata is an array on the shared ldofs: copied indices:
        `{ j : I[group] <= j < I[group+1] }` where `I,J=group_ldof.{I,J}`
      - 2 - @a ldata is an array on the true ldofs, ltdofs: copied indices:
        `{ J[j] : I[group] <= j < I[group+1] }` where `I,J=group_ltdof.{I,J}`.
      @returns The pointer @a buf plus the number of elements in the group. */
  template <class T>
  T *copyGroupToBuffer(const T *ldata, T *buf, int group, int layout = 0,
                       int length = 1) const;

  /** @brief Copy the entries corresponding to the group @a group from the
      buffer @a buf to the local array @a ldata. */
  /** For a description of @a layout, see CopyGroupToBuffer().
      @returns The pointer @a buf plus the number of elements in the group. */
  template <class T>
  const T *copyGroupFromBuffer(const T *buf, T *ldata, int group,
                               int layout = 0, int length = 1) const;

  /** @brief Perform the reduction operation @a Op to the entries of group
      @a group using the values from the buffer @a buf and the values from the
      local array @a ldata, saving the result in the latter. */
  /** For a description of @a layout, see CopyGroupToBuffer().
      @returns The pointer @a buf plus the number of elements in the group. */
  template <class T>
  const T *reduceGroupFromBuffer(const T *buf, T *ldata, int group, int layout,
                                 int length, void (*Op)(OpData<T>)) const;

  /// Begin a broadcast within each group where the master is the root.
  /** For a description of @a layout, see CopyGroupToBuffer(). */
  template <class T>
  void updateBegin(T *ldata, int layout = 0, int length = 1) const;

  /** @brief Finalize a broadcast started with BcastBegin().

      The output data @a layout can be:
      - 0 - @a ldata is an array on all ldofs; the input layout should be
            either 0 or 2
      - 1 - @a ldata is the same array as given to BcastBegin(); the input
            layout should be 1.
      For more details about @a layout, see CopyGroupToBuffer(). */
  template <class T>
  void updateEnd(T *ldata, int layout = 0, int length = 1) const;

  /** @brief Broadcast within each group where the master is the root.
      The data @a layout can be either 0 or 1.

      For a description of @a layout, see CopyGroupToBuffer(). */
  template <class T>
  void update(T *ldata, int layout = 0, int length = 1) const {
    updateBegin(ldata, layout, length);
    updateEnd(ldata, layout, length);
  }

  /// Broadcast within each group where the master is the root.
  template <class T>
  void update(std::vector<T> &ldata) const {
    update<T>((T *)ldata);
  }

  /** @brief Begin reduction operation within each group where the master is
      the root. */
  /** The input data layout is an array on all ldofs, i.e. layout 0, see
      CopyGroupToBuffer().

      The reduce operation will be specified when calling ReduceEnd(). This
      method is instantiated for int and double. */
  template <class T>
  void reduceBegin(T *ldata, int layout = 0, int length = 1) const;

  /** @brief Finalize reduction operation started with ReduceBegin().

      The output data @a layout can be either 0 or 2, see CopyGroupToBuffer().

      The reduce operation is given by the third argument (see below for list
      of the supported operations.) This method is instantiated for int and
      double.

      @note If the output data layout is 2, then the data from the @a ldata
      array passed to this call is used in the reduction operation, instead of
      the data from the @a ldata array passed to ReduceBegin(). Therefore, the
      data for master-groups has to be identical in both arrays.
  */
  template <class T>
  void reduceEnd(T *ldata, int layout, int length, void (*Op)(OpData<T>)) const;

  /** @brief Reduce within each group where the master is the root.

      The reduce operation is given by the second argument (see below for list
      of the supported operations.) */
  template <class T>
  void reduce(T *ldata, void (*Op)(OpData<T>), int layout = 0,
              int length = 1) const {
    reduceBegin(ldata, layout, length);
    reduceEnd(ldata, layout, length, Op);
  }

  /// Reduce within each group where the master is the root.
  template <class T>
  void reduce(std::vector<T> &ldata, void (*Op)(OpData<T>)) const {
    reduce<T>((T *)ldata, Op);
  }

  /// Reduce operation Sum, instantiated for int and double
  template <class T>
  static void sum(OpData<T>);
  /// Reduce operation Min, instantiated for int and double
  template <class T>
  static void min(OpData<T>);
  /// Reduce operation Max, instantiated for int and double
  template <class T>
  static void max(OpData<T>);
  /// Reduce operation bitwise OR, instantiated for int only
  template <class T>
  static void bitOR(OpData<T>);

  /** @brief Destroy a GroupCommunicator object, deallocating internal data
      structures and buffers. */
  ~GroupCommunicator();
};

}  // namespace HSF

#endif /* end of include guard: MESH_USTR_GROUPCOMMUNICATION_HPP */
