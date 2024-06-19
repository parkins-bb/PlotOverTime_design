#ifndef MESH_GEOMETRY_AABBTREE_HPP
#define MESH_GEOMETRY_AABBTREE_HPP
/*! \file AABBTree.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2024-04-10
 *  \brief Axis-Aligned Bounding Box tree
 */
#include "boundBox.hpp"
#include "mpi.h"
namespace HSF {

/// Axis-Aligned bounding box binary tree. It is used to find entities
/// in a collection (often a mesh::Mesh).
class AABBTree {
 public:
  /*! build from bounding boxes */
  AABBTree(std::vector<std::pair<std::array<scalar, 6>, label>>& boxes,
           scalar padding = 0);

  /*! build from points cloud */
  AABBTree(std::vector<std::pair<std::array<scalar, 3>, label>>& points);

  /// Move constructor
  AABBTree(AABBTree&& tree) = default;

  /// Copy constructor
  AABBTree(const AABBTree& tree) = delete;

  /// Move assignment
  AABBTree& operator=(AABBTree&& other) = default;

  /// Copy assignment
  AABBTree& operator=(const AABBTree& other) = default;

  /// Destructor
  ~AABBTree() = default;

  /// return boundBox by id
  const BoundBox* getBBox(label node) const;

  /// Compute a global bounding tree (collective on comm)
  /// This can be used to find which process a point might have a
  /// collision with.
  /// @param[in] comm MPI Communicator for collective communication
  /// @return AABBTree where each node represents a process
  AABBTree *createGlobalTree(MPI_Comm comm);

  /// Return number of bounding boxes
  label numBBoxes() const { return bboxes_.size() / 2; }

  /// Print out for debugging
  std::string debugPrint() const;

  /// Get bounding box child nodes.
  ///
  /// @param[in] node The bounding box node index
  /// @return The indices of the two child nodes. If @p node is a leaf
  /// nodes, then the values in the returned array are equal and
  /// correspond to the index of the entity that the leaf node bounds,
  /// e.g. the index of the cell that it bounds.
  std::array<label, 2> bbox(std::size_t node) const;

  /*! print boxes to vtk file, only print leaf if onlyLeaf is true */
  void printBoxes(std::string fileName, int dim, bool onlyLeaf = true);

 private:
  // Constructor
  AABBTree(std::vector<label>&& bboxes, std::vector<scalar>&& bbox_coords)
      : bboxes_(bboxes), bbox_coordinates_(bbox_coords) {}

  // Print out recursively, for debugging
  void tree_print(std::stringstream& s, label i) const;

  // List of bounding boxes (parent-child-entity relations)
  std::vector<label> bboxes_;

  // List of bounding box coordinates
  std::vector<scalar> bbox_coordinates_;
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_GEOMETRY_AABBTREE_HPP */
