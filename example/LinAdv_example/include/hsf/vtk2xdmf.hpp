#ifndef MESH_USTR_VTK2XDMF_HPP
#define MESH_USTR_VTK2XDMF_HPP

/*! \file vtk2xdmf.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2024-02-02
 *  \brief vtk file format to xdmf format
 */

#include "xdmfTool.hpp"
namespace HSF {
namespace IO {

/*! read simple legacy vtk format(one processor)
 * \param points node coordinate
 * \param cell_data grid topology,cols(CSR format)
 * \param cell_offsets grid topology,compressed rows(CSR format)
 * \param cell_types vtk cell types
 * \param cell_attributes part id
 */
void readVTKMesh(const std::string fileName, std::vector<scalar> &points,
                 std::vector<llabel> &cell_data, std::vector<label> &cell_offsets,
                 std::vector<int> &cell_types, std::vector<int> &cell_attributes);

/*! one processor */
void vtk2xdmf(const std::string fileName, Encoding encode);
} /* End namespace IO */
} /* End namespace HSF */

#endif /* end of include guard: MESH_USTR_VTK2XDMF_HPP */
