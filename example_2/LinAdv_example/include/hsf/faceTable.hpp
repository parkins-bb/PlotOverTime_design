#ifndef MESH_USTR_FACETABLE_HPP
#define MESH_USTR_FACETABLE_HPP
/*! \file faceTable.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-05-08
 *  \brief helper class to deal with face generation after reading cell2node
 */

#include "utilities.h"

namespace HSF {

struct array3_hash {
  std::size_t operator()(const std::array<llabel, 3> arr) const {
    llabel a = 0.5 * (arr[0] + arr[1]) * (arr[0] + arr[1] + 1) + arr[1];

    return 0.5 * (a + arr[2]) * (a + arr[2] + 1) + arr[2];
  }
};

class FaceTable {
 private:
  /*! size_: point number */
  label nelem_;
  unordered_map<std::array<llabel, 3>, label, array3_hash> facesMap_;
  typedef unordered_map<std::array<llabel, 3>, label, array3_hash>::iterator
      Item;
  typedef unordered_map<std::array<llabel, 3>, label,
                        array3_hash>::const_iterator ConstItem;
  std::vector<std::vector<llabel> > facesNode_;
  std::vector<int> faceType_; /*!< xdmf type */
  bool buildFacesNode_;
  bool fourth_; /*!< temporal helper variable */

 public:
  /*! Construct the table with a total of 'nr' rows. */
  explicit FaceTable(bool buildFacesNode = true);
  ~FaceTable();

  /*! Check to see if this entry is in the table and add it to the table
      if it is not there. Returns the number assigned to the table entry. */
  label push(llabel r, llabel c, llabel f);

  /*! add entry with assigned number to the table, no check */
  void pushWithNum(label num, llabel r, llabel c, llabel f);

  /*! Return the number assigned to the table entry. Abort if it's not there. */
  label operator()(llabel r, llabel c, llabel f) const;

  /*! Check to see if this entry is in the table and add it to the table
      if it is not there. The entry is addressed by the three smallest values
      of (r,c,f,t). Returns the number assigned to the table entry. */
  label push(llabel r, llabel c, llabel f, llabel t);

  /*! add entry with assigned number to the table, no check */
  void pushWithNum(label num, llabel r, llabel c, llabel f, llabel t);

  /*! Return the number assigned to the table entry. The entry is
      addressed by the three smallest values of (r,c,f,t). Return -1 if it is
      not there. */
  label operator()(llabel r, llabel c, llabel f, llabel t) const;

  /*! Return the number of elements added to the table. */
  label numberOfElements() { return nelem_; }

  /*! Return the node indexes of face i */
  const std::vector<llabel>& operator()(label i) const { return facesNode_[i]; }
  const std::vector<std::vector<llabel> >& faceNodes() const {
    return facesNode_;
  }
  const std::vector<int>& faceType() const { return faceType_; }
  const int faceType(int i) const { return faceType_[i]; }
  /*! delete facesNode_ */
  void clearFaceNodes() {
    std::vector<std::vector<llabel> >().swap(facesNode_);
  }
  /*! delete facesMap_ */
  void clearFaceMap() { facesMap_.clear(); }
  void clear() {
    clearFaceMap();
    clearFaceNodes();
  }

  /*! global id to local id */
  void restoreNodeLocalIndex(const std::unordered_map<llabel, label>& g2lMap);

  void resizeTable(label num); /*!< resize table to num,rebuild facesMap   */
};

} /* End namespace HSF */
#endif /* end of include guard: MESH_USTR_FACETABLE_HPP */
