#ifndef SOURCE_WRAPPER_HSFOUTERDATAOPERATIONINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFOUTERDATAOPERATIONINTEGRATORCOMPONENT_H

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

//     enum CommunicationOperation {
//   COPY = 0,       // 复制.
//   SUM = 1,        // 累加和.
//   MIN = 2,        // 最小值.
//   MAX = 3,        // 最大值.
//   ROUND_OFF = 4,  // 消除舍入误差（取最小值）.
//   USER = 5
// };

/**
 * @brief 外表面操作构件类 algs::OuterdataOperationIntegratorComponent
 * 在单个网格层上, 对相邻网格片数据片共享的外表面进行操作.
 *
 * 支持结点量、边心量、面心量.
 *
 * 该构件类使用方法如下:
 * -# 调用构造函数, 创建构件. \n
 *    创建时, 需要提供标准构件网格片策略类
 *    algs::StandardComponentPatchStrategy 的派生类对象.
 * -# 向构件注册数据片. \n
 *    在函数 algs::StandardComponentPatchStrategy::initializeComponent() 中,
 *    调用函数 registerPatchData(), 注册数据片.
 * -# 执行操作. \n
 *    调用 operate(), 执行操作.
 *
 * 该构件支持克隆计算. 克隆计算时，请用户特别注意:
 *  - 在属主网格层和克隆网格层同时分别进行.
 *
 * @see algs::StandardComponentPatchStrategy
 */

template <int DIM>
class OuterdataOperationIntegratorComponent
    : public algs::IntegratorComponent<DIM> {
 public:
  /**
   * @brief 构造函数.
   *
   * @param object_name    输入参数, 字符串, 对象名.
   * @param patch_strategy 输入参数, 指针, 指向标准构件网格片策略类对象.
   * @param manager        输入参数, 指针, 指向积分构件管理器.
   * @param operation      输入参数, 字符串, 表示操作类型.
   *
   * @note 当前, 参数 operation 可供选择的值为:
   *     - operation = "SUM": 求和（仅支持结点量双精度数据片）;
   *     - operation = "MIN": 求最小值;
   *     - operation = "MAX": 求最大值;
   *     - operation = "ROUND_OFF": 控制舍入误差;
   */
  OuterdataOperationIntegratorComponent(
      const string& object_name,
      algs::StandardComponentPatchStrategy<DIM>* patch_strategy,
      algs::IntegratorComponentManager<DIM>* manager, const string& operation)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
    d_op = operation;
  }

  /**
   * @brief 析构函数.
   */
  virtual ~OuterdataOperationIntegratorComponent() {}

  ///@cond DEV
  /**
   * @brief 初始化对象.
   * 该函数实现基类 algs::IntegratorComponent 的纯虚成员函数.
   */
  virtual void initialize() { d_patch_strategy->initializeComponent(this); }

  /**
   * @brief 在指定的物理时刻, 在指定网格层上, 创建通信调度.
   *
   * @param patch_level 输入参数, 指针, 指向网格层.
   */
  virtual void createSchedule(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level) {}
  ///@endcond

  /**
   * @brief 将数据片注册到积分构件中.
   *
   * @param data_id 输入参数, 整型, 数据片索引号.
   */
  virtual void registerPatchData(const int data_id) {
    d_patch_data_selector.insert(data_id);
  }

  /**
   * @brief 执行操作.
   * @param level 输入参数, 指针, 指向网格层.
   */
  void operate(const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level) {
    hier::PatchLevel<DIM>* pl =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer());
    Region* mesh = pl->getRegion();
    /*! 填充 */
    if (d_op == "COPY") {
      for (auto item : d_patch_data_selector) {
        mesh->updateFieldBegin(item);
      }
      for (auto item : d_patch_data_selector) {
        mesh->updateFieldEnd(item);
      }
    } else {
      for (auto item : d_patch_data_selector) {
        mesh->reduceFieldBegin(item);
        mesh->reduceFieldEnd(item, d_op);
      }
      for (auto item : d_patch_data_selector) {
        mesh->updateFieldBegin(item);
      }
      for (auto item : d_patch_data_selector) {
        mesh->updateFieldEnd(item);
      }
    }
  }

 private:
  /** 数据操作类型. */
  std::string d_op;

  /** 标准构件网格片策略类对象. */
  algs::StandardComponentPatchStrategy<DIM>* d_patch_strategy;

  /** 数据片选择向量, 存储待调度的数据片. */
  std::set<int> d_patch_data_selector;

  /** 通信算法. */
  /** 通信调度 */
  /** 通信调度 */

  // 计时器.
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: \
          SOURCE_WRAPPER_HSFOUTERDATAOPERATIONINTEGRATORCOMPONENT_H */
