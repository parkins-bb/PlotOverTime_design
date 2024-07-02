/*! \file dataSet.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-01-29
 *  \brief class DataSet
 */
#ifndef ALGORITHM_TOPOLOGY_DATASET_HPP
#define ALGORITHM_TOPOLOGY_DATASET_HPP
#include "utilities.h"

namespace HSF {

/*! 数据位置作用域枚举类型 */
typedef enum {
  Matrix,   /// 在（结果）矩阵上
  Row,      /// 在（结果）矩阵列上
  Col,      /// 在（结果）矩阵行上
  LMatrix,  /// 在左矩阵上
  RMatrix,  /// 在右矩阵上
  Dum,      /// 在矩阵-矩阵运算的湮没维度上
  Para  /// 参数，若干与矩阵位置没有对应关系的基本类型组成的序列
} DataPosi;
/*! 拷贝属性作用域枚举类型 */
typedef enum {
  In,    /// 仅拷入
  Out,   /// 仅拷出
  InOut  /// 拷入拷出
} CopyProp;

template <DataPosi posi, CopyProp copy, typename T>
class DataSet {
 private:
  // 数据属性
  std::vector<T>& data_;
  DataPosi posi_ = posi;
  CopyProp copy_ = copy;

 public:
  /*! 构造接口
   * \param  array 从数组构造一个DataSet对象
   */
  DataSet(std::vector<T>& array) : data_(array) {}
  /*! 析构接口 */
  ~DataSet() {}

  /*! 长度访问接口
   *  \return 返回的DataSet长度
   */
  label getSize() { return data_.size(); }
  /*! 数据位置访问接口
   *  \return 获得DataSet在拓扑中的位置
   */
  DataPosi getPosi() { return posi_; }
  /*! 数组访问接口
   *  \return 返回data数组的引用
   */
  std::vector<T>& getArray() { return data_; }
  /*! 拷贝属性访问接口
   *  \return 获得DataSet的拷贝属性
   */
  CopyProp getCopyProp() { return copy_; }
  /*! 元素访问接口
   * \param  idx 标识数据在DataSet中的顺序
   *  \return 返回一个基本类型的对象
   */
  T& operator[](label idx) { return data_[idx]; }
};

template <DataPosi posi, CopyProp copy, typename T>
vector<T>& getArray(DataSet<posi, copy, T>& data) {
  return data.getArray();
}

} /* End namespace HSF */

#endif /* end of include guard: ALGORITHM_TOPOLOGY_DATASET_HPP */
