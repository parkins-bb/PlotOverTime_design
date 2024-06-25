#ifndef MESH_USTR_EDGETABLE_HPP
#define MESH_USTR_EDGETABLE_HPP
/*! \file edgeTable.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2013-05-18
 *  \brief helper class to deal with edge generation after reading face2node
 */

#include "utilities.h"

namespace HSF {

struct array2_hash {
  std::size_t operator()(const std::array<llabel, 2> arr) const {
    return 0.5 * (arr[0] + arr[1]) * (arr[0] + arr[1] + 1) + arr[1];
  }
};

class EdgeTable {
 private:
  /*! size_: point number */
  label nelem_;
  unordered_map<std::array<llabel, 2>, label, array2_hash> edgesMap_;
  typedef unordered_map<std::array<llabel, 2>, label, array2_hash>::iterator
      Item;
  typedef unordered_map<std::array<llabel, 2>, label,
                        array2_hash>::const_iterator ConstItem;
  std::vector<std::vector<llabel> > edgesNode_;
  bool buildEdgesNode_;

 public:
  /*! Construct the table with a total of 'nr' rows. */
  explicit EdgeTable(bool buildEdgesNode = true);
  ~EdgeTable();

  /*! Check to see if this entry is in the table and add it to the table
      if it is not there. Returns the number assigned to the table entry. */
  label push(llabel r, llabel c);

  /*! add entry with assigned number to the table, no check */
  void pushWithNum(label num, llabel r, llabel c);

  /*! Return the number assigned to the table entry. Abort if it's not there. */
  label operator()(llabel r, llabel c) const;

  /*! Return the number of elements added to the table. */
  label numberOfElements() { return nelem_; }

  /*! Return the node indexes of edge i */
  const std::vector<llabel>& operator()(label i) const { return edgesNode_[i]; }
  const std::vector<std::vector<llabel> >& edgeNodes() const {
    return edgesNode_;
  }
  void clearEdgeNodes() {
    std::vector<std::vector<llabel> >().swap(edgesNode_);
  }
  void clearEdgeMap() { edgesMap_.clear(); }
  void clear() {
    clearEdgeMap();
    clearEdgeNodes();
  }
  /*! global id to local id */
  void restoreNodeLocalIndex(const std::unordered_map<llabel, label>& g2lMap);

  void resizeTable(label num); /*!< resize edge table to num */
};

} /* End namespace HSF */
#endif /* end of include guard: MESH_USTR_EDGETABLE_HPP */
