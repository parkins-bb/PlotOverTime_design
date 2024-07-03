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
  std::string filename_;  /*文件名*/
  std::string filemode_;  /*文件模式*/
  hid_t h5id_; /*!< HDF5 file handle */  // hid_t不是一个类，是一个由HDF5库定义的类型
                                         // hid_t用于表示HDF5对象（如文件、数据集、属性等）的标识符
  pugi::xml_document* xml_doc_;  /*XML文档对象指针，用于处理XML结构*/
  Encoding encoding_;  /*编码类型，用于指定数据的编码方式（HDF5或ASCII）*/
  std::string regionName_;  /*区域名称，用于标识网格区域*/
  std::string xpath_;  /*XPath表达式，用于在XML文档中查找节点*/
  /*
   * @brief 读取属性数据
   * @param attrData存储属性数据的向量；gdims存储全局维度的向量；girdName网格名称；fieldName字段名称
   * @return 数据位置
   * */
  DataLocation readAttributeData(std::vector<double>& attrData, std::vector<llabel>& gdims,
                                 const std::string gridName,
                                 const std::string fieldName) const;
  /*
   * @brief 顺序读取属性数据
   * @param attrData存储属性数据的向量；gdims存储全局维度的向量；gridName网格名称；fieldName字段名称
   * @return 数据位置
   * */
  DataLocation readAttributeDataSeq(std::vector<double>& attrData,
                                    std::vector<llabel>& gdims,
                                    const std::string gridName,
                                    const std::string fieldName) const;
  /*
   * @brief 写入属性节点
   * @param loc数据位置，gridName网格名称，fieldName字段名称
   * @return XML节点
   * */
  pugi::xml_node writeAttributeNode(DataLocation loc,
                                    const std::string gridName,
                                    const std::string fieldName) const;
  /*
   * @brief 顺序写入属性节点
   * @param loc数据位置，gridName网格名称，fieldName字段名称, MPI进程的rank
   * @return XML节点
   * */
  pugi::xml_node writeAttributeNodeSeq(DataLocation loc,
                                       const std::string gridName,
                                       const std::string fieldName,
                                       int rank) const;

 public:
  /*! * * * * * * * * * * * * * * * common * * * * * * * * * * * * * */
  static const Encoding defaultEncoding_ = Encoding::HDF5;  // 默认编码类型，初始化为HDF5
  static const std::map<int, std::pair<int, int>> xdmf2hsfType_;  // 从XDMF类型到HSF类型的映射
  static const std::map<std::pair<int, int>, int> hsf2xdmfType_;  // 从HSF类型到XDMF类型的映射
  static int xdmf2geomType(int t);  // 将XDMF类型转换为几何类型，输入int t为XDMF类型，输出int为几何类型

  XdmfTool()
      : comm_(NULL),
        filename_(""),
        filemode_(""),
        h5id_(-1),
        xml_doc_(NULL),
        encoding_(defaultEncoding_) {}
  /*! Constructor */
  // 构造函数
  XdmfTool(Communicator* comm, const std::string& filename = "",
           const std::string file_mode = "a",
           const Encoding encoding = defaultEncoding_, bool use_mpi_io = true);  /*是否使用了MPI-IO，默认值为true*/
  /*! Destrctor */
  virtual ~XdmfTool();

  /*! reset tool to another state, destructor and then reconstruct */
  /*
   * @brief 重新初始化XdmfTool对象的状态。销毁当前对象的状态，并重新根据提供的参数构造对象。
   * @descrip 1.重新初始化XdmfTool对象；2.更新MPI通信器；3.设置新的文件名和文件模式（根据新的文件名和文件模式重新打开文件）；
   * @descrip 4.设置新的编码方式（HDF5或ASCII）；5.决定是否支持MPI-IO
   * 销毁当前XdmfTool对象的“状态”，例如关闭已经打开的文件，释放资源等。重新初始化，使用新的参数重新设置对象的状态，类似于构造函数的操作
   * 清理当前XmdfTool对象的状态，使用新参数重新初始化对象，使其可以重用对象，而不必销毁和重新创建。
   * */
  void reset(Communicator* comm, const std::string& filename,
             const std::string file_mode,
             const Encoding encoding = defaultEncoding_,
             bool use_mpi_io = true);

  /*
   * @brief 用于确定在并行环境中，每个进程处理的数据范围。具体来说是将全局范围[0, N-1]均匀划分给size个进程，并返回指定rank进程所负责的局部范围
   * @param rank进程的rank(从0到size-1); n全局范围的大小，即需要划分的总元素数；size 进程的总数
   * @return std::array<I, 2>包含两个元素的数组，表示指定rank负责的局部范围。数组的第一个元素是局部范围的起始索引；第二个元素是局部范围的结束索引
   * */
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

  /*
   * @brief 确定在并行计算环境中，给定全局索引‘index’属于哪个进程。
   * 通过将全局范围[0, N-1]划分给size个进程，计算出指定index所在的进程
   * @param size进程的总数；index需要确定所有者的全局索引；N全局范围的大小，即总元素数。
   * @return 拥有给定索引index的进程的rank
   * */
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
  /*
   * @biref 用于设置XdmfTool对象的区域名称region name, 区域名称用于标识网格或数据集的特定区域，可以在后续操作中用来引用或操作该区域
   * */
  void setRegionName(std::string name) { regionName_ = name; }

  /*
   * @brief 用于获取XdmfTool对象中存储的区域名称。这个区域名称用于表示网格或数据集的特定区域，在后续操作中可以用来引用或操作该区域
   * 一个xdmfTool对象可能对应一个Region，也可能对应多个Region
   * */
  std::string getRegionName() const { return regionName_; }

  /*
   * @brief 用于获取XML数据项节点的维度信息。即解析传入的XML数据项节点，提取并返回该数据项的维度（形状）信息
   * @param 类型:const pugi::xml_node&, 传入的XML数据项节点，包含了数据项的维度信息
   * @return 类型:std::vector<llabel>, 返回一个向量，包含数据项的维度信息。
   * @example 假设该XML数据项节点对应的是二维数据，把么返回信息通常会包含[num_rows, num_columns]
   * */
  /*! Get Dimensions attribute string */
  std::vector<llabel> getDataItemDims(const pugi::xml_node& dataItemNode) const;

  /*! get file path(not full path) and HDF5 internal path */
  /*
   * @brief 从给定的XML数据项节点中提取HDF5文件路径和HDF5内部路径，并将他们存储在传入的paths数组中。
   * @param paths存储HDF5文件路径和HDF5内部路径的数组。数组的第一个元素是HDF5文件路径，数组的第二个元素是HDF5内部路径
   * @param dataItemNode为传入的XML数据项节点，包含了与HDF5文件相关的路径信息
   * */
  void getHDF5Paths(std::array<std::string, 2>& paths,
                    const pugi::xml_node& dataItemNode) const;

  /*! Return data associated with a data set node */
  /*
   * @biref 从给定的XML数据集节点中读取数据，并将其存储在传入的向量res中。
   * @param std::vector<T>& res存储读取结果的数据向量；const pugi::xml_node&传入的XML数据集节点，包含数据的元数据和存储位置；
   * @param const std::vector<llabel>为XML数据集节点中定义的数据形状（维度）；
   * @param const llabel为数据的总元素数；
   * @param std::array<llabel, 2> range为可选的范围参数，指定读取数据的范围；
   * */
  template <typename T>
  void getDataset(std::vector<T>& res, const pugi::xml_node& dataset_node,
                  const std::vector<llabel> shape_xml, const llabel totalNum,
                  std::array<llabel, 2> range = {{0, 0}}) const;

  /*
   * @brief 用于顺序读取给定的XML数据集节点中的数据，并将其存储在传入的向量res中。
   * 该函数假设数据集是顺序存储的，并在读取过程中不进行任何并行或分块操作
   * */
  template <typename T>
  void getDatasetSeq(std::vector<T>& res, const pugi::xml_node& dataset_node) const;

  /*
   * @brief 
   *
   * */
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
  /*
   * @brief 读取指定区域的几何数据并存储在向量中
   * @param std::vector<Tensor1<double>>& coords: 用于存储几何数据的向量
   * @param const std::string name: 区域的名称
   * @return llabel: 读取的几何数据的大小
   * */
  /*! read one region's geometry data */
  llabel readGeometry(std::vector<Tensor1<double>>& coords,
                      const std::string name) const;

  /*
   * @brief 读取指定区域的几何数据并存储在向量中
   * @param std::vector<Tensor1<double>>& coords: 用于存储几何数据的向量
   * @param const std::vector<llabel>& index: 选定点的索引
   * @param const std::string name: 区域的名称
   * @return llabel: 读取的几何数据的大小
   * */
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
