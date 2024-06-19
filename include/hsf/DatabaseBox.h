// 文件名:    DatabaseBox.h
// 软件包:    JAUMIN toolbox
// 版权  :	(c) 2004-2010 北京应用物理与计算数学研究所
// 版本号:    $Revision: 257 $
// 修改  :    $Date: 2012-02-16 12:55:07 +0800 (四, 2012-02-16) $
// 描述  :  数据库box类, 用于在数据库中存储box.
//

#ifndef included_tbox_DatabaseBox
#define included_tbox_DatabaseBox

#ifndef included_JAUMIN_config
#include "JAUMIN_config.h"
#endif

#ifdef DEBUG_CHECK_ASSERTIONS
#ifndef included_assert
#define included_assert
#include <assert.h>
#endif
#endif

#ifndef DatabaseBox_MAX_DIM
#define DatabaseBox_MAX_DIM 3
#else
Macro overloaded : DatabaseBox_MAX_DIM
#endif

namespace HSF {
namespace tbox {

/*!
 * @brief 结构体 DatabaseBox_POD 封装了类
 *  tbox::DatabaseBox 的所有数据成员以便于HDF5使用.
 */
struct DatabaseBox_POD {
  int d_dimension;
  int d_lo[DatabaseBox_MAX_DIM];
  int d_hi[DatabaseBox_MAX_DIM];
};

/**
 * @brief 数据库box类 tbox::DatabaseBox, 用于在数据库中存储box.
 *
 * 由于数据库中不能直接支持JAUMIN中的box类型,
 * 因此定义了一个数据库可直接操作的复合类型DatabaseBox_POD.
 * 于是, 将box存入数据库的步骤为:
 * -# 调用 hier::Box<DIM>::DatabaseBox_from_Box() 将box转换成类
 *tbox::DatabaseBox 的对象.
 * -# 调用 tbox::Database::putDatabaseBox()将类 tbox::DatabaseBox
 *的对象存入数据库.
 *
 * 从数据库中读取box的步骤为:
 *  -# 调用 tbox::Database::getDatabaseBox() 从数据库中读取类 tbox::DatabaseBox
 *的对象.
 *  -# 调用 hier::Box<DIM>::set_Box_from_DatabaseBox 将类 tbox::DatabaseBox
 *的对象转换成box.
 *
 * @see hier::Index
 * @see tbox::Database
 */

class DatabaseBox {
 public:
  /**
   * @brief 缺省构造函数.
   */
  DatabaseBox() {
    d_data.d_dimension = 0;
    d_data.d_lo[0] = d_data.d_hi[0] = 0;
    d_data.d_lo[1] = d_data.d_hi[1] = 0;
    d_data.d_lo[2] = d_data.d_hi[2] = 0;
  }

  /**
   * @brief 构造函数.
   *
   * @param dimension 输入参数, 整型, 表示box的维数, 其取值范围为[0, 3].
   * @param lower 输入参数, 指针, 指向box的索引下界.
   * @param upper 输入参数, 指针, 指向box的索引上界.
   */
  DatabaseBox(const int dimension, const int* lower, const int* upper) {
    d_data.d_dimension = dimension;
    d_data.d_lo[0] = lower[0];
    d_data.d_lo[1] = lower[1];
    d_data.d_lo[2] = lower[2];
    d_data.d_hi[0] = upper[0];
    d_data.d_hi[1] = upper[1];
    d_data.d_hi[2] = upper[2];
  }

  /**
   * @brief 复制构造函数.
   *
   * @param box 输入参数, 数据库box, 输入的数据库box.
   */
  DatabaseBox(const DatabaseBox& box) {
    d_data.d_dimension = box.d_data.d_dimension;
    d_data.d_lo[0] = box.d_data.d_lo[0];
    d_data.d_lo[1] = box.d_data.d_lo[1];
    d_data.d_lo[2] = box.d_data.d_lo[2];
    d_data.d_hi[0] = box.d_data.d_hi[0];
    d_data.d_hi[1] = box.d_data.d_hi[1];
    d_data.d_hi[2] = box.d_data.d_hi[2];
  }

