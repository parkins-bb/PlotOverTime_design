#ifndef SOURCE_WRAPPER_HSFCOPYINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFCOPYINTEGRATORCOMPONENT_H

#include <string>
#include <vector>

#include "PatchLevel.h"
#include "ustrMesh.hpp"
using namespace std;

#include "IntegratorComponent.h"
#include "StandardComponentManager.h"
#include "StandardComponentPatchStrategy.h"

namespace HSF {
namespace algs {

/**
 * @brief 复制构件类 algs::CopyIntegratorComponent 在网格层上, 复制数据片.
 * 注意: 该构件遍历网格片时, 只将数据从同一网格片上的源数据片复制到目的数据片,
 * 而不会从相邻数据片复制数据到目的数据片的影像区.
 *
 * 该构件类使用方法如下:
 * -# 调用构造函数, 创建复制构件. \n
 *    创建时, 需要提供一个标准构件网格片策略类
 *algs::StandardComponentPatchStrategy 的派生类对象.
 * -# 向复制构件注册待复制的数据片. \n
 *    在派生类中实现函数
 *algs::StandardComponentPatchStrategy::initializeComponent() 时,
 *    调用该构件的函数 registerCopyPatchData(), 注册待复制的数据片.
 * -# 执行数据复制操作. \n
 *    调用该构件的函数 copyPatchData(), 复制数据.
 * @see algs::StandardComponentPatchStrategy
 */

template <int DIM>
class CopyIntegratorComponent : public algs::IntegratorComponent<DIM> {
 public:
  /**
   * @brief 构造函数.
   *
   * @param object_name    输入参数, 字符串, 复制构件对象的名称.
   * @param patch_strategy 输入参数, 指针, 指向标准构件网格片策略类对象.
   * @param manager        输入参数, 指针, 指向积分构件管理器.
   */
  CopyIntegratorComponent(
      const string& object_name,
      algs::StandardComponentPatchStrategy<DIM>* patch_strategy,
      IntegratorComponentManager<DIM>* manager)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
  }

  virtual ~CopyIntegratorComponent() {}

  /**
   * @brief 初始化复制构件对象.
   * 该函数实现基类 algs::IntegratorComponent 的纯虚成员函数.
   * 该函数注册待复制的数据片.
   */
  virtual void initialize() { d_patch_strategy->initializeComponent(this); }

  /**
   * @brief 注册待复制的源数据片和目的数据片.
   * @param dst_id 输入参数, 整型,表示目的数据片的索引号.
   * @param src_id 输入参数, 整型,表示源数据片的索引号.
   */
  void registerCopyPatchData(const int dst_id, const int src_id) {
    ASSERT((dst_id >= 0) && (src_id >= 0));
    d_copy_src2dst.insert(make_pair(src_id, dst_id));
  }

  /**
   * @brief 复制数据片.
   *
   * @param level 输入参数, 指针, 指向网格层.
   * @param time  输入参数, 双精度型, 表示复制时刻.
   */
  void copyPatchData(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level,
      const double time) {
    Region* mesh =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer())->getRegion();
    for (auto item : d_copy_src2dst) {
      mesh->copyField(item.first, item.second);
      mesh->setFieldTimeStamp(item.second, time);
    }
  }

 private:
  /** 标准构件网格片策略类对象. */
  algs::StandardComponentPatchStrategy<DIM>* d_patch_strategy;

  /** 数据片选择向量, 为复制数据片分别存储目的数据片和源数据片的索引号. */
  multimap<int, int> d_copy_src2dst;
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFCOPYINTEGRATORCOMPONENT_H */
