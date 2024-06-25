#ifndef SOURCE_WRAPPER_HSFARRAYDATA_H
#define SOURCE_WRAPPER_HSFARRAYDATA_H
/*! \file ArrayData.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */
#include <iostream>
#include <vector>
namespace HSF {

namespace pdat {

/*!
 * @brief 数组量类 ArrayData<DIM, TYPE> 是定义在某个索引范围的模版数组，
 * 支持各种数据片派生类。
 *
 * 数据以列主序存储（Fortran风格: (g, i, d)），g表示分量，i表示索引，d表示深度。
 *
 * 数据类型TYPE为标准数据类型(bool, char, double, float, int)
 *
 */

template <int DIM, class TYPE>
class ArrayData {
 public:
  /*!
   * @brief 返回指定分量、索引范围和深度数的数组的大小。
   *
   * @param group  输入参数，整型，表示分量数。
   * @param entity 输入参数，整型，网格实体数。
   * @param depth  输入参数，整型，表示深度数。
   *
   * @return size_t 数值量的大小(字节为单位)。
   */
  // std::size_t getSizeOfData(const int group, const int entity, const int
  // depth);

  ///@name 构造和初始化函数。
  //@{

  /*!
   * @brief 空构造函数。
   */
  ArrayData() : d_group(0), d_entity(0), d_depth(0), d_offset(0) {}

  /*!
   * @brief 依据指定网格实体中心坐标对象、分量、深度和影像区宽度创建一个数组量。
   *
   * @param group  输入参数，整型，表示分量。
   * @param eneity 输入参数，整型，表示网格实体数。
   * @param depth  输入参数，整型，表示深度。
   */
  ArrayData(const int group, const int entity, const int depth)
      : d_group(group),
        d_entity(entity),
        d_depth(depth),
        d_offset(group * entity) {
    initializeArray(group, entity, depth);
  }

  /*!
   * @brief 复制构造函数。
   */
  // ArrayData(const ArrayData<DIM, TYPE>& array_data){}

  /*!
   * @brief 析构函数，释放数组量的内存空间。
   */
  ~ArrayData() {}

  /*!
   * @brief
   *依据指定的网格实体中心坐标对象，深度和影像区宽度初始化数组量（分配内存空间）。
   *
   * @param group  输入参数，整型，表示分量。
   * @param eneity 输入参数，整型，表示网格实体数。
   * @param depth  输入参数，整型，表示深度。
   * @param pool   输入参数，指针，指向内存池。
   */
  void initializeArray(const int group, const int entity, const int depth) {
    d_array.resize(group * entity * depth);
  }

  /*!
   * @brief 查询数组量是否已经被初始化(分配内存空间)。
   */
  bool isInitialized() const { return (!d_array.empty()); }

  /*!
   * @brief 返回数组量的分量数。
   *
   * @return 整型，数组量的分量数。
   */
  int getGroup() const { return d_group; }

  /*!
   * @brief 返回数组量的网格实体数。
   *
   * @return 整型，数组量的网格实体数。
   */
  int getNumberOfEntity() const { return d_entity; }

  /*!
   * @brief 返回数组量的深度。
   *
   * @return 整型，数组量的深度。
   */
  int getDepth() const { return d_depth; }

  /*!
   * @brief 返回数组量的偏移量(即：每个深度的数据个数)。
   *
   * @return 整型，数组量的偏移量。
   */
  int getOffset() const { return d_offset; }

  /*!
   * @brief 返回指针，指向指定深度的数组的首地址。
   *
   * @param d 输入参数，整型，深度。
   *
   * @return 指针，指向指定深度的数组的首地址。
   */
  TYPE* getPointer(const int d = 0) {
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((d >= 0) && (d < d_depth));
#endif
    return (&d_array[d * d_offset]);
  }

  /*!
   * @brief 返回常量指针，指向指定深度的数组的首地址。
   *
   * @param d 输入参数，整型，深度。
   *
   * @return 指针，指向指定深度的数组的首地址。
   */
  const TYPE* getPointer(const int d = 0) const {
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((d >= 0) && (d < d_depth));
#endif
    return (&d_array[d * d_offset]);
  }

  /*!
   * @brief 重载算子：将参数指定的数组赋值给当前类对象。
   *
   * @param rhs 输入参数，数组量类型，待赋值数组。
   *
   * @return 数组量型引用。
   */
  ArrayData<DIM, TYPE>& operator=(const ArrayData<DIM, TYPE>& rhs);

