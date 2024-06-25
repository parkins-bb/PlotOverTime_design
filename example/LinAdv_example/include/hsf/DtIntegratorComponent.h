#ifndef SOURCE_WRAPPER_HSFDTINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFDTINTEGRATORCOMPONENT_H

#include <string>
#include <vector>

#include "PatchLevel.h"
#include "mpi.h"
using namespace std;

#include "IntegratorComponent.h"
#include "StandardComponentManager.h"
#include "StandardComponentPatchStrategy.h"

namespace HSF {
namespace algs {

/**
 * 步长构件类使用方法如下:
 * -# 调用构造函数, 创建步长构件. \n
 *    创建时, 需要提供一个标准构件网格片策略类
 *algs::StandardComponentPatchStrategy 的派生类对象.
 * -# 向步长构件注册待填充数据片. \n
 *    在派生类中实现函数
 *algs::StandardComponentPatchStrategy::initializeComponent() 时,
 *    调用该构件的函数 registerCommunicationPatchData(), 注册待填充的数据片.
 * -# 执行通信和计算. \n
 *    调用该构件的函数 getLevelDt(), 执行指定网格层上的数据填充和时间步长计算.
 *
 * 步长构件执行通信和计算时, 将调用"标准构件网格片策略类"派生类的成员函数.
 *具体如下:
 * -# 通信阶段: 与数值构件 algs::NumericalIntegratorComponent 的行为相同.
 * -# 计算阶段: 遍历网格层的每个网格片,
 *并调用标准构件网格片策略类派生类中的函数getPatchDt(),
 *    逐个网格片地计算时间步长; 最后返回所有网格片中最小的时间步长.
 */
template <int DIM>
class DtIntegratorComponent : public algs::IntegratorComponent<DIM> {
 public:
  DtIntegratorComponent(
      const string& object_name,
      algs::StandardComponentPatchStrategy<DIM>* patch_strategy,
      IntegratorComponentManager<DIM>* manager)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
  }

  virtual ~DtIntegratorComponent() {}

  /**
   * @brief 初始化步长构件对象.
   * 该函数实现基类 algs::IntegratorComponent 的纯虚成员函数.
   *
   * 该函数注册待填充的数据片,并将通信项注册到通信算法.
   */
  virtual void initialize() { d_patch_strategy->initializeComponent(this); }

  /**
   * @brief  注册待填充的数据片.
   * @param dst_id 输入参数, 整型,表示待填充的目的数据片的索引号.
   * @param src_id 输入参数, 整型,表示源数据片的索引号.
   * @note
   *   -
   *目的数据片(dst_id)的内部区域和内部影像区将被源数据片(src_id)通过复制填充.
   *   - 如果src_id等于dst_id, 则仅填充dst_id的影像区.
   */
  virtual void registerCommunicationPatchData(const int dst_id,
                                              const int src_id) {}

  /**
   * @brief 在指定的网格层上, 在指定的物理时刻, 计算时间步长.
   *
   * @param patch_level  输入参数, 指针, 指向网格层.
   * @param time         输入参数, 双精度型, 表示当前的物理时刻.
   * @param initial_time 输入参数, 逻辑型, 真值表示当前为初始时刻.
   * @param flag_last_dt 输入参数, 整型, 表示上个时间步积分的状态?
   * @param last_dt      输入参数, 双精度型, 表示上个时间步的步长?
   * @param do_physical_boundary_fill 输入参数, 逻辑型,
   *真值表示填充物理边界条件, 缺省值为真值?
   *
   * @return 双精度型, 表示允许的时间步长.
   *
   * @note
   *  - 该函数会自动调用用户实现的
   *algs::StandardComponentPatchStrategy::getPatchDt().
   */
  double getLevelDt(const HSF::tbox::Pointer<hier::PatchLevel<DIM> > level,
                    const double time, const bool initial_time,
                    const int flag_last_dt, const double last_dt,
                    const bool do_physical_boundary_fill = true) {
    hier::PatchLevel<DIM>* pl =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer());
    Region* mesh = pl->getRegion();

    if (do_physical_boundary_fill) {
      /*! 通信阶段？需不需要，需要的话若何判断哪些量要通信？ */
    }
    double dt = d_patch_strategy->getPatchDt(*pl->getPatch(0), time,
                                             initial_time, flag_last_dt,
                                             last_dt, this->d_object_name);
    Communicator* comm = mesh->getMesh()->comm();
    MPI_Allreduce(MPI_IN_PLACE, &dt, 1, MPI_DOUBLE, MPI_MIN,
                  comm->getMpiComm());
    return dt;
  }

 private:
  /** 标准构件网格片策略类对象. */
  algs::StandardComponentPatchStrategy<DIM>* d_patch_strategy;
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFDTINTEGRATORCOMPONENT_H */
