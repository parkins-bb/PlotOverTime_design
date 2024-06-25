#ifndef USTR_CONTACT_COLLISIONMANAGER_HPP
#define USTR_CONTACT_COLLISIONMANAGER_HPP
/*! \file collisionManager.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief contact ghost communication manager
 */

#include "collisionDetection.hpp"
#include "connectivity.hpp"
#include "hsfRegion.hpp"
#include "ustrMesh.hpp"
namespace HSF {

/*! 各种场、拓扑需要变化，subset也要调整；geo可以维持不变？ */
class CollisionManager {
 private:
  std::vector<CollisionDetection> collisions_;
  Region* reg_;
  USTRMesh* mesh_;
  label num_[4]; /*!< old node/edge/face/cell num(contains topological ghost) */
  struct InterfaceRecord {
    label gc_, gf_, ge_, gn_, gelemNum_;
    ProcGroup grp_;
    map<int, std::set<llabel>> proc2myghost_;
  } interfaceRec_;
  struct SubsetRecord {
    map<std::pair<int, SetType>, label> id2num_;
  } subsetRec_;
  /*! delete contact ghost layer before build */
  void deleteContactGhost();
  /*! rebuild mesh entities after add contact ghost layer */
  void updateEntities();
  /*! rebuild ghost Face/Edge/Node  */

 public:
  CollisionManager();
  CollisionManager(Region* reg);
  ~CollisionManager();

  /*! init function */
  void addContactPair(int set_id_1, int set_id_2);

  /*! recreate contact ghost */
  void createContactGhost(scalar padding, bool only_boundary = true,
                          SetType mode = NODE);
};

} /* End namespace HSF */
#endif /* end of include guard: USTR_CONTACT_COLLISIONMANAGER_HPP */
