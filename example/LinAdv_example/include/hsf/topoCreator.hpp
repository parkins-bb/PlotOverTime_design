/*! \file topoCreator.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-01-30
 *  \brief Topology factory, not used yet
 */
#ifndef ALGORITHM_TOPOLOGY_TOPOCREATOR_HPP
#define ALGORITHM_TOPOLOGY_TOPOCREATOR_HPP
#include <iostream>

#include "topology.hpp"

class Parameter;

namespace HSF {

template <typename... Args>
using topoCreatorPtr = Topology (*)(Args... sets);

template <typename... Args>
class TopoFactory {
 public:
  static HASHMAP<Word, topoCreatorPtr<Args...> > topoMap_;
  static Topology* constructTopo(Word type, Args... sets) {
    if (topoMap_.find(type) == topoMap_.end()) {
      COUT << "Incorrect Topology Type!" << ENDL;
      exit(1);
    }
    return topoMap_.find(type)->second(sets...);
  }
  static Topology* constructTopo(Parameter& para);
};

template <typename... Args>
class RegisterTopoCtor {
 public:
  RegisterTopoCtor(const Word& name, topoCreatorPtr<Args...> fptr) {
    if (TopoFactory<Args...>::topoMap_.find(name) ==
        TopoFactory<Args...>::topoMap_.end()) {
      TopoFactory<Args...>::topoMap_[name] = fptr;
    }
  }
  ~RegisterTopoCtor() {}
};

} /* End namespace HSF */

#endif /* end of include guard: ALGORITHM_TOPOLOGY_TOPOCREATOR_HPP */