  /*!
   * @brief 重载算子：获得指定分量、索引和深度的分量的引用。
   *
   * @param g 输入参数，整型，分量。
   * @param i 输入参数，整型，索引。
   * @param d 输入参数，整型，深度。
   *
   * @return TYPE型引用，指定分量、索引和深度位置的分量的引用。
   */
  TYPE& operator()(const int g, const int i, const int d = 0) {
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((g >= 0) && (g < d_group));
    TBOX_ASSERT((d >= 0) && (d < d_depth));
#endif
    const int index = g + i * d_group + d * d_offset;
    if (!(index < d_depth * d_offset)) {
      std::cout << "index = " << index << " g = " << g << " i = " << i
                << " d = " << d << std::endl;
      std::cout << "   d_group = " << d_group << std::endl;
      std::cout << "   d_entity = " << d_entity << std::endl;
      std::cout << "   d_depth = " << d_depth << std::endl;
      std::cout << "   d_offset = " << d_offset << std::endl;
    }
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((index >= 0) && (index < d_depth * d_offset));
#endif
    return (d_array[index]);
  }

  /*!
   * @brief 重载算子：获得指定分量、索引和深度的分量的常引用。
   *
   * @param g 输入参数，整型，分量。
   * @param i 输入参数，整型，索引。
   * @param d 输入参数，整型，深度。
   *
   * @return TYPE型常引用，指定分量、索引和深度位置的分量的常引用。
   */
  const TYPE& operator()(const int g, const int i, const int d) const {
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((g >= 0) && (g < d_group));
    TBOX_ASSERT((d >= 0) && (d < d_depth));
#endif
    const int index = g + i * d_group + d * d_offset;
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((index >= 0) && (index < d_depth * d_offset));
#endif
    return (d_array[index]);
  }

  /*!
   * @brief 设置数组量的值为无效值(例如:NaN)。
   */
  // void undefineData(){}

  /*!
   * @brief 将数组量中所有数据赋予同一个值。
   * @param t 输入参数，TYPE型，表示值。
   */
  void fillAll(const TYPE& t) { d_array = t; }

  /*!
   * @brief 将数组量中指定深度的所有数据赋予同一个值。
   * @param t   输入参数，TYPE型，表示值。
   * @param d   输入参数，整型，表示深度，缺省等于0。
   */
  void fill(const TYPE& t, const int d = 0) {
    for (std::size_t i = 0; i < d_offset; i++) {
      d_array[i + d * d_offset] = t;
    }
  }

  /**
   * @brief 将参数“t”赋值给数据片的某一元素.
   *
   * @param t 输入参数, 无类型指针, 指向修改值.
   * @param g 输入参数, 整型, 待赋值元素的分量索引.
   * @param i 输入参数, 整型, 待赋值元素对应的网格实体的索引.
   * @param d 输入参数, 整型, 待赋值元素的深度索引.
   *
   * @note 分量索引、网格实体索引、深度索引唯一确定数据片的一个元素.
   */
  void fill(const void* t, const int g, const int i, const int d) {
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((g >= 0) && (g < d_group));
    TBOX_ASSERT((d >= 0) && (d < d_depth));
#endif
    const int index = g + i * d_group + d * d_offset;
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((index >= 0) && (index < d_depth * d_offset));
#endif
    d_array[index] = *(TYPE*)t;
  }

  /**
   * @brief 将参数“t”累加到数据片的某一元素.
   *
   * @param t 输入参数, 无类型指针, 指向累加值.
   * @param g 输入参数, 整型, 待累加元素的分量索引.
   * @param i 输入参数, 整型, 待累加元素对应的网格实体的索引.
   * @param d 输入参数, 整型, 待累加元素的深度索引.
   *
   * @note 深度, 索引, 组分数唯一确定数据片的一个位置.
   */
  void add(const void* t, const int g, const int i, const int d) {
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((g >= 0) && (g < d_group));
    TBOX_ASSERT((d >= 0) && (d < d_depth));
#endif
    const int index = g + i * d_group + d * d_offset;
#ifdef DEBUG_CHECK_ASSERTIONS
    TBOX_ASSERT((index >= 0) && (index < d_depth * d_offset));
#endif
    d_array[index] += *(TYPE*)t;
  }

 private:
  int d_group;   /**< 数据片分量 */
  int d_entity;  /**< 数据片网格实体数 */
  int d_depth;   /**< 数据片深度 */
  int d_offset;  // 数组d_array按列主序存储，d_offset表示同一几何位置上不同深度变量值在内存中的偏移量，
  std::vector<TYPE> d_array; /**< 数据存储数组 */
};                           // namespace pdat
}  // namespace pdat
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFARRAYDATA_H */
