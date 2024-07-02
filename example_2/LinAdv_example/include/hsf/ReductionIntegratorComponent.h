#ifndef SOURCE_WRAPPER_HSFREDUCTIONINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFREDUCTIONINTEGRATORCOMPONENT_H
/*! \file ReductionIntegratorComponent.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

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
 * @brief 归约构件类 algs::ReductionIntegratorComponent 在网格层上,
 *        执行归约计算.
 *
 * 归约构件类使用方法如下:
 * -# 调用构造函数, 创建归约构件. \n
 *    创建时, 需要提供一个标准构件网格片策略类
 *algs::StandardComponentPatchStrategy 的派生类对象,
 *    以及一个归约操作类(tbox::MPI::Op)对象.
 * -# 初始化归约向量.
 * -# 执行归约计算. \n
 *    调用该构件的函数 reduction(), 完成归约操作.
 *
 * 归约构件执行通信和计算时, 将调用"标准构件网格片策略类"派生类的成员函数.
 *具体如下:
 * -# 通信阶段: 与数值构件 algs::NumericalIntegratorComponent 的行为相同.
 * -# 计算阶段: 遍历网格层的每个网格片, 并调用函数
 *algs::StandardComponentPatchStrategy::reduceOnPatch(),
 *    逐个网格片地执行归约计算. 最后返回整个网格层的归约值.
 *
 * @note
 *  -
 *  - 当前，该类只支持double类型的归约计算，
 *    如果需要归约其他类型的数据，请先转换为double型。
 *    且只支持归约求和， 求最大， 求最小三种归约。
 *
 * @see algs::StandardComponentPatchStrategy
 */

template <int DIM>
class ReductionIntegratorComponent : public algs::IntegratorComponent<DIM> {
 public:
  /**
   * @brief 构造函数.
   *
   * @param object_name 输入参数, 字符串, 归约构件对象的名称.
   * @param op          输入参数, 归约操作类, 归约操作.
   * @param patch_strategy 输入参数, 指针, 指向标准构件网格片策略类对象.
   * @param manager 输入参数, 指针, 指向积分构件管理器.
   * @param is_clone_computing 输入参数, 逻辑型, 真值表示该构件服务于克隆计算,
   *缺省值为假值.
   *
   * @note
   * 参数 op 可以设为标准的MPI归约算子, 如 MPI_MAX, MPI_MIN,
   * 也可以设为自定义的MPI归约算子。
   */
  ReductionIntegratorComponent(
      const string &object_name, MPI::Op op,
      algs::StandardComponentPatchStrategy<DIM> *patch_strategy,
      algs::IntegratorComponentManager<DIM> *manager)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
    d_reduction_op = op;
  }

  /**
   * @brief 析构函数.
   */
  virtual ~ReductionIntegratorComponent() {}

  ///@cond DEV

  /**
   * @brief 初始化归约构件对象.
   * 该函数实现基类 algs::IntegratorComponent 的纯虚成员函数.
   *
   * 该函数注册待填充的数据片,并将通信项注册到通信算法.
   */
  virtual void initialize() { d_patch_strategy->initializeComponent(this); }

  ///@endcond

  /**
   * @brief 在指定的网格层上, 执行归约计算.
   *
   * @param vector       输出参数, 指针, 指向归约向量.
   * @param len          输入参数, 整型, 归约向量的长度.
   * @param patch_level  输入参数, 指针, 指向网格层.
   * @param time         输入参数, 双精度型, 表示当前的物理时刻.
   * @param dt           输入参数, 双精度型, 表示当前时间步长.
   * @param inside_clone 输入参数,
   *布尔值，真值表示各个克隆网格层独立规约，缺省为假.
   *
   * @note
   *  - 在调用该函数前，vector需要先初始化.
   *  - 该函数会自动调用用户实现的
   *    algs::StandardComponentPatchStrategy::reduceOnPatch().
   */
  void reduction(double *vector, int len,
                 const HSF::tbox::Pointer<hier::BasePatchLevel<DIM>> level,
                 const double time, const double dt,
                 const bool inside_clone = false) {
    hier::PatchLevel<DIM> *pl =
        dynamic_cast<hier::PatchLevel<DIM> *>(level.getPointer());
    Region *mesh = pl->getRegion();
    USTRMesh *umesh = dynamic_cast<USTRMesh *>(mesh->getMesh());
    HSF::tbox::Pointer<hier::Patch<DIM>> pch = pl->getPatch(0);

    /*! 计算 */
    d_patch_strategy->reduceOnPatch(vector, len, *pch, time, dt,
                                    this->d_object_name);

    /*! 通信 */
    MPI_Allreduce(MPI_IN_PLACE, vector, len, MPI_DOUBLE, d_reduction_op,
                  umesh->comm()->getMpiComm());
  }

 private:
  MPI::Op d_reduction_op;

  /** 标准构件网格片策略类对象. */
  algs::StandardComponentPatchStrategy<DIM> *d_patch_strategy;

  // 计时器.
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: \
          SOURCE_WRAPPER_HSFREDUCTIONINTEGRATORCOMPONENT_H */
