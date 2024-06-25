#ifndef SOURCE_WRAPPER_HSFHIERARCHYTIMEINTEGRATOR_H
#define SOURCE_WRAPPER_HSFHIERARCHYTIMEINTEGRATOR_H

/*! \file HierarchyTimeIntegrator.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>

#include "Database.h"
#include "Pointer.h"
#include "PatchHierarchy.h"
#include "hsfRegion.hpp"

using namespace std;

namespace HSF {
namespace algs {
template <int DIM>
class TimeIntegratorLevelStrategy;

template <int DIM>
class HierarchyTimeIntegrator {
 public:
  HierarchyTimeIntegrator(
      const string& object_name,
      HSF::tbox::Pointer<HSF::tbox::Database> input_db,
      HSF::tbox::Pointer<hier::PatchHierarchy<DIM> > patch_hierarchy,
      TimeIntegratorLevelStrategy<DIM>* level_strategy,
      bool register_for_restart = true);

  ~HierarchyTimeIntegrator() {}

  bool initializeHierarchy();
  double advanceHierarchy();

  double getIntegratorTime() const { return d_integrator_time; }
  double getStartTime() const { return d_start_time; }
  double getEndTime() const { return d_end_time; }
  void resetEndTime(const double end_time) { d_end_time = end_time; }
  int getIntegratorStep() const { return d_integrator_step; }
  int getMaxIntegratorSteps() const { return d_max_integrator_steps; }
  bool stepsRemaining() const {
    return (d_integrator_step < d_max_integrator_steps);
  }

  // const tbox::Pointer<hier::BasePatchHierarchy<DIM> > getPatchHierarchy()
  // const; tbox::Pointer<TimeIntegratorLevelStrategy<DIM> > getLevelStrategy()
  // const;
 private:
  // double advancePatchLevel( tbox::Pointer<hier::BasePatchLevel<DIM> >
  // patch_level);
  string d_object_name;          /*!< 对象名称。 */
  bool d_registered_for_restart; /*!< 该对象是否注册到重启动管理器。 */
  TimeIntegratorLevelStrategy<DIM>*
      d_level_strategy; /*!< 单层网格时间积分算法 */
  HSF::tbox::Pointer<hier::PatchHierarchy<DIM> > d_patch_hierarchy;
  Region* d_reg; /*!<  region */

  double d_start_time;        /*!< 积分起始时刻 */
  double d_end_time;          /*!< 积分终止时刻 */
  int d_max_integrator_steps; /*!< 允许的最大积分步数 */
  double d_max_dt;            /*!< 允许的最大时间步长 */
  double d_grow_dt =
      1.2; /*!< 相对于上个时间步, 当前时间步长允许放大的最大倍数 */

  /**
   * 重建网格的时间步间隔。
   */
  int d_regrid_interval = 2;
  /**
   * 网格细化的预警区宽度。
   */
  int d_regrid_tag_buffer;

  bool d_is_deforming;
  /**
   * 重分重映网格的时间步间隔。
   */
  int d_remap_interval = 0;
  /**
   * 重分重映网格的警区宽度。
   */
  int d_remap_tag_buffer = 1;

  bool d_use_implicit_timestepping; /*!< 真值表示时间积分采用隐式格式。 */

  double d_integrator_time; /*!< 积分的当前时刻。 */
  double d_level_sim_time;
  int d_integrator_step; /*!< 积分的当前时间步数。 */

  double d_dt_max_level;    /*!< 允许的最大时间步长。 */
  double d_dt_min_level;    /*!< 允许的最小时间步长。 */
  double d_dt_actual_level; /*!< 当前的实际时间步长。 */
  double d_dt_last_step;    /*!< 上一个时间步的步长。 */
  int d_flag_last_step;     /*!< 上一个时间步积分的返回值。 */
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFHIERARCHYTIMEINTEGRATOR_H */
