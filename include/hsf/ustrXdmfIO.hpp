#ifndef MESH_USTR_USTRXDMFIO_HPP
#define MESH_USTR_USTRXDMFIO_HPP
/*! \file ustrXdmfIO.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-04-25
 *  \brief xdmf mesh IO tool for unstructure mesh
 */
#include "connectivity.hpp"
#include "xdmfTool.hpp"

namespace HSF {

namespace IO {

class UstrXdmfTool : public XdmfTool {
 private:
  int cellTypeNum_;            /*!< global cell types number */
  std::set<int> cellTypesSet_; /*!< global cell types array */

  /*! parallel,  */
  void buildCellTypes(const std::vector<int> &cell_types);

 public:
  UstrXdmfTool() : XdmfTool(), cellTypeNum_(-1) {}
  UstrXdmfTool(Communicator *comm, const std::string &filename = "",
               const std::string file_mode = "a",
               const Encoding encoding = defaultEncoding_)
      : XdmfTool(comm, filename, file_mode, encoding), cellTypeNum_(-1) {}

  virtual ~UstrXdmfTool() {}

  /*! parallel read internalMesh cell2node topology */
  llabel readTopology(std::vector<llabel> &cell_data,
                      std::vector<label> &cell_offsets,
                      std::vector<int> &cellTypes, const std::string name);
  /*! serial read internalMesh cell2node topology */
  llabel readTopologySeq(std::vector<llabel> &cell_data,
                         std::vector<label> &cell_offsets,
                         std::vector<int> &cellTypes, const std::string name);

  /*! parallel write topology */
  void writeTopology(const std::vector<llabel> &cell_data,
                     const std::vector<label> &cell_offsets,
                     const std::vector<int> &cell_types, const std::string name,
                     bool writeMixed = false);
  /*! serial write topology */
  void writeTopologySeq(const std::vector<llabel> &cell_data,
                        const std::vector<label> &cell_offsets,
                        const std::vector<int> &cell_types,
                        const std::string name);
  /*! boundary IO */
  void writeSubset(const std::vector<llabel> &cell_data,
                   const std::vector<label> &cell_offsets,
                   const std::vector<int> &cell_types, const int attributeID,
                   const SetType setType, const std::string gridName,
                   const std::string bcName);
  void writeSubset(const std::vector<llabel> &cell_data, const int attributeID,
                   const SetType setType, const std::string gridName,
                   const std::string bcName); /*!< global index only, no topo */
  /*! 除cell/node类型为index，其余均包含topology */
  void readSubset(std::vector<std::vector<llabel>> &cell_data,
                  std::vector<std::vector<label>> &cell_offsets,
                  std::vector<std::vector<int>> &cell_types,
                  std::vector<int> &attributeID, std::vector<SetType> &setTypes,
                  const std::string gridName, std::vector<std::string> &bcName);

  void writeBoundaryItemSeq(const std::vector<llabel> &cell_data,
                            const std::vector<label> &cell_offsets,
                            const std::vector<int> &cell_types,
                            const int attributeID, const std::string gridName,
                            const std::string bcName);
  void readBoundaryItemSeq(std::vector<std::vector<llabel>> &cell_data,
                           std::vector<std::vector<label>> &cell_offsets,
                           std::vector<std::vector<int>> &cell_types,
                           std::vector<int> &attributeID,
                           const std::string gridName,
                           std::vector<std::string> &bcName);

  /*! (overwrite) read attribute by cell types(maybe) */
  template <typename T>
  void readCellAttributeByTypes(std::vector<T> &fieldData,
                                const std::vector<int> &cell_types,
                                const std::string gridName,
                                const std::string fieldName) const;
  /*! (overwrite) write attribute by cell types(or not) */
  template <typename T>
  void writeCellAttributeByTypes(const std::vector<T> &fieldData,
                                 const std::vector<int> &cell_types,
                                 label realNum, const std::string gridName,
                                 const std::string fieldName,
                                 bool hasMixedTopo = false) const;
  template <typename T>
  void writeNodeAttribute(const std::vector<T> &fieldData, label realNum,
                          label entityLength, const std::vector<llabel> &index,
                          llabel totalNodeNum, const std::string gridName,
                          const std::string fieldName) const;

 private:
  /* data */
};

} /* End namespace IO */
} /* End namespace HSF */

#endif /* end of include guard: MESH_USTR_USTRXDMFIO_HPP */
