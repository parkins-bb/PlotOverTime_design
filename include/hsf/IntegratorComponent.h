#ifndef SOURCE_WRAPPER_HSFINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFINTEGRATORCOMPONENT_H
/*! \file IntegratorComponent.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <string>

#include "PatchLevel.h"
using namespace std;

namespace HSF {
namespace algs {

template <int DIM>
class IntegratorComponentManager;

/**
 * @brief 积分构件基类 algs::IntegratorComponent，所有积分构件的抽象基类。
 */
template <int DIM>
class IntegratorComponent {
 public:
  IntegratorComponent(const string& object_name,
                      HSF::algs::IntegratorComponentManager<DIM>* manager) {
    d_object_name = object_name;
    d_manager = manager;
  }

  virtual ~IntegratorComponent() {}

  const string& getName() const { return d_object_name; }
  void setName(const std::string& name) { d_object_name = name; }
  virtual void initialize() = 0;
  virtual void registerPatchData(const int patch_data_id) {}

  virtual void registerCopyPatchData(const int dst_id, const int src_id) {}
  virtual void registerInitPatchData(const int patch_data_id) {}
  virtual void registerSweepingPatchData(const int patch_data_id) {}
  virtual void registerParticleData(const int particle_data_id) {}
  virtual void registerCommunicationPatchData(const int dst_id,
                                              const int src_id) {}
  virtual void registerFederals(const int dst_federal_id,
                                const int src_federal_id) {}

  virtual void registerMatrix(int patch_data_id) {}
  virtual void registerMassMatrix(int patch_data_id) {}
  virtual void registerStiffMatrix(int patch_data_id) {}
  // virtual void registerDOFInfo(tbox::Array<tbox::Array<int> > dof_info);
  virtual void registerRHS(int patch_data_id) {}
  virtual void registerSolution(int patch_data_id) {}
  // virtual void registerSolution(const tbox::Array<int>& patch_data_id);

  // virtual void createSchedule(tbox::Pointer<hier::BasePatchLevel<DIM> >
  // level);
  virtual void registerMatrixMultiVector(int mat_id, int src_vec_id,
                                         int dst_vec_id) {}

  HSF::algs::IntegratorComponentManager<DIM>* getManager() { return d_manager; }

 protected:
  /** 对象名称。 */
  string d_object_name;

  HSF::algs::IntegratorComponentManager<DIM>* d_manager;
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFINTEGRATORCOMPONENT_H */
