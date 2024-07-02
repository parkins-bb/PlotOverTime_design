#ifndef SOURCE_WRAPPER_HSFNUMERICALINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFNUMERICALINTEGRATORCOMPONENT_H

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
 * @brief 数值构件类 algs::NumericalIntegratorComponent 在网格层上,
 * 对应一个BSP阶段, 填充数据片的影像区并完成数值计算.
 *
 * 该构件类使用方法如下:
 * -# 调用构造函数，创建数值构件. \n
 *    创建时, 需要提供标准构件网格片策略类
 *    algs::StandardComponentPatchStrategy 的派生类对象.
 * -# 向数值构件注册待填充的数据片. \n
 *    在函数 algs::StandardComponentPatchStrategy::initializeComponent() 中,
 *    调用函数 registerCommunicationPatchData(), 注册待填充的数据片.
 * -# 执行通信和计算. \n
 *    调用该构件的 computing(), 填充已注册数据片的影像区并完成数值计算.
 *
 * 为了实现该构件类, 用户需要提供"标准构件网格片策略类"
 * algs::StandardComponentPatchStrategy 的派生类.
 *
 */

template <int DIM>
class NumericalIntegratorComponent : public algs::IntegratorComponent<DIM> {
 public:
  /**
   * @brief 构造函数.
   * @param for_initialize_level_data 输入参数, 逻辑型,
   *真值表示该构件服务于初始化网格层数据片, 缺省值为假值.
   */
  NumericalIntegratorComponent(
      const string& object_name,
      algs::StandardComponentPatchStrategy<DIM>* patch_strategy,
      IntegratorComponentManager<DIM>* manager,
      const bool for_initialize_level_data = false)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
  }

  virtual ~NumericalIntegratorComponent() {}

  /**
   * @brief  注册待填充的数据片.
   * @param dst_id 输入参数, 整型,表示待填充的目的数据片的索引号.
   * @param src_id 输入参数, 整型,表示源数据片的索引号.
   * @note
   *目的数据片(dst_id)的内部区域和内部影像区将被源数据片(src_id)通过复制填充.
   *   - 如果src_id等于dst_id, 则仅填充dst_id的影像区.
   */
  void registerCommunicationPatchData(const int dst_id, const int src_id) {
    d_src2dst.insert(make_pair(src_id, dst_id));
  }

  /**
   * @brief 在指定的网格层上, 在指定的物理时刻, 完成数值计算.
   * @param patch_level               输入参数, 指针, 指向网格层.
   * @param time                      输入参数, 双精度型, 表示当前的物理时刻.
   * @param dt                        输入参数, 双精度型, 表示当前的时间步长.
   * @param initial_time              输入参数, 逻辑型, 真值表示当前为初始时刻,
   *缺省值为否.
   * @note
   *    - 该函数会自动调用用户实现的
   *      algs::StandardComponentPatchStrategy::computeOnPatch().
   */
  void computing(const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level,
                 const double time, const double dt,
                 const bool initial_time = false) {
    hier::PatchLevel<DIM>* pl =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer());
    Region* mesh = pl->getRegion();

    /*! 填充 */
    for (auto& item : d_src2dst) {
      if (item.first != item.second) mesh->copyField(item.first, item.second);
      mesh->updateFieldBegin(item.second);
    }
    for (auto& item : d_src2dst) {
      mesh->updateFieldEnd(item.second);
    }
    /*! 计算 */
    d_patch_strategy->computeOnPatch(*pl->getPatch(0), time, dt, initial_time,
                                     this->d_object_name);
  }

  /**
   * @brief 初始化数值构件对象.
   * 该函数注册待填充的数据片,并将通信项注册到通信算法.
   */
  virtual void initialize() { d_patch_strategy->initializeComponent(this); }

 private:
  /** 标准构件网格片策略类对象. */
  algs::StandardComponentPatchStrategy<DIM>* d_patch_strategy;

  /** 该数值构件是否服务于初始化网格层的数据片. */
  bool d_for_initialize_level_data;
  multimap<int, int> d_src2dst;
};

}  // namespace algs
}  // namespace HSF
#endif /* end of include guard: \
          SOURCE_WRAPPER_HSFNUMERICALINTEGRATORCOMPONENT_H */
