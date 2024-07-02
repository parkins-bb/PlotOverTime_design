/*! \file xdmfTool.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-25
 *  \modified 2023-12-20
 *  \brief class XdmfTool, help to write and read .xmf mesh file
 */

#ifndef MESH_IO_XDMFTOOL_HPP
#define MESH_IO_XDMFTOOL_HPP
#include <memory>
#include <string>

#include "geometry.hpp"
#include "hdf5Tool.hpp"
#include "tensor.hpp"
#include "utilities.h"

/*! pugixml is a light-weight C++ XML processing library */
namespace pugi {
class xml_node;
class xml_document;
}  // namespace pugi
namespace HSF {

enum class DataLocation : int {
  Node,
  Edge,
  Face,
  Cell,
  Grid,
  Other,
};
std::string toString(DataLocation loc);

class XdmfCellType {
 public:
  enum Type {
    Polyvertex = 1,
    Polyline = 2,
    Polygon = 3,
    Triangle = 4,
    Quadrilateral = 5,
    Tetrahedron = 6,
    Pyramid = 7,
    Wedge = 8,
    Hexahedron = 9,
    Polyhedron = 16,
    Edge_3 = 34,
    Triangle_6 = 36,
    Quadrilateral_8 = 37,
    Quadrilateral_9 = 35,
    Tetrahedron_10 = 38,
    Pyramid_13 = 39,
    Wedge_15 = 40,
    Wedge_18 = 41,
    Hexahedron_20 = 48,
    Hexahedron_24 = 49,
    Hexahedron_27 = 50,
    // Hexahedron_64 = 51,
    // Hexahedron_125 = 52,
    // Hexahedron_216 = 53,
    // Hexahedron_343 = 54,
    // Hexahedron_512 = 55,
    // Hexahedron_729 = 56,
    // Hexahedron_1000 = 57,
    // Hexahedron_1331 = 64,
    // Hexahedron_Spectral_64 = 65,
    // Hexahedron_Spectral_125 = 66,
    // Hexahedron_Spectral_216 = 67,
    // Hexahedron_Spectral_343 = 68,
    // Hexahedron_Spectral_512 = 69,
    // Hexahedron_Spectral_729 = 70,
    // Hexahedron_Spectral_1000 = 71,
    // Hexahedron_Spectral_1331 = 72,
    Mixed = 112,
  };
  static int toXdmfType(const std::string typeName);
  static const std::string toString(int type);
  static int getOffset(int type);
  static int getDim(int type);
};

namespace IO {
typedef enum { HDF5, ASCII } Encoding;
/*!
 * \class XdmfTool
 * \brief This class supports the input/output of meshes in XDMF
 * (http://www.xdmf.org) format. It creates an XML file that describes the data
 * and points to a HDF5 file that stores the actual problem data. Parallel IO is
 * supported.
 */
class XdmfTool {
 protected:
  Communicator* comm_; /*!< utilities MPI communicator */
  std::string filename_;
  std::string filemode_;
  hid_t h5id_; /*!< HDF5 file handle */
  pugi::xml_document* xml_doc_;
  Encoding encoding_;
  std::string regionName_;
  std::string xpath_;
  DataLocation readAttributeData(std::vector<double>& attrData, std::vector<llabel>& gdims,
                                 const std::string gridName,
                                 const std::string fieldName) const;
  DataLocation readAttributeDataSeq(std::vector<double>& attrData,
                                    std::vector<llabel>& gdims,
                                    const std::string gridName,
                                    const std::string fieldName) const;
  pugi::xml_node writeAttributeNode(DataLocation loc,
                                    const std::string gridName,
                                    const std::string fieldName) const;
  pugi::xml_node writeAttributeNodeSeq(DataLocation loc,
                                       const std::string gridName,
                                       const std::string fieldName,
                                       int rank) const;

 public:
  /*! * * * * * * * * * * * * * * * common * * * * * * * * * * * * * */
  static const Encoding defaultEncoding_ = Encoding::HDF5;
  static const std::map<int, std::pair<int, int>> xdmf2hsfType_;
  static const std::map<std::pair<int, int>, int> hsf2xdmfType_;
  static int xdmf2geomType(int t);

