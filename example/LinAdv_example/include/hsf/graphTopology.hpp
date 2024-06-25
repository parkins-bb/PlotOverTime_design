#ifndef TOPOLOGY_GRAPH_GRAPHTOPOLOGY_HPP
#define TOPOLOGY_GRAPH_GRAPHTOPOLOGY_HPP
/*! \file graphTopology.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-01-30
 *  \modified 2023-04-13
 *  \brief unstructured mesh, graph topology, COO format
 */
#include "topology.hpp"

namespace HSF {

/*! Helper struct for defining a kind of connectivity */
struct Connection {
  label from;
  llabel to;
  Connection() = default;
  Connection(label from, llabel to) : from(from), to(to) {}

  bool operator==(const Connection &rhs) const {
    return (from == rhs.from) && (to == rhs.to);
  }
  bool operator<(const Connection &rhs) const {
    return (from == rhs.from) ? (to < rhs.to) : (from < rhs.from);
  }
};

/*!
 * \class GraphTopology
 * \brief Stores the connectivity of elements of TYPE I
    to elements of TYPE II, for example, it may be Cell-To-Face
    connectivity table, etc.
 */
class GraphTopology : public Topology {
 private:
  std::vector<label> row_;    /*! row indexes */
  std::vector<llabel> col_;   /*! col indexes  */
  std::vector<label> cprRow_; /*!< compressed row indexes */
  bool sorted_;               /*! if indexes're sorted  */
  bool buildRow_;             /*!< row_ is empty when buildRow_ is false */
  bool globalId_; /*!< whether the col index is global id or local id */

 public:
  /*! default constructor */
  GraphTopology();
  /*! Copy constructor */
  GraphTopology(const GraphTopology &);
  /*! COO topo constructor
   *  \param [in] row : row indexes
   *  \param [in] col : col indexes
   *  \param [in] nRow : row size
   *  \param [in] nCol : col size
   */
  GraphTopology(const std::vector<label> &row, const std::vector<llabel> &col,
                label nRow, llabel nCol = -1);

  /*! Create a table with an upper limit for the number of connections.
   *  col_[i] set to -1 */
  explicit GraphTopology(label dim, label connections_per_row = 4);

  /*! Create a table from a list of connections, see makeFromList(). */
  GraphTopology(label nrows, std::vector<Connection> &list);

  /*! Create a table from a std::vectorstd::vector */
  GraphTopology(const std::vector<std::vector<llabel> > &data);

  /*! Create a table with one entry per row with column indices given by
   * 'partitioning'. */
  GraphTopology(label nrows, label *partitioning);

  /*! default deconstructor */
  ~GraphTopology();

  /*! Assignment operator */
  GraphTopology &operator=(const GraphTopology &topo);

  /// Move constructor
  GraphTopology(GraphTopology &&list) = default;
  /// Move assignment operator
  GraphTopology &operator=(GraphTopology &&list) = default;

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */

  /*! Returns index of the connection between element i of TYPE I and element j
   * of TYPE II. If there is no connection between element i and element j
   * established in the table, then the return value is -1. */
  label index(label i, llabel j) const;

  /*! return element index value of row i, position j */
  inline llabel operator()(label i, llabel j) const {
    return col_[cprRow_[i] + j];
  }
  inline llabel &operator()(label i, llabel j) { return col_[cprRow_[i] + j]; }

  /*! Set the rows and the number of all connections for the table. Does NOT
   * initialize the whole array cprRow_ ! (cprRow_[0]=0 and cprRow_[rows]=nnz
   * only) */
  void setDims(label rows, label nnz);
  void makeI(int nrows);
  void addAColumnInRow(int r) { cprRow_[r]++; }
  void addColumnsInRow(int r, int ncol) { cprRow_[r] += ncol; }
  void makeJ();
  void addConnection(int r, int c) { col_[cprRow_[r]++] = c; }
  void addConnections(int r, const int *c, int nc);
  void shiftUpI();

  /*! Return the number of col indexes for row i */
  inline label rowSize(label i) const {
    ASSERT(sorted_ == true);
    return cprRow_[i + 1] - cprRow_[i];
  }
  /*! Return col indexes pointer of row i */
  inline const llabel *getRow(label i) const {
    ASSERT(sorted_ == true);
    return col_.data() + cprRow_[i];
  }
  /*! Return col indexes pointer of row i */
  inline llabel *getRow(label i) {
    ASSERT(sorted_ == true);
    return col_.data() + cprRow_[i];
  }
  /*! Return row i in array row (the GraphTopology must be finalized) */
  inline void getRow(label i, std::vector<llabel> &row) const {
    ASSERT(sorted_ == true);
    const llabel *ind = getRow(i);
    row.assign(ind, ind + rowSize(i));
  }

