#ifndef MESH_USTR_DOTK2XDMF_HPP
#define MESH_USTR_DOTK2XDMF_HPP

/*! \file dotK2xdmf.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2024-02-02
 *  \brief fixed format LS-Dyna .K file  to xdmf format
 */

#include "xdmfTool.hpp"
namespace HSF {
namespace IO {

/*! read .K mesh file(one processor)
 * \param points node coordinate (XYZXYZXYZ)
 * \param cell_data grid topology,cols(CSR format)
 * \param cell_offsets grid topology,compressed rows(CSR format)
 * \param cell_types xdmf cell types
 * \param cell_attributes part id
 * \param node_lists node id groups
 * \param node_list_id id for each node group
 * \param elem_data set_segment topology(elem2node) for each set, cols(CSR)
 * \param elem_offsets set_segment topology(elem2node) for each set, cprRow(CSR)
 * \param elem_types xdmf type array for each set
 * \param set_segment_id id for each elem set
 * \param ordered whether the data is ordered
 */
void readKMesh(const std::string fileName, std::vector<scalar> &points,
               std::vector<llabel> &cell_data, std::vector<label> &cell_offsets,
               std::vector<int> &cell_types, std::vector<int> &cell_attributes,
               std::vector<std::vector<llabel>> &node_lists, std::vector<int> &node_list_id,
               std::vector<std::vector<llabel>> &elem_data,
               std::vector<std::vector<label>> &elem_offsets, std::vector<std::vector<int>> &elem_types,
               std::vector<int> &set_segment_id, bool ordered = true);

/*! one processor */
std::string dotK2xdmf(const std::string fileName, Encoding encode,
                      std::string outputPath = "");
} /* End namespace IO */
} /* End namespace HSF */

#endif /* end of include guard: MESH_USTR_DOTK2XDMF_HPP */
