#ifndef SOURCE_WRAPPER_HSFTIMEINTEGRATORLEVELSTRATEGY_H
#define SOURCE_WRAPPER_HSFTIMEINTEGRATORLEVELSTRATEGY_H

/*! \file TimeIntegratorLevelStrategy.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include "Pointer.h"
#include "IntegratorComponentManager.h"
#include "PatchLevel.h"
#include "hsfRegion.hpp"

namespace HSF {
namespace algs {

template <int DIM>
class TimeIntegratorLevelStrategy {
 public:
  TimeIntegratorLevelStrategy() {}
  virtual ~TimeIntegratorLevelStrategy() {}

  // bool usingRefinedTimestepping();
  // bool usingImplicitDiscretization();

  virtual void initializeLevelIntegrator(
      HSF::tbox::Pointer<IntegratorComponentManager<DIM>> manager) {}

  virtual void initializeLevelData(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM>> level,
      const double init_data_time, const bool initial_time = true) {}

  // virtual void restartLevelData( const
  // tbox::Pointer<hier::BasePatchLevel<DIM> > level, const double current_time,
  // const int step_no);

  virtual double getLevelDt(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM>> level,
      const double dt_time, const bool initial_time, const int flag_last_dt,
      const double last_dt) {
    return 0;
  }

  virtual int advanceLevel(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM>> level,
      const double current_time, const double predict_dt, const double max_dt,
      const double min_dt, const bool first_step, const int advance_step_number,
      double& actual_dt) {
    return 0;
  }

  virtual void acceptTimeDependentSolution(
      const HSF::tbox::Pointer<hier::BasePatchLevel<DIM>> level,
      const double new_time, const bool last_step) {}

  void setIntegratorComponentManager(
      HSF::tbox::Pointer<IntegratorComponentManager<DIM>> manager) {
    d_component_manager = manager;
  }

 private:
  // 积分构件管理器。
  HSF::tbox::Pointer<IntegratorComponentManager<DIM>> d_component_manager;
};
}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: \
          SOURCE_WRAPPER_HSFTIMEINTEGRATORLEVELSTRATEGY_H */
