#ifndef SOURCE_WRAPPER_HSFUPDATECOORDINATECOMPONENT_H
#define SOURCE_WRAPPER_HSFUPDATECOORDINATECOMPONENT_H
/*! \file UpdateCoordinateComponent.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <string>
#include <vector>

#include "Patch.h"
#include "PatchLevel.h"
#include "ustrMesh.hpp"
using namespace std;

#include "IntegratorComponent.h"
#include "StandardComponentManager.h"
#include "StandardComponentPatchStrategy.h"

namespace HSF {

namespace algs {

/**
 * @brief 坐标更新构件类 algs::UpdateCoordinateComponent
 * 更新影像区的结点坐标，清除框架缓存的边心、面心、体心坐标.
 *
 * 如果应用程序修改了PatchGeometry中保存的结点坐标，那么
 * 随后必须在网格片策略类中调用本构件的成员函数 updating()，
 * 它会做三件事情：
 * 1）消除结点坐标在网格片外表面间的误差（可选）
 * 2）填充结点坐标的影像区
 * 3）清除缓存的边心、面心、体心坐标，以让这些坐标在用户获取
 *    时重新基于结点坐标进行计算
 *
 * 该构件类使用方法如下:
 * -# 调用构造函数, 创建构件. \n
 *    创建时, 需要提供标准构件网格片策略类
 *    algs::StandardComponentPatchStrategy 的派生类对象.
 * -# 消除结点坐标外表面误差，更新影像区结点坐标,
 *清除缓存的边心、面心、体心坐标. \n
 *    调用 updating().
 *
 * @see algs::StandardComponentPatchStrategy
 */

template <int DIM>
class UpdateCoordinateComponent : public algs::IntegratorComponent<DIM> {
 public:
  /**
   * @brief 构造函数.
   *
   * @param object_name    输入参数, 字符串, 对象名.
   * @param manager        输入参数, 指针, 指向积分构件管理器.
   */
  UpdateCoordinateComponent(const string& object_name,
                            algs::IntegratorComponentManager<DIM>* manager)
      : IntegratorComponent<DIM>(object_name, manager) {
    manager->registerComponent(this);
  }

  /**
   * @brief 析构函数.
   */
  virtual ~UpdateCoordinateComponent() {}

  ///@cond DEV

  /**
   * @brief 初始化对象.
   * 该函数实现基类 algs::IntegratorComponent 的纯虚成员函数.
   */
  virtual void initialize() {}

  ///@endcond

  /**
   * @brief 填充更新后的结点坐标到相邻网格片的影像区.
   * @param level 输入参数, 指针, 指向网格层.
   * @param erase_error, 输入参数, 布尔值，真值表示消除坐标在网格片间的误差.
   */
  void updating(const HSF::tbox::Pointer<hier::BasePatchLevel<DIM>> level,
                const bool erase_error = true) {
    hier::PatchLevel<DIM>* pl =
        dynamic_cast<hier::PatchLevel<DIM>*>(level.getPointer());
    HSF::tbox::Pointer<hier::Patch<DIM>> pch = pl->getPatch(0);
    HSF::tbox::Pointer<hier::PatchGeometry<DIM>> pch_geo =
        pch->getPatchGeometry();
    HSF::tbox::Pointer<pdat::NodeData<DIM, double>> node_coord =
        pch_geo->getNodeCoordinates();

    Region* mesh = pl->getRegion();
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(mesh->getMesh());

    /*! 更新中的坐标 */
    if (DIM == 2) {
      Tensor1<double>* fc = uMesh->nodeCoords();
      for (int i = 0; i < uMesh->node().getNum(); i++) {
        fc[i][0] = (*node_coord)(0, i);
        fc[i][1] = (*node_coord)(1, i);
      }
    } else {
      Tensor1<double>* fc = uMesh->nodeCoords();
      for (int i = 0; i < uMesh->node().getNum(); i++) {
        fc[i][0] = (*node_coord)(0, i);
        fc[i][1] = (*node_coord)(1, i);
        fc[i][2] = (*node_coord)(2, i);
      }
    }

    /*! 影像区与share的同步 */
    uMesh->updateNodeCoord();

    /*! 删除PatchGeometry中的指针 */
    pch_geo->d_node_coordinates.setNull();
    pch_geo->d_edge_coordinates.setNull();
    pch_geo->d_face_coordinates.setNull();
    pch_geo->d_cell_coordinates.setNull();
    if (pch_geo->d_node_coordinates_id > 0)
      mesh->deleteField(pch_geo->d_node_coordinates_id);
    if (pch_geo->d_edge_coordinates_id > 0)
      mesh->deleteField(pch_geo->d_edge_coordinates_id);
    if (pch_geo->d_face_coordinates_id > 0)
      mesh->deleteField(pch_geo->d_face_coordinates_id);
    if (pch_geo->d_cell_coordinates_id > 0)
      mesh->deleteField(pch_geo->d_cell_coordinates_id);
  }
};

}  // namespace algs
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFUPDATECOORDINATECOMPONENT_H \
        */