  XdmfTool()
      : comm_(NULL),
        filename_(""),
        filemode_(""),
        h5id_(-1),
        xml_doc_(NULL),
        encoding_(defaultEncoding_) {}
  /*! Constructor */
  XdmfTool(Communicator* comm, const std::string& filename = "",
           const std::string file_mode = "a",
           const Encoding encoding = defaultEncoding_, bool use_mpi_io = true);
  /*! Destrctor */
  virtual ~XdmfTool();

  /*! reset tool to another state, destructor and then reconstruct */
  void reset(Communicator* comm, const std::string& filename,
             const std::string file_mode,
             const Encoding encoding = defaultEncoding_,
             bool use_mpi_io = true);

  /*!
   * \brief Return local range for the calling process, partitioning the
   * global [0, N - 1] range across all ranks into partitions of almost equal
   * size.
   * \param[in] rank MPI rank of the caller
   * \param[in] N The value to partition
   * \param[in] size The number of MPI ranks across which to partition
   */
  template <typename I>
  static constexpr std::array<I, 2> localRange(int rank, I N, int size) {
    // ASSERT(rank >= 0);
    // ASSERT(N >= 0);
    // ASSERT(size > 0);

    // Compute number of items per rank and remainder
    const I n = N / size;
    const I r = N % size;

    // Compute local range
    if (static_cast<I>(rank) < r)
      return {rank * (n + 1), rank * (n + 1) + n + 1};
    else
      return {rank * n + r, rank * n + r + n};
  }
  /*!
   * \brief Return which rank owns index in global range [0, N - 1] (inverse
   * of MPI::local_range).
   * \param[in] size Number of MPI ranks
   * \param[in] index The index to determine owning rank
   * \param[in] N Total number of indices
   * \return The rank of the owning process
   */
  static constexpr int indexOwner(int size, llabel index, llabel N) {
    // ASSERT(index < N);

    // Compute number of items per rank and remainder
    const llabel n = N / size;
    const llabel r = N % size;

    if (index < r * (n + 1)) {
      // First r ranks own n + 1 indices
      return index / (n + 1);
    } else {
      // Remaining ranks own n indices
      return r + (index - r * (n + 1)) / n;
    }
  }

  // void printFileInfo() const;
  void setRegionName(std::string name) { regionName_ = name; }
  std::string getRegionName() const { return regionName_; }

  /*! Get Dimensions attribute string */
  std::vector<llabel> getDataItemDims(const pugi::xml_node& dataItemNode) const;
  /*! get file path(not full path) and HDF5 internal path */
  void getHDF5Paths(std::array<std::string, 2>& paths,
                    const pugi::xml_node& dataItemNode) const;
  /*! Return data associated with a data set node */
  template <typename T>
  void getDataset(std::vector<T>& res, const pugi::xml_node& dataset_node,
                  const std::vector<llabel> shape_xml, const llabel totalNum,
                  std::array<llabel, 2> range = {{0, 0}}) const;
  template <typename T>
  void getDatasetSeq(std::vector<T>& res, const pugi::xml_node& dataset_node) const;

  template <typename T>
  void addDataItem(pugi::xml_node& xml_node, const hid_t h5id,
                   const std::string& h5path, const std::vector<T>& data,
                   llabel offset, const std::vector<llabel>& shape,
                   const std::string& dataType, bool use_mpi_io,
                   std::string name = "") const;
  template <typename T>
  void addDataItemSeq(pugi::xml_node& xml_node, const hid_t h5id,
                      const std::string& h5path, const std::vector<T>& data,
                      const std::vector<std::vector<llabel>>& shape,
                      const std::string& dataType, std::string name = "",
                      int rank = 0) const;

  /*! read one region's geometry data */
  llabel readGeometry(std::vector<Tensor1<double>>& coords,
                      const std::string name) const;
  /*! read selected points */
  llabel readGeometry(std::vector<Tensor1<double>>& coords,
                      const std::vector<llabel>& index, const std::string name) const;
  llabel readGeometrySize(const std::string name) const;
  /*! independent read coordinates */
  llabel readGeometrySeq(std::vector<Tensor1<double>>& coords,
                         const std::string name) const;

