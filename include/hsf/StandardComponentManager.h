#ifndef SOURCE_WRAPPER_HSFSTANDARDCOMPONENTMANAGER_H
#define SOURCE_WRAPPER_HSFSTANDARDCOMPONENTMANAGER_H

#include <list>
#include <string>

#include "IntegratorComponentManager.h"
#include "hsfRegion.hpp"
using namespace std;

namespace HSF {
namespace algs {
template <int DIM>
class InitializeIntegratorComponent;

template <int DIM>
class StandardComponentManager
    : public HSF::algs::IntegratorComponentManager<DIM> {
 public:
  StandardComponentManager(Region* mesh) { d_mesh = mesh; }

  virtual ~StandardComponentManager();

  virtual void registerComponent(
      HSF::algs::IntegratorComponent<DIM>* component) {
    d_components.push_back(component);
  }
  virtual void unregisterComponent(
      HSF::algs::IntegratorComponent<DIM>* component) {
    d_components.remove(component);
  }

  virtual void initializeComponents();
  // virtual tbox::Pointer<hier::BasePatchHierarchy<DIM> > getPatchHierarchy();
  HSF::algs::InitializeIntegratorComponent<DIM>*
  getInitializeIntegratorComponent() const;

 private:
  /// 链表，包含所有构件对象。
  std::list<HSF::algs::IntegratorComponent<DIM>*> d_components;
  /// 指针，指向网格片层次结构。
  Region* d_mesh;
};

}  // namespace algs
}  // namespace HSF
#endif /* end of include guard: SOURCE_WRAPPER_HSFSTANDARDCOMPONENTMANAGER_H \
        */
