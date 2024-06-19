#ifndef SOURCE_WRAPPER_HSFSTANDARDCOMPONENTPATCHSTRATEGY_H
#define SOURCE_WRAPPER_HSFSTANDARDCOMPONENTPATCHSTRATEGY_H
/*! \file StandardComponentPatchStrategy.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <string>

#include "Patch.h"
using namespace std;

namespace HSF {

namespace algs {

template <int DIM>
class IntegratorComponent;

template <int DIM>
class StandardComponentPatchStrategy {
 public:
  StandardComponentPatchStrategy() {}
  virtual ~StandardComponentPatchStrategy() {}

  virtual void initializeComponent(
      HSF::algs::IntegratorComponent<DIM>* component) const = 0;
  virtual void initializePatchData(hier::Patch<DIM>& patch, const double time,
                                   const bool initial_time,
                                   const string& component_name) {}
  virtual void computeOnPatch(hier::Patch<DIM>& patch, const double time,
                              const double dt, const bool initial_time,
                              const string& component_name) {}
  virtual double getPatchDt(hier::Patch<DIM>& patch, const double time,
                            const bool initial_time, const int flag_last_dt,
                            const double last_dt,
                            const string& component_name) {
    return 0;
  }
  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  virtual void reduceOnPatch(double* vector, int len, hier::Patch<DIM>& patch,
                             const double time, const double dt,
                             const string& component_name) {}
  virtual void remapPatchData(hier::Patch<DIM>& patch, const double time,
                              const string& component_name) {}
  virtual void setPhysicalBoundaryConditions(hier::Patch<DIM>& patch,
                                             const double fill_time,
                                             const double dt,
                                             const string& component_name) {}

  void setTagIndex(const int tag_index) { d_tag_index = tag_index; };
  void clearTagIndex() { d_tag_index = -1; };
  int getTagIndex() { return (d_tag_index); };
  void setIntCName(const string& component_name) {
    d_current_component_name = component_name;
  };
  void clearIntCName() { d_current_component_name = ""; };

 private:
  /*! 当前构件的名称. */
  string d_current_component_name;
  /*! 当前存储标记值的数据片索引号. */
  int d_tag_index;
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: \
          SOURCE_WRAPPER_HSFSTANDARDCOMPONENTPATCHSTRATEGY_H */
