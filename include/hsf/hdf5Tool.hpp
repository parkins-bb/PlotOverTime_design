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

namespace HSF {
namespace IO {

class HDF5Tool {
#define HDF5_FAIL -1
 private:
  /*! return HDF5 data type */
  template <typename T>
  static hid_t hdf5Type() {
    throw std::runtime_error("Cannot get HDF5 data type. ");
  }
  /*! Add group to HDF5 file
   *  \param [in] handle HDF5 file handle
   *  \param[in] dataset_path Data set path to add
   */
  static void addGroup(const hid_t handle, const std::string& dataset_path);

 public:
  /*!
   *  Open HDF5 and return file descriptor
   *  \param[in] comm MPI communicator
   *  \param[in] filename Name of the HDF5 file to open
   *  \param[in] mode Mode in which to open the file (w, r, a)
   *  \param[in] use_mpi_io True if MPI-IO should be used
   */
  static hid_t openFile(MPI_Comm comm, const std::string& filename,
                        const std::string& mode, const bool use_mpi_io);

  /*!
   *  Close HDF5 file
   *  \param[in] handle HDF5 file handle
   */
  static void closeFile(const hid_t handle);

  /*!
   *  Flush data to file to improve data integrity after interruption
   *  \param[in] handle HDF5 file handle
   */
  static void flushFile(const hid_t handle);

  /*!
   *  Get filename
   *  \param[in] handle HDF5 file handle
   *  return The filename
   */
  static std::string getFilename(hid_t handle);

  /*! get base name(exclude path info) */
  static std::string getBasename(hid_t handle);

  /*!
   *  Check for existence of dataset in HDF5 file
   *  \param[in] handle HDF5 file handle
   *  \param[in] dataset_path Data set path
   *  \return True if @p dataset_path is in the file
   */
  static bool hasDataset(const hid_t handle, const std::string& dataset_path);

  /*!
   *  Get dataset shape (size of each dimension) from Dataspace
   *  \param[in] handle HDF5 file handle
   *  \param[in] dataset_path Dataset path
   *  \return The shape of the dataset (row-major)
   */
  static std::vector<hsize_t> getDatasetShape(const hid_t handle,
                                              const std::string& dataset_path);

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

// check if a file(full path) is exist
inline bool fileExist(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

} /* End namespace IO */
} /* End namespace HSF */

#endif /* end of include guard: MESH_IO_HDF5TOOL_HPP */
