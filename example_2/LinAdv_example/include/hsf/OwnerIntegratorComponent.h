#ifndef SOURCE_WRAPPER_HSFOWNERINTEGRATORCOMPONENT_H
#define SOURCE_WRAPPER_HSFOWNERINTEGRATORCOMPONENT_H
/*! \file OwnerIntegratorComponent.h
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
 * @brief 属主构件类 algs::OwnerIntegratorComponent 在网格层上,
 * 设置该网格层中每个网格片上的网格实体的属主标志。
 *
 * 在JAUMIN框架中，某些网格实体（点、边、面）同时存在于两个
 * 甚至多个网格片。应用程序可通过属主构件，为每个网格实体
 * 指定唯一的属主网格片。
 *
 * 该构件类使用方法如下:
 * -# 调用构造函数，创建构件. \n
 * -# 向构件注册用于接收属主标志的数据片. \n
 *    在函数 algs::StandardComponentPatchStrategy::initializeComponent() 中,
 *    调用函数 registerPatchData(), 注册用于接收属主标志的数据片.
 * -# 设置数组标志. \n
 *    调用该构件的 setup(), 完成属主标志的设置.
 *
 * @see algs::StandardComponentPatchStrategy
 */
template <int DIM>
class OwnerIntegratorComponent : public algs::IntegratorComponent<DIM> {
 public:
  /**
   * @brief 构造函数.
   *
   * @param object_name               输入参数, 字符串, 数值构件对象的名称.
   * @param patch_strategy            输入参数, 指针,
   *指向标准构件网格片策略类对象.
   * @param manager                   输入参数, 指针, 指向积分构件管理器.
   */
  OwnerIntegratorComponent(
      const string& object_name,
      algs::StandardComponentPatchStrategy<DIM>* patch_strategy,
      algs::IntegratorComponentManager<DIM>* manager)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
    d_patch_strategy = patch_strategy;
  }

  /**
   * @brief 析构函数.
   */
  virtual ~OwnerIntegratorComponent() {}

  /**
   * @brief  注册用于接收属主标志的数据片.
   * @param owner_data_id 输入参数, 整型,表示用于接收属主标志的数据片的索引号.
   *                      该数据片可对应于结点量、面心量或中心量.
   * 数值类型必须是
   *                      布尔型，真值表示相应的网格实体被其所在的网格片拥有.
   *                      数据片可以带有影像区，也可以不带影像区.
   */
  void registerPatchData(const int owner_data_id) {
    d_owner_data_id.insert(owner_data_id);
  }

  /**
   * @brief 在指定的网格层上, 设置属主标志.
   * @param patch_level               输入参数, 指针, 指向网格层.
   */
  void setup(const HSF::tbox::Pointer<hier::BasePatchLevel<DIM> > level) {
    hier::PatchLevel<DIM>* pl =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer());
    Region* mesh = pl->getRegion();
    USTRMesh* umesh = dynamic_cast<USTRMesh*>(mesh->getMesh());
    for (auto item : d_owner_data_id) {
      Field<int>& f = mesh->getField<int>(item);
      f = 1;
      SetType t = f.getSetType();
      const std::vector<label>*sharedInd, *ghostInd;
      const std::vector<int>*sharedGrp, *ghostGrp;
      const GroupCommunicator *sharedgc, *ghostgc;
      auto* interf = umesh->getInterface();
      switch (t) {
        case SetType::NODE:
          sharedInd = &interf->sharedNodes_;
          ghostInd = &interf->ghostNodes_;
          sharedGrp = &interf->sNodeGroup_;
          ghostGrp = &interf->gNodeGroup_;
          sharedgc = interf->snodeGc_;
          ghostgc = interf->gnodeGc_;
          break;
        case SetType::EDGE:
          sharedInd = &interf->sharedEdges_;
          ghostInd = &interf->ghostEdges_;
          sharedGrp = &interf->sEdgeGroup_;
          ghostGrp = &interf->gEdgeGroup_;
          sharedgc = interf->sedgeGc_;
          ghostgc = interf->gedgeGc_;
          break;
        case SetType::FACE:
          sharedInd = &interf->sharedFaces_;
          ghostInd = &interf->ghostFaces_;
          sharedGrp = &interf->sFaceGroup_;
          ghostGrp = &interf->gFaceGroup_;
          sharedgc = interf->sfaceGc_;
          ghostgc = interf->gfaceGc_;
          break;
        case SetType::CELL:
        case SetType::DOF:
        default:
          COUT << "Not implemented!" << ENDL;
      }

      for (std::size_t i = 0; i < sharedInd->size(); i++) {
        if (!sharedgc->GetGroupTopology().isMaster((*sharedGrp)[i]))
          f[(*sharedInd)[i]] = 0;
      }

      if (f.getHasGhost()) {
        for (std::size_t i = 0; i < ghostInd->size(); i++) {
          if (!ghostgc->GetGroupTopology().isMaster((*ghostGrp)[i]))
            f[(*ghostInd)[i]] = 0;
        }
      }
    }
  }

  void initialize() { d_patch_strategy->initializeComponent(this); }

 private:
  /** 标准构件网格片策略类对象. */
  algs::StandardComponentPatchStrategy<DIM>* d_patch_strategy;

  std::set<int> d_owner_data_id;
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFOWNERINTEGRATORCOMPONENT_H \
        */
