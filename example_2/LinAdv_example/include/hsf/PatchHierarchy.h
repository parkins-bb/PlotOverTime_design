#ifndef SOURCE_WRAPPER_HSFPATCHHIERARCHY_H
#define SOURCE_WRAPPER_HSFPATCHHIERARCHY_H
/*! \file PatchHierarchy.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <string>
#include <vector>

#include "Pointer.h"
#include "BasePatchHierarchy.h"
#include "hsfRegion.hpp"
#include "VariableDatabase.h"
#include "ustrMesh.hpp"

using namespace std;

namespace HSF {
namespace hier {

template <int>
class GridGeometry;
template <int>
class GridTopology;
template <int>
class PatchLevel;

/**
 * @brief 网格片层次结构类
 *hier::PatchHierarchy，以网格层为单位，管理非结构网格。
 *
 * 用户创建该类对象，需提供如下类的对象：
 * - hier::GridGeometry: 网格几何类。
 * - hier::GridTopology: 网格拓扑类。
 *
 * @see hier::PatchLevel
 */

template <int DIM>
class PatchHierarchy : public BasePatchHierarchy<DIM> {
 public:
  /**
   * @brief 构造函数。
   */
  PatchHierarchy() {}

  /**
   * @brief 构造函数。
   *
   * @param object_name          输入参数，字符串，对象名。
   * @param grid_geometry        输入参数，指针，指向网格几何类对象。
   * @param grid_topology        输入参数，指针，指向网格拓扑类对象。
   * @param register_for_restart
   *输入参数，逻辑型，真值表示将该网格片层次结构注册到重启动管理器。
   * @param federal_number       输入参数，整型，联邦计算时表示邦员号。
   * @param federation_comm_sets
   *输入参数，指针，联邦计算时非空，指向联邦通信器集合。
   *
   * @note
   * 主要完成操作:
   *  -# 初始化网格层数目为0。
   *  -# 设置几何信息。
   *  -# 当参数 <i>register_for_restart</i>为真时,
   *     将该网格片层次结构注册到重启动管理器。 @if DEV
   *  -# 创建网格层及网格片工厂对象。@endif
   *
   * 断言检查:
   *  -# 字符串object_name非空。
   *  -# 网格几何指针grid_geometry非空。
   *  -# 网格拓扑指针grid_topology非空。
   */
  PatchHierarchy(const string &object_name,
                 HSF::tbox::Pointer<GridGeometry<DIM> > grid_geometry,
                 HSF::tbox::Pointer<GridTopology<DIM> > grid_topology,
                 bool register_for_restart = true);

  ~PatchHierarchy() {
    if (d_mesh) delete d_mesh;
    if (d_reg) delete d_reg;
  }

  /**
   * @brief 获取网格几何对象。
   * @return 指针，指向网格几何对象。
   */
  // HSF::tbox::Pointer<GridGeometry<DIM> > getGridGeometry() const {}

  /**
   * @brief 在该网格片层次结构中，判断是否存指定的网格层。
   * @param level_index 输入参数，整型，网格层层号。
   * @return 逻辑型，真值表示存在第 <i>level_index</i> 个网格层。
   */
  // bool levelExists(const int level_index = 0) const {}

  /**
   * @brief 获取该网格片层次结构中的网格层总数。
   * @return 整型，网格层总数。
   */
  int getNumberOfLevels() const { return d_number_levels; }

  USTRMesh *getMesh() {
    d_mesh = dynamic_cast<USTRMesh *>(d_reg->getMesh());
    return d_mesh;
  }
  Region *getRegion() { return d_reg; }

  HSF::tbox::Pointer<PatchLevel<DIM> > getPatchLevel(int index) {
    ASSERT(index < d_number_levels)
    return d_patch_levels[index];
  }
  HSF::tbox::Pointer<GridTopology<DIM> > getTopology() {
    return d_grid_topology;
  }
  HSF::tbox::Pointer<GridGeometry<DIM> > getGeometry() {
    return d_grid_geometry;
  }

 private:
  string d_object_name; /*!< 该网格片层次结构的名字 */
  bool d_registered_for_restart; /*!< 表示该网格片层次结构是否用于重启动 */
  int d_number_levels; /*!< 该网格片层次结构中的网格层总数 */

  std::vector<HSF::tbox::Pointer<PatchLevel<DIM> > >
      d_patch_levels; /*!< 指针数组，每个指针指向本网格片层次结构中的一个网格层
                       */
  HSF::tbox::Pointer<GridGeometry<DIM> >
      d_grid_geometry; /*!< 指针，指向网格几何对象 */

  HSF::tbox::Pointer<GridTopology<DIM> >
      d_grid_topology; /*!< 指针，指向网格拓扑对象 */

  USTRMesh *d_mesh;
  Region *d_reg;
};
}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFPATCHHIERARCHY_H */