  /*! write one region's geometry data */
  void writeGeometry(const std::vector<Tensor1<double>>& coords,
                     const std::string name, const int gdim = 3) const;
  void writeGeometry(const std::vector<double>& points, const std::string name,
                     const int gdim = 3) const;
  /*! write to local points to global location */
  void writeGeometry(const std::vector<Tensor1<double>>& coords,
                     const std::vector<llabel>& index, llabel totalNodeNum,
                     const std::string name, const int gdim = 3) const;
  void writeGeometrySeq(const std::vector<Tensor1<double>>& coords,
                        const std::string name, const int gdim = 3) const;
  /*! add time node  */
  void writeTimeNode(scalar time, const std::string name) const;

  /*! read one region's field data, T can be scalar/Tensor1/Tensor2 */
  template <typename T>
  DataLocation readAttribute(std::vector<T>& fieldData, const std::string gridName,
                             const std::string fieldName) const;
  template <typename T>
  DataLocation readAttributeSeq(std::vector<T>& fieldData, const std::string gridName,
                                const std::string fieldName) const;

  template <typename T>
  void writeAttribute(const std::vector<T>& fieldData, DataLocation loc,
                      const std::string gridName,
                      const std::string fieldName) const;
  template <typename T>
  void writeAttributeSeq(const std::vector<T>& fieldData, DataLocation loc,
                         const std::string gridName,
                         const std::string fieldName) const;
  /*! 暂不实现边界条件读写 */
};

struct VTKGeom {
  /// VTK geometry types
  static const int POINT = 1;

  /// Low-order (linear, straight-sided) VTK geometric types
  static const int SEGMENT = 3;
  static const int TRIANGLE = 5;
  static const int SQUARE = 9;
  static const int TETRAHEDRON = 10;
  static const int CUBE = 12;
  static const int PRISM = 13;
  static const int PYRAMID = 14;

  /// Legacy quadratic VTK geometric types
  static const int QUADRATIC_SEGMENT = 21;
  static const int QUADRATIC_TRIANGLE = 22;
  static const int QUADRATIC_SQUARE = 23;
  static const int BIQUADRATIC_SQUARE = 28;
  static const int QUADRATIC_TETRAHEDRON = 24;

  static const int QUADRATIC_CUBE = 25;
  static const int TRIQUADRATIC_CUBE = 29;
  static const int BIQUADRATIC_QUADRATIC_CUBE = 33;

  static const int QUADRATIC_PRISM = 26;
  static const int BIQUADRATIC_QUADRATIC_PRISM = 32;

  static const int QUADRATIC_PYRAMID = 27;

  /// Arbitrary-order VTK geometric types
  static const int LAGRANGE_SEGMENT = 68;
  static const int LAGRANGE_TRIANGLE = 69;
  static const int LAGRANGE_SQUARE = 70;
  static const int LAGRANGE_TETRAHEDRON = 71;
  static const int LAGRANGE_CUBE = 72;
  static const int LAGRANGE_PRISM = 73;
  static const int LAGRANGE_PYRAMID = 74;

  static const std::map<int, int> vtk2xdmfType_;

  /// permutation from HSF's vertex ordering to VTK's vertex ordering.
  /// If the orderings are the same, the vertex permutation will be NULL.
  static const int* vertexPermutation[BasicGeometry::NUMBASICGEOTYPE];

  /// Map from HSF's Geometry::Type to linear VTK geometries.
  static const int map[BasicGeometry::NUMBASICGEOTYPE];
  /// Map from HSF's Geometry::Type to legacy quadratic VTK geometries/
  static const int quadraticMap[BasicGeometry::NUMBASICGEOTYPE];
  /// Map from HSF's Geometry::Type to arbitrary-order Lagrange VTK geometries
  static const int highOrderMap[BasicGeometry::NUMBASICGEOTYPE];

  /// Given a VTK geometry type, return the corresponding HSF Geometry::Type.
  static BasicGeometry::Type getHSFGeometry(int vtk_geom);
  /// For the given VTK geometry type and number of points, return the order of
  /// the element.
  static int getOrder(int vtk_geom, int npoints);
  /// judge if a VTK geometry type is an arbitrary-order Lagrange element?
  static bool isLagrange(int vtk_geom);
  /// judge if a VTK geometry type is a legacy quadratic Lagrange element?
  static bool isQuadratic(int vtk_geom);
};

} /* End namespace IO */
}  // namespace HSF

#endif /* end of include guard: MESH_IO_XDMFTOOL_HPP */
