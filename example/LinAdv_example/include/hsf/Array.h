//
// 文件名: Array.h
// 软件包: JAUMIN toolbox,用于内存管理.
// 版权  : 2004-2014, 北京应用物理与计算数学研究所
// 版本号: $Revision$
// 修改  : $Date$
// 描述  : 灵敏数组类,  用于定义数组, 可调整大小并防止内存泄漏.
//

#ifndef JAUMIN_TBOX_ARRAY_H
#define JAUMIN_TBOX_ARRAY_H

#include <algorithm>               // for std::min, std::max
#include <boost/shared_array.hpp>  // for boost::shared_array
#include <cassert>
#include <cstdlib>
#include <cstring>  // for bzero, memcpy, etc.
#include <memory>
#include <new>  // for placement new
#include <vector>

#include "Complex.h"
#include "Pointer.h"
#include "utilityExceptions.h"

namespace HSF {
namespace tbox {

/**
 * @brief 灵敏数组类 tbox::Array, 基于引用计数的数组.
 *
 * tbox::Array 是标准数组的替代。与标准数组类似，它可通过 getSize() 获取数组
 * 长度，通过下表访问数组元素，此外还可通过 getPointer() 返回对应的标准数组。
 * 与标准数组不同，tbox::Array 使用引用计数管理数组的赋值。在复制构造或赋值
 * 时，目标对象和源对象共享同样的数组内存空间，引用计数+1。tbox::Array析构时
 * 将检查引用计数，当且仅当引用计数为0时删除数组内存空间。
 *
 * 灵敏数组可通过内存池调整大小，并可防止内存泄漏, 所以建议用户使用灵敏数组,
 * 方法如下:
 * - 调整数组大小: resizeArray() .
 * - 获取数组元素个数: size(), getSize() .
 * - 获取数组首地址: getPointer()  .
 * - 获取第i个数组元素: data_array[i] .
 *
 * @note
 * 为了降低灵敏数组初始化开销, 针对 POD 类型, 部分构造函数以及 resizeArray 函
 * 数不对数组进行值初始化. 用户必须显式给数组元素赋值或调用初始化构造函数. 这
 * 和STL容器的行为不同(STL容器针对任意类型进行默认初始化). 针对非POD类型, 所有
 * 函数都进行默认初始化.
 *
 * @note
 * - 在JAUMIN中, 通常使用灵敏数组, 并将其简称为数组.
 * - 由于resizeArray()需要将原数组的数据复制给调整后的新数组, 开销很大, 所以
 *   请用户慎用该函数.
 *
 * @see tbox::Pointer
 */

template <typename T>
class Array {
 public:
  /// @name 构造与析构
  /// @{

  /**
   * @brief 默认构造函数, 创建一个空数组.
   */
  Array() : d_nelems(0), d_array() {}

  /**
   * @brief 构造函数, 创建一个包含n个元素的数组.
   * @param n 输入参数, 整型, 数组元素的个数.
   * @param standard_type 输入参数, 逻辑型,
   *   真值表示T是标准的内置数据类型, 缺省为假.
   *
   * @note 当前实现通过 type_traits 判断是否为内置数据类型，standard_type
   *  参数不起作用，保留它的目的是和原接口兼容.
   */
  Array(int n, bool standard_type = false)
      : d_nelems(n), d_array(new std::vector<T>(n)) {
    ASSERT(n >= 0);
  }

  /**
   * @brief 构造函数, 创建一个包含n个元素的数组并值初始化.
   * @param n 输入参数, 整型, 数组元素的个数.
   * @param val 输入参数，T型，表示初始化值
   * @param standard_type 输入参数, 逻辑型,
   *   真值表示T是标准的内置数据类型, 缺省为假.
   *
   * @note 当前实现通过 type_traits 判断是否为内置数据类型，standard_type
   *  参数不起作用，保留它的目的是和原接口兼容.
   */
  Array(int n, const T& val, bool standard_type = false)
      : d_nelems(n), d_array(new std::vector<T>(n, val)) {
    ASSERT(n >= 0);
  }

  /**
   * @brief 复制构造函数.
   * @param array 输入参数, 源数组
   * @note 新数组和源数组共享内存空间和引用计数，引用计数+1
   */
  Array(const Array<T>& array)
      : d_nelems(array.d_nelems),
        d_array(array.d_array),
        shadow(array.shadow),
        shadow_array(array.shadow_array) {}

  /*! 浅拷贝 */
  Array(const std::vector<T>& vec)
      : d_nelems(vec.size()), d_array(), shadow(true) {
    shadow_array = const_cast<T*>(vec.data());
  }

  /**
   * @brief 析构函数.
   */
  ~Array() {}

  /**
   * @brief 赋值运算符
   */
  Array& operator=(const Array<T>& r) {
    d_nelems = r.d_nelems;
    d_array = r.d_array;
    shadow = r.shadow;
    shadow_array = r.shadow_array;
    return *this;
  }

  /// @}
  /// @name 常用操作
  /// @{

