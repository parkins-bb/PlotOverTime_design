#ifndef SOURCE_WRAPPER_HSFENTITYUTILITIES_H
#define SOURCE_WRAPPER_HSFENTITYUTILITIES_H
/*! \file EntityUtilities.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include "connectivity.hpp"
namespace HSF {
namespace hier {

/*!
 * @brief 该类为网格实体及其相关类定义枚举量。
 */
class EntityUtilities {
 public:
  /*!
   * @brief 网格实体类型。
   */
  enum EntityType {
    CELL = 0,  // 单元。
    NODE = 1,  // 结点。
    EDGE = 2,  // 边。
    FACE = 3,  // 面。
    INVALID_ENTITY = -1
  };

  /*!
   * @brief 数据片类型。
   */
  enum PatchDataType {
    CELL_DATA = 0,        // 中心量数据片。
    NODE_DATA = 1,        // 结点量数据片。
    EDGE_DATA = 2,        // 边量数据片。
    FACE_DATA = 3,        // 面量数据片。
    OUTER_NODE_DATA = 4,  // 网格片外表面结点量数据片。
    OUTER_EDGE_DATA = 5,  // 网格片外表面边量数据片。
    OUTER_FACE_DATA = 6,  // 网格片外表面面量数据片。
    INVALID_DATA = -1
  };

  static SetType entity2setType(EntityUtilities::EntityType t) {
    switch (t) {
      case (CELL):
        return SetType::CELL;
      case (NODE):
        return SetType::NODE;
      case (EDGE):
        return SetType::EDGE;
      case (FACE):
        return SetType::FACE;
      case (INVALID_ENTITY):
      default:
        return SetType::NODE;
    }
  }
};

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFENTITYUTILITIES_H */
