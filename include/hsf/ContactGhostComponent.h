#ifndef SOURCE_WRAPPER_HSFCONTACTGHOSTCOMPONENT_H
#define SOURCE_WRAPPER_HSFCONTACTGHOSTCOMPONENT_H
/*! \file ContactGhostComponent.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <set>
#include <string>
#include <vector>

#include "PatchLevel.h"
#include "collisionManager.hpp"

using namespace std;

#include "IntegratorComponent.h"
#include "StandardComponentManager.h"
#include "StandardComponentPatchStrategy.h"

namespace HSF {
namespace algs {

/**
 * @brief 接触影像区构件类 algs::ContactGhostComponent 为网格层创建
 * 接触影像区.
 *
 * 在JAUMIN框架中，通过拓扑相邻关系形成的影像区称为“拓扑影像区”，
 * 通过几何相邻关系形成的影像区称为"接触影像区"。本构件的功能
 * 是为网格层创建接触影像区。
 *
 * 该构件类使用方法如下:
 * -# 调用构造函数，创建构件. \n
 *    创建时, 需要提供标准构件网格片策略类
 *    algs::StandardComponentPatchStrategy 的派生类对象.
 * -# 向构件注册接触对. \n
 *    在函数 algs::StandardComponentPatchStrategy::initializeComponent() 中,
 *    调用函数 registerContactPair(), 注册接触对.
 *    例如：
      void
 UserPatchStrategy<DIM>::initializeComponent(algs::IntegratorComponent<DIM> *
 component) const
      {
          const string &component_name = component->getName();
          if(component_name == "CONTACT_GHOST") {
              ((algs::ContactGhostComponent<DIM>*)component)->registerContactPair(
                      master_face_id, hier::EntityUtilities::FACE,
                      slave_face_id, hier::EntityUtilities::FACE);
          }
      }
 * -# 创建接触影像区. \n
 *    调用该构件的 contact(), 为网格层创建或更新接触影像区。
 *
 * 为了实现该构件类, 用户需要提供"标准构件网格片策略类"
 * algs::StandardComponentPatchStrategy 的派生类.
 *
 * @see algs::StandardComponentPatchStrategy
 */
template <int DIM>
class ContactGhostComponent : public algs::IntegratorComponent<DIM> {
 public:
  /**
   * @brief 构造函数.
   *
   * @param object_name               输入参数, 字符串, 数值构件对象的名称.
   * @param patch_strategy            输入参数, 指针,
   *指向标准构件网格片策略类对象.
   * @param manager                   输入参数, 指针, 指向积分构件管理器.
   */
  ContactGhostComponent(
      const string& object_name,
      algs::StandardComponentPatchStrategy<DIM>* patch_strategy,
      algs::IntegratorComponentManager<DIM>* manager)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
    contact_manager = NULL;
  }

  /**
   * @brief 析构函数.
   */
  virtual ~ContactGhostComponent() { DELETE_POINTER(contact_manager); }

  /*!
   * @brief 注册接触实体集对
   * @param set_id_1，输入参数，实体集编号
   * @param set_type_1，输入参数，实体集类型
   * @param set_id_2，输入参数，实体集编号
   * @param set_type_2，输入参数，实体集类型
   * @note  调用此函数意味着实体集<set_id_1, set_type_2>会跟实体集<set_id_2,
   * set_type_2>接触
   */
  void registerContactPair(int set_id_1,
                           hier::EntityUtilities::EntityType set_type_1,
                           int set_id_2,
                           hier::EntityUtilities::EntityType set_type_2) {
    ASSERT(set_type_1 == hier::EntityUtilities::EntityType::FACE);
    ASSERT(set_type_2 == hier::EntityUtilities::EntityType::FACE);
    if (set_id_1 <= set_id_2)
      pairs_.insert(std::make_pair(set_id_1, set_id_2));
    else
      pairs_.insert(std::make_pair(set_id_2, set_id_1));
  }

  /**
   * @brief 在指定的网格层上, 创建拓扑连接影像区.
   *
   * @param patch_level      输入参数, 指针, 指向网格层.
   * @param topo_width       输入参数, 整型, 拓扑影像区宽度.
   * @param only_boundary     输入参数,
   *布尔型，真值表示只扩接触边界上的拓扑影像单元.
   *
   * @note
   * -# 若调用此函数前网格层已存在接触影像区，则删除之，但拓扑连接影像区将保留.
   */
  void topological(const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level,
                   const int topo_width, const bool only_boundary = true) {}

  /**
   * @brief 在指定的网格层上, 创建或更新接触影像区.
   *
   * @param patch_level      输入参数, 指针, 指向网格层.
   * @param contact_gap      输入参数, 浮点型, 接触预警区宽度.
   * @param contact_on_boundary 输入参数, 布尔型，真值表示接触只发生在物理边界.
   * @param mode             "N", "F", or "FN" (See doc/programming_guide.pdf).
   *
   * @note
   * -#    每个网格片的几何包围盒外扩contact_gap之后，与之
   *       相交的邻居网格片上的网格单元将会被收集到该网格片，成为
   *       它的接触影像单元.
   * -#    创建或更新接触影像区后，所有带有影像区的数据片在影像区
   *       上的值会被破坏，应用程序须根据自身需要重新填充数据片影像区。
   */
  void contact(const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level,
               const double contact_gap, const bool contact_on_boundary = true,
               const string& mode = "N") {
    Region* mesh =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer())->getRegion();
    if (contact_manager == NULL) {
      contact_manager = new CollisionManager(mesh);
      for (auto& item : pairs_)
        contact_manager->addContactPair(item.first, item.second);
    }
    SetType m;
    if (mode == "N") {
      m = SetType::NODE;
    } else if (mode == "F") {
      m = SetType::FACE;
    } else if (mode == "FN") {
      m = SetType::EDGE;
    }
    contact_manager->createContactGhost(contact_gap, contact_on_boundary, m);
  }

  /**
   * @brief 初始化数值构件对象.
   * 该函数注册待填充的数据片,并将通信项注册到通信算法.
   */
  void initialize() { d_patch_strategy->initializeComponent(this); }

  /**
   * @brief 在指定的物理时刻, 在指定网格层上, 创建通信调度.
   * @param patch_level 输入参数, 指针, 指向网格层.
   */
  // void createSchedule( const HSF::tbox::Pointer<hier::BasePatchLevel<DIM>
  // > level) {}
  ///@endcond

 private:
  /** 标准构件网格片策略类对象. */
  algs::StandardComponentPatchStrategy<DIM>* d_patch_strategy;
  std::set<std::pair<int, int> > pairs_;
  CollisionManager* contact_manager;

  // tbox::Pointer<hier::ContactDescriptor<DIM> > d_contact_descriptor;
  // tbox::Pointer<hier::ContactGhostExpansion<DIM> > d_contact_ghost_expansion;
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFCONTACTGHOSTCOMPONENT_H */
