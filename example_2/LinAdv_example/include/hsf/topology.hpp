#ifndef ALGORITHM_TOPOLOGY_TOPOLOGY_HPP
#define ALGORITHM_TOPOLOGY_TOPOLOGY_HPP
/*! \file topology.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2013-04-12
 *  \brief  base class of topology
 * 用来描述一系列数据序列各元素对应关系的表，形式上呈现为行（其中一些数据序列对应的位置）、列（另一些数据数据序列对应的位置）关联关系的稀疏矩阵，可以是完全的图拓扑结构、也可以是有规律的其它结构。例如，在对数值计算中网格连接关系，或者数据分析中相关性关系进行抽象时，得到的关联矩阵、邻接矩阵等结构。这些矩阵一般具有很强的稀疏性。拓扑是对关联关系的抽象，其中不存在表示关联关系所需必要信息外的其它信息。
 */

#include "utilities.h"
namespace HSF {

template <typename T>
struct EntitiesNum {
  T r, c, nnz;
  EntitiesNum() : r(0), c(0), nnz(0) {}
  EntitiesNum(T row, T col, T Nnz = 0) : r(row), c(col), nnz(Nnz) {}
  /*! use default copy constructor */
};

/**
 * @brief    base class for topology
 */
class Topology {
 protected:
  const char *type_;            /*!< topology type: COO/Band/MultiBand */
  EntitiesNum<llabel> global_; /*!< glboal size, lazy load */
  EntitiesNum<label> local_;    /*!< local size */
  EntitiesNum<label> real_; /*!< local real size(others are ghost entities) */

 public:
  /// default constructors
  Topology();
  ///  constructors from local row col nnz size
  Topology(label row, label col, label Nnz = 0);

  /// default deconstructor, must be virtual
  virtual ~Topology();
  /// 工厂构造接口
  // Topology(const Parameter& para);
  /// Copy constructor
  Topology(const Topology &topo);
  /// Assignment operator
  Topology &operator=(const Topology &topo);

  /*! * * * * * * * * * * * * * * * * * * * * * * * * * */
  inline const char *type() const { return type_; }
  // entities num 访问接口
  inline EntitiesNum<llabel> &globalNum() { return global_; }
  inline EntitiesNum<label> &localNum() { return local_; }
  inline EntitiesNum<label> &realNum() { return real_; }
  inline const EntitiesNum<llabel> &globalNum() const { return global_; }
  inline const EntitiesNum<label> &localNum() const { return local_; }
  inline const EntitiesNum<label> &realNum() const { return real_; }
  inline llabel &globalRow() { return global_.r; }
  inline llabel &globalCol() { return global_.c; }
  inline llabel &globalNnz() { return global_.nnz; }
  inline label &localRow() { return local_.r; }
  inline label &localCol() { return local_.c; }
  inline label &localNnz() { return local_.nnz; }
  inline label &realRow() { return real_.r; }
  inline label &realCol() { return real_.c; }
  inline label &realNnz() { return real_.nnz; }
  inline const llabel &globalRow() const { return global_.r; }
  inline const llabel &globalCol() const { return global_.c; }
  inline const llabel &globalNnz() const { return global_.nnz; }
  inline const label &localRow() const { return local_.r; }
  inline const label &localCol() const { return local_.c; }
  inline const label &localNnz() const { return local_.nnz; }
  inline const label &realRow() const { return real_.r; }
  inline const label &realCol() const { return real_.c; }
  inline const label &realNnz() const { return real_.nnz; }

  /*! * * * * * * * * * * * * * *pure virtual funcs* * * * * * * * * * * * */
  /*! 拓扑排序，实现带宽减小、计算时数据局部集中等目的 */
  virtual int reorderTopo(std::vector<label> &perm) = 0;
  virtual void print(std::ostream &out = *POUT.stream(),
                     int width = 4) const = 0;
  virtual void clear() = 0;
  // virtual std::size_t memoryUsage() const = 0;

};  // end class

}  // namespace HSF

#endif /* end of include guard: ALGORITHM_TOPOLOGY_TOPOLOGY_HPP */