  /**
   * @brief 调整数组大小.
   * @param n 输入参数, 整型, 表示调整后的数组元素个数.
   * @note 假设存在一个包含m个元素的原灵敏数组,
   *   通过调用函数 resizeArray(n) 可将其调整为一个包含n个元素的新灵敏数组.
   *   此时, 如果m小于n, 将原灵敏数组所有元素的值依次复制给新灵敏数组前m个元素.
   *   如果m大于等于n, 将原灵敏数组前n个元素的值依次复制给新灵敏数组.
   */
  void resizeArray(int n) {
    ASSERT(n >= 0);
    d_nelems = n;
    if (n == 0) {
      d_array.reset();
      return;
    }

    if (d_array) {
      (*d_array).resize(n);
    } else {
      d_array.reset(new std::vector<T>(n));
    }
  }

  /**
   * @brief 获取数组元素个数.
   * @return 整型, 数组元素个数.
   */
  int getSize() const { return d_nelems; }

  /**
   * @brief 设置数组为空.
   */
  void setNull() {
    d_nelems = 0;
    d_array.reset();
  }

  /**
   * @brief 判断数组是否为空.
   * @return 逻辑型, 真值表示数组为空.
   */
  bool isNull() const { return !d_array; }

  /**
   * @brief 获取指针, 指向数组的第i个元素.
   * @param i 输入参数, 整型, 数组元素索引号.
   * @return 指针, 指向数组的第i个元素,
   *  其中i的取值范围为[0,N-1], N为数组元素个数.
   */
  T* getPointer(int i = 0) const {
    ASSERT(i >= 0 && i < d_nelems);
    ASSERT(d_array);
    if (!shadow)
      return &((*d_array)[i]);
    else
      return shadow_array;
  }

  /**
   * @brief 获取只读指针, 指向数组的第i个元素.
   * @param i 输入参数, 整型, 数组元素索引号.
   * @return 指针, 指向数组的第i个元素,
   *  其中i的取值范围为[0,N-1], N为数组元素个数.
   */
  const T* getConstPointer(int i = 0) const {
    ASSERT(i >= 0 && i < d_nelems);
    ASSERT(d_array);
    if (!shadow)
      return &((*d_array)[i]);
    else
      return shadow_array;
  }

  /// @}
  /// @name 容器接口
  /// @{

  /**
   * @brief 获取数组元素个数.
   * @return 整型, 数组元素个数.
   */
  int size() const { return d_nelems; }

  /**
   * @brief 下标运算符
   * @param i 整数，数组下标，必须在0-size()-1之间
   * @return 对应元素的引用
   */
  T& operator[](int i) const {
    ASSERT(i >= 0 && i < d_nelems);
    ASSERT(d_array);
    if (!shadow)
      return ((*d_array)[i]);
    else
      return shadow_array[i];
  }

  /// @}

 private:
  int d_nelems;
  std::shared_ptr<std::vector<T> > d_array;
  bool shadow = false;
  T* shadow_array = NULL;
};  // class Array

template <>
class Array<bool> {
 public:
  Array() : d_nelems(0), d_array() {}
  Array(int n, bool standard_type = false) : d_nelems(n), d_array() {
    ASSERT(n >= 0);
    d_array.reset(new bool[n]);
  }
  Array(int n, const bool& val, bool standard_type = false)
      : d_nelems(n), d_array() {
    ASSERT(n >= 0);
    d_array.reset(new bool[n]);
    for (int i = 0; i < n; i++) d_array[n] = val;
  }
  Array(const Array<bool>& array)
      : d_nelems(array.d_nelems), d_array(array.d_array) {}
  ~Array() {}
  Array& operator=(const Array<bool>& r) {
    d_nelems = r.d_nelems;
    d_array = r.d_array;
    return *this;
  }
  void resizeArray(int n) {
    ASSERT(n >= 0);
    const int min_val = std::min(n, d_nelems);
    d_nelems = n;
    if (n == 0) {
      d_array.reset();
      return;
    }

    if (d_array) {
      bool* buf = new bool[n];
      for (int i = 0; i < min_val; ++i) buf[i] = d_array[i];
      (d_array).reset(buf);
    } else {
      (d_array).reset(new bool[n]);
    }
  }

  int getSize() const { return d_nelems; }

  void setNull() {
    d_nelems = 0;
    d_array.reset();
  }

  bool isNull() const { return !d_array; }
  bool* getPointer(int i = 0) const {
    ASSERT(i >= 0 && i < d_nelems);
    ASSERT(d_array);
    return &((d_array)[i]);
  }
  const bool* getConstPointer(int i = 0) const {
    ASSERT(i >= 0 && i < d_nelems);
    ASSERT(d_array);
    return &((d_array)[i]);
  }
  int size() const { return d_nelems; }
  bool& operator[](int i) const {
    ASSERT(i >= 0 && i < d_nelems);
    ASSERT(d_array);
    return (d_array[i]);
  }

 private:
  int d_nelems;
  boost::shared_array<bool> d_array;

};  // class Array

}  // namespace tbox
}  // namespace HSF

#endif
