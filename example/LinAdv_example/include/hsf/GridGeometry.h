#ifndef SOURCE_WRAPPER_HSFGRIDGEOMETRY_H
#define SOURCE_WRAPPER_HSFGRIDGEOMETRY_H

#include <stdlib.h>

#include <iostream>
#include <string>

#include "Pointer.h"
#include "GridTopology.h"

using namespace std;

namespace HSF {
namespace tbox {
class Database;
}
}  // namespace HSF

namespace HSF {

namespace hier {

/*!
 * @brief 网格几何类 hier::GridGeometry 存储网格的元数据，
 * 包括：
 * 1. 网格生成器。网格层创建重建算法类 GriddingAlgorithm
 *    根据网格生成器提供的子网格数据创建网格片层次结构。
 * 2. 克隆信息。
 * 3. 联邦/邦员信息。
 *
 * @see hier::PatchGeometry
 */

template <int DIM>
class GridGeometry {
 public:
  /*!
   * @brief 构造函数。
   * @param object_name 输入参数，字符串，表示网格几何的名称。
   */
  GridGeometry(const string &object_name,
               HSF::tbox::Pointer<HSF::tbox::Database> db) {
    d_object_name = object_name;
    d_geo_db = db;
  }

  /*!
   * @brief 析构函数。
   */
  virtual ~GridGeometry() {}

  /**
   * @brief 查询网格几何对象的名称。
   */
  const string &getName() const { return d_object_name; }

  HSF::tbox::Pointer<HSF::tbox::Database> getDatabase() { return d_geo_db; }

  /**
   * @brief 查询网格几何是否带自接触。
   */
  // bool hasContact() const {}

  /*!
   * 框架内部表示物理边界实体集id号
   */
  static const int s_physical_boundary_entity_set_id = -1;

  /*!
   * 框架内部表示失效实体集id号
   */
  static const int s_non_active_entity_set_id = -2;
  /// @endcond

 private:
  /*!
   * 用于重启动和输出错误信息的数据库名字。
   */
  string d_object_name;
  HSF::tbox::Pointer<HSF::tbox::Database> d_geo_db;

  // 最大进程数
  // int d_max_number_procs;

  /**
   * 网格几何是否带有自接触。
   * 如果没有自接触，那么通告<node_coord, part_no>可以唯一标识一个顶点.
   */
  // bool d_has_contact;

  /**
   * Federal no.
   */
  // int d_federal_number;
};

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFGRIDGEOMETRY_H */
