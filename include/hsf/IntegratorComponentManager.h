#ifndef SOURCE_WRAPPER_HSFINTEGRATORCOMPONENTMANAGER_H
#define SOURCE_WRAPPER_HSFINTEGRATORCOMPONENTMANAGER_H

#include <string>
using namespace std;

namespace HSF {
namespace algs {

template <int DIM>
class IntegratorComponent;

template <int DIM>
class IntegratorComponentManager {
 public:
  IntegratorComponentManager() {}

  virtual ~IntegratorComponentManager() {}

  virtual void registerComponent(IntegratorComponent<DIM>* component) {}
  virtual void unregisterComponent(IntegratorComponent<DIM>* component) {}
  virtual void initializeComponents() = 0;
  // virtual void resetHierarchyConfiguration(
  // tbox::Pointer<hier::BasePatchHierarchy<DIM> > hierarchy) = 0; virtual void
  // resetLevelConfiguration( tbox::Pointer<hier::BasePatchLevel<DIM> > level) =
  // 0;

 private:
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFINTEGRATORCOMPONENTMANAGER_H \
        */
