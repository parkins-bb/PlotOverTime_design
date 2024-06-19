#ifndef SOURCE_WRAPPER_HSFPATCHLEVEL_H
#define SOURCE_WRAPPER_HSFPATCHLEVEL_H
/*! \file PatchLevel.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include "Pointer.h"
// #include "Patch.h"
#include <vector>

// #include "GridGeometry.h"
// #include "GridTopology.h"
#include "Patch.h"
#include "PatchGeometry.h"
#include "PatchHierarchy.h"
#include "PatchTopology.h"
#include "utilityExceptions.h"

namespace HSF {

namespace hier {

template <int DIM>
class BasePatchLevel {
 public:
  BasePatchLevel() {}

  virtual ~BasePatchLevel() {}

  virtual int getNumberOfLocalPatches() const { return 1; }
  virtual int getNumberOfGhostPatches() const { return 1; }

  /**
   * @brief 在该网格层上，将指定数据片的时戳设置为指定的时刻。
   * @param timestamp 输入参数，双精度浮点型，指定时刻。
   * @param id        输入参数，整型，数据片索引号。
   */
  virtual void setTime(const double timestamp, const int id) {}

  virtual void setTime(const double timestamp) {}
};

template <int DIM>
class PatchLevel : public hier::BasePatchLevel<DIM> {
 public:
  /*!
   * @brief 构造函数，创建空网格层。
   * @param grid_geometry 输入参数，指针，指向网格几何类对象。
   * @param grid_topology 输入参数，指针，指向网格拓扑类对象。
   *
   * 断言检查:
   *  -# 网格几何指针(grid_geometry)非空。
   *  -# 网格拓扑指针(grid_topology)非空。
   */
  PatchLevel(PatchHierarchy<DIM>* ihy) {
    //USTRMesh* mesh = ihy->getMesh();
    d_reg = ihy->getRegion();
    HSF::tbox::Pointer<PatchGeometry<DIM> > patch_geometry =
        new PatchGeometry<DIM>(d_reg);
    HSF::tbox::Pointer<PatchTopology<DIM> > patch_topology =
        new PatchTopology<DIM>(d_reg);
    HSF::tbox::Pointer<Patch<DIM> > patch =
        new Patch<DIM>(patch_geometry, patch_topology);
    d_patches.push_back(patch);
    d_number_local_patches = 1;
    d_number_global_patches = 1;
  }

  /**
   * @brief 析构函数。
   */
  virtual ~PatchLevel() {}

  /**
   * @brief 获取该网格层中编号为 <i>p</i> 的网格片。
   * @param p 输入参数，整型，网格片编号。
   * @return 指针，指向该网格层中编号为 <i>p</i> 的网格片。
   * @note 参数中的p表示网格片的全局编号。
   */
  HSF::tbox::Pointer<Patch<DIM> > getPatch(const int p) const {
    ASSERT(p < d_number_local_patches) return d_patches[p];
  }

  Region* getRegion() { return d_reg; }

 private:
  std::vector<HSF::tbox::Pointer<Patch<DIM> > > d_patches;
  Region* d_reg;
  int d_number_global_patches;
  int d_number_local_patches;
};

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFPATCHLEVEL_H */
