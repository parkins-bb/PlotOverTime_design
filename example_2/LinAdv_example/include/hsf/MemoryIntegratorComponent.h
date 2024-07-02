#ifndef SOURCE_WRAPPER_HSFMEMORYINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFMEMORYINTEGRATORCOMPONENT_H
/*! \file MemoryIntegratorComponent.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <string>
#include <vector>

#include "PatchLevel.h"
using namespace std;

#include "IntegratorComponent.h"
#include "StandardComponentManager.h"
#include "StandardComponentPatchStrategy.h"

namespace HSF {
namespace algs {

template <int DIM>
class IntegratorComponentManager;

/**
 * @brief 内存构件类 algs::MemoryIntegratorComponent 在非结构网格层上,
 * 申请和释放数据片的内存空间.
 */
template <int DIM>
class MemoryIntegratorComponent : public algs::IntegratorComponent<DIM> {
 public:
  MemoryIntegratorComponent(const string& object_name,
                            StandardComponentPatchStrategy<DIM>* patch_strategy,
                            IntegratorComponentManager<DIM>* manager)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
  }

  virtual ~MemoryIntegratorComponent() {}

  /**
   * @brief 初始化对象.
   * 该函数实现基类 algs::IntegratorComponent 的纯虚成员函数.
   * 该函数注册待填充的数据片,并将通信项注册到通信算法.
   */
  virtual void initialize() {
    d_patch_strategy->initializeComponent(this);
    d_num_register = 0;
  }

  /**
   * @brief 注册待分配内存空间的数据片.
   * @param patch_data_id 输入参数, 整型,表示待注册的数据片的索引号.
   */
  virtual void registerPatchData(const int patch_data_id) {
    d_patch_data_selector_for_memory.insert(patch_data_id);
    d_num_register = d_patch_data_selector_for_memory.size();
  }

  /**
   * @brief 为数据片调度内存空间.
   *
   * @param patch_level 输入参数, 指针, 指向网格层.
   * @param time        输入参数, 双精度型, 表示申请内存的时刻.
   * @note
   * 该构件只为当前没有调度内存的数据片调度内存；
   * 如果数据片已调度内存, 则只修改其时戳.
   */
  void allocatePatchData(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level,
      const double time) {
    Region* mesh =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer())->getRegion();

    for (auto item : d_patch_data_selector_for_memory) {
      mesh->allocateField(item);
      mesh->setFieldTimeStamp(time, item);
    }
  }

  /**
   * @brief 释放数据片内存空间.
   * @param patch_level 输入参数, 指针, 指向网格层.
   */
  void deallocatePatchData(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level) {
    Region* mesh =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer())->getRegion();

    for (auto item : d_patch_data_selector_for_memory) {
      mesh->deallocateField(item);
    }
  }

  /**
   * @brief 为网格层上的数据片设置新的时戳.
   *
   * @param patch_level 输入参数, 指针, 指向网格层.
   * @param time        输入参数, 双精度型, 表示新的时戳.
   */
  void setTime(const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level,
               const double time) {
    Region* mesh =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer())->getRegion();

    for (auto item : d_patch_data_selector_for_memory) {
      mesh->setFieldTimeStamp(time, item);
    }
  }

 private:
  /// @cond DEV

  /** 标准构件网格片策略类对象. */
  algs::StandardComponentPatchStrategy<DIM>* d_patch_strategy;

  /** 数据片选择向量, 存储待调度内存空间的数据片. */
  std::set<int> d_patch_data_selector_for_memory;

  /** 用于检测是否有数据片注册到当前内存构件. */
  int d_num_register;
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFMEMORYINTEGRATORCOMPONENT_H \
        */
