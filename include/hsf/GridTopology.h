#ifndef SOURCE_WRAPPER_HSFGRIDTOPOLOGY_H
#define SOURCE_WRAPPER_HSFGRIDTOPOLOGY_H
/*! \file GridTopology.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <string>

#include "Pointer.h"
using namespace std;

namespace HSF {
namespace tbox {
class Database;
}
}  // namespace HSF

namespace HSF {
namespace hier {

/**
 * @brief 网格拓扑类，管理共性网格拓扑信息，主要包括：
 *   - 影像区类型；
 *   - 网格单元类型。
 *
 */
template <int DIM>
class GridTopology {
 public:
  /**
   * @brief 构造函数。
   *
   * @param object_name 输入参数, 字符串, 表示该对象的名字。
   * @param input_db    输入参数, 指针, 指向输入数据库, 参数值从该数据库读取。
   */
  GridTopology(const string& object_name,
               HSF::tbox::Pointer<HSF::tbox::Database> db) {
    d_object_name = object_name;
    d_topo_db = db;
  }

  HSF::tbox::Pointer<HSF::tbox::Database> getDatabase() { return d_topo_db; }

  /**
   * 析构函数。
   */
  ~GridTopology() {}

  /// @cond DEV

 private:
  /*!
   用于重启动和输出错误信息的数据库名字。
   */
  string d_object_name;
  HSF::tbox::Pointer<HSF::tbox::Database> d_topo_db;
};

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFGRIDTOPOLOGY_H */
