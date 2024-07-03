/*! \file hdf5Tool.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2022-05-25
 *  \modified 2022-06-02
 *  \brief class hdf5Tool, an interface to some HDF5 functionality
 */

#ifndef MESH_IO_HDF5TOOL_HPP
#define MESH_IO_HDF5TOOL_HPP
#include <mpi.h>
#include <sys/stat.h>

#include <array>
#include <stdexcept>
#include <vector>

#include "hdf5.h"
// 命名空间为HSF::IO
namespace HSF {
namespace IO {

class HDF5Tool {
#define HDF5_FAIL -1
 private:
  /*! return HDF5 data type */
  template <typename T>
  static hid_t hdf5Type() {    
    throw std::runtime_error("Cannot get HDF5 data type. ");
  }    // 返回HDF5数据类型
  /*! Add group to HDF5 file
   *  \param [in] handle HDF5 file handle
   *  \param[in] dataset_path Data set path to add
   */
  static void addGroup(const hid_t handle, const std::string& dataset_path);   // 用于在HDF5文件中添加一个组

 /* 
  * hid_t是一个由HDF5库定义的类型，它实际上是一个整数类型，用于表示HDF5对象（如文件、数据集、属性等）的标识符
  * hid_t类型的变量被称为HDF5对象标识符(handle)，用于在HDF5函数中引用特定的HDF5对象。
  * 每个hid_t标识符唯一地标识一个打开的HDF5对象。
  */
 public:
  // 打开一个HDF5文件并返回文件句柄
  /*!
   *  Open HDF5 and return file descriptor
   *  \param[in] comm MPI communicator
   *  \param[in] filename Name of the HDF5 file to open
   *  \param[in] mode Mode in which to open the file (w, r, a)
   *  \param[in] use_mpi_io True if MPI-IO should be used
   */
  static hid_t openFile(MPI_Comm comm, const std::string& filename,
                        const std::string& mode, const bool use_mpi_io);
  
  // 关闭一个HDF5文件，实际上输入的hid_t类型的参数handle就是函数openFile()的返回值。
  /*!
   *  Close HDF5 file
   *  \param[in] handle HDF5 file handle
   */
  static void closeFile(const hid_t handle);

  // 将数据刷新到文件中，以提高数据完整性。
  // 如果程序意外中断，刷新数据可以保证数据被写入文件中，而不是停留在内存中的缓冲区。
  /*!
   *  Flush data to file to improve data integrity after interruption
   *  \param[in] handle HDF5 file handle
   */
  static void flushFile(const hid_t handle);

  // 获取HDF5文件的文件名
  /*!
   *  Get filename
   *  \param[in] handle HDF5 file handle
   *  return The filename
   */
  static std::string getFilename(hid_t handle);

  // 获取HDF5文件的基本名称（不包括路径信息）
  /*! get base name(exclude path info) */
  static std::string getBasename(hid_t handle);

  // 检查HDF5文件是否存在指定路径的数据集
  /*!
   *  Check for existence of dataset in HDF5 file
   *  \param[in] handle HDF5 file handle
   *  \param[in] dataset_path Data set path
   *  \return True if @p dataset_path is in the file
   */
  static bool hasDataset(const hid_t handle, const std::string& dataset_path);

  // 获取数据集的形状（各维度的大小）
  /*!
   *  Get dataset shape (size of each dimension) from Dataspace
   *  \param[in] handle HDF5 file handle
   *  \param[in] dataset_path Dataset path
   *  \return The shape of the dataset (row-major)
   */
  static std::vector<hsize_t> getDatasetShape(const hid_t handle,
                                              const std::string& dataset_path);

