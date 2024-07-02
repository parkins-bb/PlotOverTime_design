/*! \file hsfMpi.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2024-01-16
 *  \brief
 */
#ifndef REGION_MESH_HSFMPI_HPP
#define REGION_MESH_HSFMPI_HPP

#include <mpi.h>

#include <complex>
#include <limits>
#include <numeric>
#include <vector>

#if defined(_WIN32)
#define NOMINMAX
#endif
#include "utilities.h"

namespace HSF {

/*!
 * Return the corresponding MPI_Datatype for simple C++ data types.
 * \tparam T C++ type for which to return the corresponding MPI_Datatype
 */
template <typename T>
MPI_Datatype mpiType() {
  return T::mpiType();
}
/*! return MPI datatype for C++ char */
template <>
inline MPI_Datatype mpiType<char>() {
  return MPI_CHAR;
}
/*! return MPI datatype for C++ bool */
// template <>
// inline MPI_Datatype mpiType<bool>() {
//  return MPI_CXX_BOOL;
//}
/*! return MPI datatype for C++ int */
template <>
inline MPI_Datatype mpiType<int>() {
  return MPI_INT;
}
/*! return MPI datatype for C++ long */
template <>
inline MPI_Datatype mpiType<long>() {
  return MPI_LONG;
}
/*! return MPI datatype for C++ unsigned long */
template <>
inline MPI_Datatype mpiType<unsigned long>() {
  return MPI_UNSIGNED_LONG;
}
/*! return MPI datatype for C++ long long int */
template <>
inline MPI_Datatype mpiType<long long int>() {
  return MPI_LONG_LONG_INT;
}
/*! return MPI datatype for C++ float */
template <>
inline MPI_Datatype mpiType<float>() {
  return MPI_FLOAT;
}
/*! return MPI datatype for C++ double */
template <>
inline MPI_Datatype mpiType<double>() {
  return MPI_DOUBLE;
}
/*! return MPI datatype for C++ bool */
template <>
inline MPI_Datatype mpiType<bool>() {
  return MPI_CXX_BOOL;
}
/*! return MPI datatype for C++ std::complex<float> */
template <>
inline MPI_Datatype mpiType<std::complex<float> >() {
#ifndef MPI_CXX_FLOAT_COMPLEX
#define MPI_CXX_FLOAT_COMPLEX MPI_COMPLEX
#endif
  return MPI_CXX_FLOAT_COMPLEX;
}
/*! return MPI datatype for C++ std::complex<double> */
template <>
inline MPI_Datatype mpiType<std::complex<double> >() {
#ifndef MPI_CXX_DOUBLE_COMPLEX
#define MPI_CXX_DOUBLE_COMPLEX MPI_DOUBLE_COMPLEX
#endif
  return MPI_CXX_DOUBLE_COMPLEX;
}
/*! return MPI datatype for C++ std::pair<int,int> */
template <>
inline MPI_Datatype mpiType<std::pair<int, int> >() {
  return MPI_2INT;
}
/*! return MPI datatype for C++ std::pair<long int,long int> */
template <>
inline MPI_Datatype mpiType<std::pair<long int, long int> >() {
  static MPI_Datatype llMpiType = MPI_DATATYPE_NULL;
  if (llMpiType == MPI_DATATYPE_NULL) {
    /*! creates an MPI datatype by replicating an existing one a certain number
     * of times. */
    MPI_Type_contiguous(2, mpiType<long int>(), &llMpiType);
    MPI_Type_commit(&llMpiType);
  }
  return llMpiType;
}

/*!
 * Perform an MPI_Alltoallv. Each rank sends sbuf[i] to process i
 * The results are received in a single contiguous vector rbuf
 * pbuf has pointers into rbuf, with pbuf[i] pointing to the
 * data received from rank i.
 * \tparam T type of data to send
 * \param sbuf send buffers (should be size P)
 * \param rbuf receive buffer, can be empty, will be allocated
 * \param pbuf pointers (to positions in rbuf) to where data
 * received from different ranks start
 * \param Ttype MPI_Datatype corresponding to the template parameter T
 * TODO: 增加非阻塞版本的allToallv？
 */
template <typename T>
void allToallv(MPI_Comm mpiComm, std::vector<std::vector<T> >& sbuf,
               std::vector<T>& rbuf, std::vector<T*>& pbuf,
               const MPI_Datatype Ttype);

/*!
 * Perform an MPI_Alltoallv. Each rank sends sbuf[i] to process
 * i. The results are received in a single contiguous vector which
 * is returned.
 * \tparam T type of data to send, this should have a
 * corresponding mpiType<T>() implementation or should define
 * T::mpiType()
 * \param sbuf send buffers (should be size P)
 * \rbuf return, receive buffer
 */
template <typename T>
void allToallv(std::vector<T>& rbuf, MPI_Comm comm,
               std::vector<std::vector<T> >& sbuf) {
  std::vector<T*> pbuf;
  allToallv(comm, sbuf, rbuf, pbuf, mpiType<T>());
}

/*!
 * Receive a vector of T's from process src, with tag. The message
 * size does not need to be known in advance.
 * \tparam T template parameter of vector to receive, should have
 * a corresponding mpiType<T>() implementation
 * \param src process to receive from
 * \param tag tag to match the message
 * \param rbuf return, std::vector<T> with the data to be received.
 */
template <typename T>
void recv(std::vector<T>& rbuf, int src, label tag, MPI_Comm comm_) {
  MPI_Status stat;
  MPI_Probe(src, tag, comm_, &stat);
  int msgsize;
  MPI_Get_count(&stat, mpiType<T>(), &msgsize);
  rbuf.resize(msgsize);
  MPI_Recv(rbuf.data(), msgsize, mpiType<T>(), src, tag, comm_,
           MPI_STATUS_IGNORE);
}
/*!
 * Receive a vector of T's from any processes, with tag. The message
 * size does not need to be known in advance.
 * \tparam T template parameter of vector to receive, should have
 * a corresponding mpiType<T>() implementation
 * \param tag tag to match the message
 * \param rPair a pair that contains the rank of the process that sent the
 * message and a rvalue reference of std::vector<T> with the data to be
 * received.
 */
template <typename T>
void recvAnySrc(std::pair<label, std::vector<T> >& rPair, label tag,
                MPI_Comm comm_) {
  MPI_Status stat;
  /*! Tell MPI to receive msg without restricting the rank of the sender. */
  MPI_Probe(MPI_ANY_SOURCE, tag, comm_, &stat);
  int msgsize;
  MPI_Get_count(&stat, mpiType<T>(), &msgsize);
  rPair.second.resize(msgsize);
  MPI_Recv(rPair.second.data(), msgsize, mpiType<T>(), stat.MPI_SOURCE, tag,
           comm_, MPI_STATUS_IGNORE);
  rPair.first = stat.MPI_SOURCE;
}
/*!
 * Receive a value of T from any process src, with tag
 * \tparam T template parameter of vector to receive, should have
 * a corresponding mpiType<T>() implementation
 * \param tag tag to match the message
 */
template <typename T>
T recvOne(int src, label tag, MPI_Comm comm_) {
  T t;
  MPI_Recv(&t, 1, mpiType<T>(), src, tag, comm_, MPI_STATUS_IGNORE);
  return t;
}

/*!
 * Return a subcommunicator with P ranks, starting from rank P0
 * [P0:stride:P0+stride*P) will be included in the new communicator
 * This operation is collective on all the processes in this communicator
 * \param P0 first rank in the new communicator
 * \param P number of ranks in the new communicator
 * \param stride stride between ranks in this communicator
 * determining which ranks will go into new communicator
 * \return new communicator containing P ranks from this
 * communicator [P0:stride:P0+stride*P)
 */
void mpiSubComm(MPI_Comm& newComm, const MPI_Comm& comm, int P0, int P,
                int stride = 1);

/// @brief Determine incoming graph edges using the NBX consensus
/// algorithm.
///
/// Given a list of outgoing edges (destination ranks) from this rank,
/// this function returns the incoming edges (source ranks) to this rank.
///
/// @note This function is for sparse communication patterns, i.e. where
/// the number of ranks that communicate with each other is relatively
/// small. It is scalable, i.e. no arrays the size of the communicator
/// are constructed and the communication pattern is sparse. It
/// implements the NBX algorithm presented in
/// https://dx.doi.org/10.1145/1837853.1693476.
///
/// @note For sparse graphs, this function has \f$O(\log p)\f$ cost,
/// where \f$p\f$is the number of MPI ranks. It is suitable for modest
/// MPI rank counts.
///
/// @note The order of the returned ranks is not deterministic.
///
/// @note Collective.
///
/// @param[in] comm MPI communicator
/// @param[in] edges Edges (ranks) from this rank (the caller).
/// @return Ranks that have defined edges from them to this rank.
std::vector<int> compute_graph_edges_nbx(MPI_Comm comm,
                                         std::vector<int>& edges);

/*!
 * \class Triplet
 * \brief helper class to communicate matrix data
 */
template <typename Cmpt>
class Triplet {
 public:
  label r_, c_;
  Cmpt v_;
  Triplet() {}
  Triplet(label row, label col, Cmpt value) : r_(row), c_(col), v_(value) {}
  static MPI_Datatype tripletMpiType_;
  static MPI_Datatype mpiType();
  static void freeMpiType();
};

/*!
 * \class IdxIJ
 * \brief helper class to get symm pattern matrix after pivoting
 */
class IdxIJ {
 public:
  label i_, j_;
  IdxIJ() {}
  IdxIJ(label ii, label jj) : i_(ii), j_(jj) {}
  static MPI_Datatype idxijMpiType_;
  static MPI_Datatype mpiType();
  static void freeMpiType();
};
/*!
 * \class IdxVal
 * \brief helper class to permute solved result when using pivoting
 */
template <typename Cmpt>
class IdxVal {
 public:
  label i_;
  Cmpt v_;
  IdxVal() {}
  IdxVal(label idx, Cmpt value) : i_(idx), v_(value) {}
  static MPI_Datatype idxvalMpiType_;
  static MPI_Datatype mpiType();
  static void freeMpiType();
};

}  // namespace HSF

#endif /* end of include guard: REGION_MESH_HSFMPI_HPP */