  /*! Return row indexes pointer */
  inline label *row() { return row_.data(); }
  /*! Return row indexes pointer */
  inline llabel *col() { return col_.data(); }
  inline label *cprRow() { return cprRow_.data(); }
  inline const label *row() const { return row_.data(); }
  inline const llabel *col() const { return col_.data(); }
  inline const std::vector<llabel> &colVec() const { return col_; }
  inline const label *cprRow() const { return cprRow_.data(); }
  inline const std::vector<label> &cprRowVec() const { return cprRow_; }
  inline bool sorted() const { return sorted_; }
  inline void setSorted(bool torf) { sorted_ = torf; }
  inline void setBuildRow(bool torf) { buildRow_ = torf; }
  inline bool isGlobalId() const { return globalId_; }
  inline void setGlobalId(bool torf) { globalId_ = torf; }

  /*! return raw std::vector, rarely used */
  const std::vector<label> &getCprRowVector() const { return cprRow_; }
  const std::vector<llabel> &getColVector() const { return col_; }

  inline void resizeRow(label size) {
    real_.r = local_.r = size;
    if (buildRow_) row_.resize(size);
    cprRow_.resize(size + 1);
  }
  inline void resizeCol(llabel size) {
    real_.nnz = local_.nnz = size;
    col_.resize(size);
  }
  /*! Returns the number of TYPE II elements (after finalize() is called). */
  llabel width() const;

  /*! @brief Sort the column (TYPE II) indices in each row. */
  void sortRows();
  /*! build row_ from cprRow_ */
  void buildRow();
  /*!  Replace row_ and col_ with newI and newJ.
   * If newsize < 0, then the size of the GraphTopology is not modified. */
  void setIJ(label *newI, llabel *newJ, label newsize = -1);

  /*! Establish connection between element i and element j in the table. The
   *  return value is the index of the connection. It returns -1 if it fails to
   *  establish the connection. Possibilities are there is not enough memory on
   *  row i to establish connection to j, or an attempt to establish new
   *  connection after calling Finalize(). */
  llabel push(label i, llabel j);

  /*! Finalize the table initialization. The function may be called only once,
   *  after the table has been initialized, in order to compress array col (by
   *  getting rid of -1's in array col). Calling this function will "freeze" the
   *  table and function Push will work no more. */
  void finalize();

  /*! Create the table from a list of connections {(from, to)}, where 'from' is
   *  a TYPE I index and 'to' is a TYPE II index. The list is assumed to be
   *  sorted and free of duplicities */
  void makeFromList(label nrows, std::vector<Connection> &list);

  /*! deep copy */
  void copy(GraphTopology &copy) const;
  /*! swap this with other */
  void swap(GraphTopology &other);

  /*! redistribute topology after partition */
  void exchangeData(Communicator *comm, const std::vector<label> &partition);

  /*! append ghost layer to  xx2node topology */
  void addGhostLayer(label cellNum, const llabel *c2nodeNum,
                     const llabel *c2node);

  /*! reset localRowSize to newRowNum */
  void resetRowSize(label newRowNum);

  void reorderRow(const std::vector<label> &perm);
  void reorderCol(const std::vector<label> &perm);
  virtual int reorderTopo(std::vector<label> &perm) override;
  virtual void print(std::ostream &out = *POUT.stream(),
                     int width = 4) const override;
  /*! clear graphTopology */
  virtual void clear() override;
  // virtual std::size_t memoryUsage() const override;
};

/*!  Transpose a GraphTopology */
void transpose(const GraphTopology &A, GraphTopology &At);
GraphTopology *transpose(const GraphTopology &A);

/*!  Transpose an std::vector<llabel> */
void transpose(const std::vector<llabel> &A, GraphTopology &At);

/*!  C = A * B  (as boolean matrices) */
void mult(const GraphTopology &A, const GraphTopology &B, GraphTopology &C);
GraphTopology *mult(const GraphTopology &A, const GraphTopology &B);

} /* End namespace HSF */

#endif /* end of include guard: TOPOLOGY_GRAPH_GRAPHTOPOLOGY_HPP */