  // 将数据写入现有的HDF5文件。块/点
  /*!
   * Write data to existing HDF file as defined by range blocks on each process
   * \param[in] handle HDF5 file handle
   * \param[in] dataset_path Path for the dataset in the HDF5 file
   * \param[in] data Data to be written, flattened into 1D vector (row-major
   * storage)
   * \param[in] range The local range on this processor
   * \param[in] global_size The global shape shape of the array
   * \param[in] use_mpi_io True if MPI-IO should be used
   * \param[in] use_chunking True if chunking should be used
   */
  template <typename T>
  static void writeDataset(const hid_t handle, const std::string& dataset_path,
                           const T* data, const std::array<hsize_t, 2>& range,
                           const std::vector<hsize_t>& global_size,
                           bool use_mpi_io, bool use_chunking);
  /*!
   * Write data to existing HDF file as defined by points on each process
   * \param[in] handle HDF5 file handle
   * \param[in] dataset_path Path for the dataset in the HDF5 file
   * \param[in] data Data to be written, flattened into 1D vector (row-major
   * storage)
   * \param[in] global_size The global shape shape of the array
   * \param[in] nPts selected point num
   * \param[in] coord The coord parameter is a pointer to a buffer containing a
   * serialized 2-dimensional array of size num_elements by the rank of the
   * dataspace. The array lists dataset elements in the point selection; that
   * is, it’s a list of of zero-based values specifying the coordinates in the
   * dataset of the selected elements. The order of the element coordinates in
   * the coord array specifies the order in which the array elements are
   * iterated through when I/O is performed. Duplicate coordinate locations are
   * not checked.
   * \param[in] use_mpi_io True if MPI-IO should be used
   * \param[in] use_chunking True if chunking should be used
   */
  template <typename T>
  static void writeDataset(const hid_t handle, const std::string& dataset_path,
                           const T* data,
                           const std::vector<hsize_t>& global_size, size_t nPts,
                           hsize_t* coord, bool use_mpi_io, bool use_chunking);

  // 从DHF5文件中读取数据。块/点
  /*!
   * Read data from a HDF5 dataset "dataset_path" as defined by range blocks on
   * each process.
   * \param[in] handle HDF5 file handle
   * \param[in] dataset_path Path for the dataset in the HDF5 file
   * \param[in] range The local range on this processor
   * \return Flattened 1D array of values.
   * If range = {1, 0}, then all data is read on this process.
   */
  template <typename T>
  static void readDataset(std::vector<T>& res, const hid_t handle,
                          const std::string& dataset_path,
                          const std::array<hsize_t, 2>& range);

  /*!
   * Read data from a HDF5 dataset "dataset_path" as defined by points on
   * each process.
   * \param[in] handle HDF5 file handle
   * \param[in] dataset_path Path for the dataset in the HDF5 file
   * \param[in] nPts selected point num
   * \param[in] coord The coord parameter is a pointer to a buffer containing a
   * serialized 2-dimensional array of size num_elements by the rank of the
   * dataspace. The array lists dataset elements in the point selection; that
   * is, it’s a list of of zero-based values specifying the coordinates in the
   * dataset of the selected elements. The order of the element coordinates in
   * the coord array specifies the order in which the array elements are
   * iterated through when I/O is performed. Duplicate coordinate locations are
   * not checked.
   * \return Flattened 1D array of values.
   */
  template <typename T>
  static void readDataset(std::vector<T>& res, const hid_t handle,
                          const std::string& dataset_path, size_t nPts,
                          hsize_t* coord);

  /*!
   *  Set MPI atomicity. See
   *  https://support.hdfgroup.org/HDF5/doc/RM/RM_H5F.html#File-SetMpiAtomicity
   *  Writes must be followed by an MPI_Barrier on the communicator before
   *  any subsequent reads are guaranteed to return the same data.
   */
  // static void setMpiAtomicity(const hid_t handle, const bool atomic);

  /*!
   *  Get MPI atomicity. See
   *  https://www.open-mpi.org/doc/v2.0/man3/MPI_File_get_atomicity.3.php
   */
  // static bool getMpiAtomicity(const hid_t handle);
};

// 内联函数，检查文件是否存在。使用stat函数检查指定路径的文件是否存在。
// check if a file(full path) is exist
inline bool fileExist(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

} /* End namespace IO */
} /* End namespace HSF */

#endif /* end of include guard: MESH_IO_HDF5TOOL_HPP */