  /**
   * @brief 重载"="运算符.
   *
   * @param box 输入参数, 数据库box类, 输入的数据库box.
   */
  DatabaseBox& operator=(const DatabaseBox& box) {
    d_data.d_dimension = box.d_data.d_dimension;
    d_data.d_lo[0] = box.d_data.d_lo[0];
    d_data.d_lo[1] = box.d_data.d_lo[1];
    d_data.d_lo[2] = box.d_data.d_lo[2];
    d_data.d_hi[0] = box.d_data.d_hi[0];
    d_data.d_hi[1] = box.d_data.d_hi[1];
    d_data.d_hi[2] = box.d_data.d_hi[2];
    return (*this);
  }

  /**
   * @brief 析构函数.
   */
  ~DatabaseBox() {}

  /**
   * @brief 判断该数据库box是否为空.
   *
   * @return 逻辑型, 真值表示box非空.
   *
   * @note
   * 如果box的维数是0或者某维上的索引上界小于下界, 那么box为空.
   */
  bool empty() const {
    if (d_data.d_dimension == 0) {
      return true;
    } else {
      for (int i = 0; i < d_data.d_dimension; i++) {
        if (d_data.d_lo[i] > d_data.d_hi[i]) return true;
      }
    }
    return false;
  }

  /**
   * @brief 返回box的维数.
   *
   * @return 整型, 表示box的维数.
   */
  int getDimension() const { return (d_data.d_dimension); }

  /**
   * @brief 设置box的维数.
   *
   * @param dimension 输入参数, 整型, 表示box的维数.
   */
  void setDimension(const int dimension) {
#ifdef DEBUG_CHECK_ASSERTIONS
    assert((dimension >= 0) && (dimension <= DatabaseBox_MAX_DIM));
#endif
    d_data.d_dimension = dimension;
  }

  /**
   * @brief 返回box第i维索引下界.
   * @param i 输入参数, 整型, 表示指定的维.
   * @return 整型, 表示box第i维索引下界.
   */
  int& lower(const int i) {
#ifdef DEBUG_CHECK_ASSERTIONS
    assert((i >= 0) && (i < d_data.d_dimension));
#endif
    return (d_data.d_lo[i]);
  }

  /**
   * @brief 返回box第i维索引上界.
   * @param i 输入参数, 整型, 表示指定的维.
   * @return 整型, 表示box第i维索引上界.
   */
  int& upper(const int i) {
#ifdef DEBUG_CHECK_ASSERTIONS
    assert((i >= 0) && (i < d_data.d_dimension));
#endif
    return (d_data.d_hi[i]);
  }

  /**
   * @brief 返回box第i维索引下界.
   * @param i 输入参数, 整型, 表示指定的维.
   * @return 整型, 表示box第i维索引下界.
   */
  int lower(const int i) const {
#ifdef DEBUG_CHECK_ASSERTIONS
    assert((i >= 0) && (i < d_data.d_dimension));
#endif
    return (d_data.d_lo[i]);
  }

  /**
   * @brief 返回box第i维索引上界.
   * @param i 输入参数, 整型, 表示指定的维.
   * @return 整型, 表示box第i维索引上界.
   */
  int upper(const int i) const {
#ifdef DEBUG_CHECK_ASSERTIONS
    assert((i >= 0) && (i < d_data.d_dimension));
#endif
    return (d_data.d_hi[i]);
  }

  /**
   * @brief 判断两个box是否描述同一索引子空间.
   *
   * @return 整型, 值1表示两个box相等, 否则不相等.
   */
  int operator==(const DatabaseBox& box) const {
    if (d_data.d_dimension != box.getDimension()) {
      return 0;
    } else {
      for (int i = 0; i < d_data.d_dimension; i++) {
        if (d_data.d_lo[i] != box.lower(i) || d_data.d_hi[i] != box.upper(i))
          return 0;
      }
    }
    return 1;
  }

  /**
   * @brief 存储该数据库box的维数, 索引下界和索引上界.
   *
   * @note
   * - 该数据成员属性为public,
   *   目的是让类 tbox::HDFDatabase
   *   在为HDF5定义复合类型时无须镜像该数据结构.
   */
  DatabaseBox_POD d_data;
};

}  // namespace tbox
}  // namespace HSF

#endif
