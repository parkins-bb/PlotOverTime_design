#ifndef SOURCE_WRAPPER_HSFINITIALIZEINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFINITIALIZEINTEGRATORCOMPONENT_H

#include <string>
#include <vector>

#include "PatchLevel.h"
using namespace std;

#include "IntegratorComponent.h"
#include "StandardComponentManager.h"
#include "StandardComponentPatchStrategy.h"

namespace HSF {
namespace algs {

/**
 * 该构件类使用方法如下:
 * -# 调用构造函数，创建初值构件. \n
 *    创建时, 需要提供一个标准构件网格片策略类
 *algs::StandardComponentPatchStrategy 的派生类对象.
 * -# 向初值构件注册待赋初值的数据片. \n
 *    在派生类中实现函数
 *algs::StandardComponentPatchStrategy::initializeComponent() 时,
 *    调用该构件的函数 registerInitPatchData(), 注册待赋值的数据片.
 * -# 为网格层的数据片赋初值. \n
 *    调用该构件的函数 initializeLevelData(), 在新网格层上,
 *    为注册的数据片申请内存空间并赋值(请注意: 不含数据片的影像区);
 *    然后, 遍历网格层的每个网格片, 调用函数
 *    algs::StandardComponentPatchStrategy::initializePatchData(), 赋初值.
 */

template <int DIM>
class InitializeIntegratorComponent : public algs::IntegratorComponent<DIM> {
 public:
  InitializeIntegratorComponent(
      const string& object_name,
      StandardComponentPatchStrategy<DIM>* patch_strategy,
      algs::IntegratorComponentManager<DIM>* manager)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
  }

  virtual ~InitializeIntegratorComponent() {}

  virtual void initialize() { d_patch_strategy->initializeComponent(this); }
  virtual void registerInitPatchData(const int patch_data_id) {
    d_component.insert(patch_data_id);
  }

  /**
   * @brief 在新网格层上, 为待初始化的数据片申请内存空间, 并初始化之.
   *  - 该函数会自动调用用户实现的
   *    algs::StandardComponentPatchStrategy::initializePatchData().
   */
  void initializeLevelData(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level,
      const double time, const bool initial_time) {
    hier::PatchLevel<DIM>* pl =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer());
    Region* mesh = pl->getRegion();

    for (auto item : d_component) mesh->allocateField(item);

    d_patch_strategy->initializePatchData(*pl->getPatch(0), time, initial_time,
                                          this->d_object_name);
  }

 private:
  // 私有成员函数.
  // void initializeDataOnPatchLevel(Region* mesh, const double time, const
  // double initial_time, const string& name) {}

  /** 标准构件网格片策略类对象. */
  StandardComponentPatchStrategy<DIM>* d_patch_strategy;

  /** 数据片选择器, 用于管理初始化过程中需要开辟内存空间的数据片. */
  std::set<int> d_component;
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: \
          SOURCE_WRAPPER_HSFINITIALIZEINTEGRATORCOMPONENT_H */
