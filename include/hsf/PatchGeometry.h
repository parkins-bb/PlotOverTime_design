#ifndef SOURCE_WRAPPER_HSFPATCHGEOMETRY_H
#define SOURCE_WRAPPER_HSFPATCHGEOMETRY_H

#include "Array.h"
#include "PatchData.h"
#include "Pointer.h"
#include "hsfField.hpp"
#include "hsfRegion.hpp"
#include "ustrMesh.hpp"
#include "utilityExceptions.h"

namespace HSF {

namespace algs {
template <int DIM>
class UpdateCoordinateComponent;
}

namespace hier {

/**
 * @brief 网格片几何类 hier::PatchGeometry
 *为非结构网格描述网格片的几何信息和物理边界条件信息。
 *
 */

template <int DIM>
class PatchGeometry {
  friend algs::UpdateCoordinateComponent<DIM>;

 public:
  PatchGeometry(Region* mesh) { d_mesh = mesh; }
  ~PatchGeometry() {}

  /**
   * @brief 获取单元中心坐标。
   * @param update 输入参数，逻辑型，真值表示根据结点坐标更新单元中心坐标。
   * @return 指针，指向存放单元中心坐标的数据片。
   */
  HSF::tbox::Pointer<pdat::CellData<DIM, double> > getCellCoordinates() {
    if (d_cell_coordinates.isNull()) {
      if (DIM == 2) {
        Field<double>* f = new Field<double>(
            "cellCoord", SetType::FACE, TIMESTEP::NONE, 2, d_mesh->getMesh());
        d_cell_coordinates_id = d_mesh->addField(f);
        f->setHasGhost(true);
        d_mesh->allocateField(d_cell_coordinates_id);
        Tensor1<double>* fc = d_mesh->getMesh()->faceCenters();
        for (int i = 0; i < d_mesh->getMesh()->face().getNum(); i++) {
          (*f)(i, 0) = fc[i][0];
          (*f)(i, 1) = fc[i][1];
        }
        d_cell_coordinates = new pdat::CellData<DIM, double>(f);
      } else {
        Field<double>* f = new Field<double>(
            "cellCoord", SetType::CELL, TIMESTEP::NONE, 3, d_mesh->getMesh());
        d_cell_coordinates_id = d_mesh->addField(f);
        f->setHasGhost(true);
        d_mesh->allocateField(d_cell_coordinates_id);
        Tensor1<double>* fc = d_mesh->getMesh()->cellCenters();
        for (int i = 0; i < d_mesh->getMesh()->cell().getNum(); i++) {
          (*f)(i, 0) = fc[i][0];
          (*f)(i, 1) = fc[i][1];
          (*f)(i, 2) = fc[i][2];
        }
        d_cell_coordinates = new pdat::CellData<DIM, double>(f);
      }
    }
    return d_cell_coordinates;
  }

  /**
   * @brief 获取结点坐标。
   * @return 指针，指向存放结点坐标的数据片。
   */
  HSF::tbox::Pointer<pdat::NodeData<DIM, double> > getNodeCoordinates() {
    if (d_node_coordinates.isNull()) {
      if (DIM == 2) {
        Field<double>* f = new Field<double>(
            "nodeCoord", SetType::NODE, TIMESTEP::NONE, 2, d_mesh->getMesh());
        d_node_coordinates_id = d_mesh->addField(f);
        f->setHasGhost(true);
        d_mesh->allocateField(d_node_coordinates_id);
        Tensor1<double>* fc = d_mesh->getMesh()->nodeCoords();
        for (int i = 0; i < d_mesh->getMesh()->node().getNum(); i++) {
          (*f)(i, 0) = fc[i][0];
          (*f)(i, 1) = fc[i][1];
        }
        d_node_coordinates = new pdat::NodeData<DIM, double>(f);

      } else {
        Field<double>* f = new Field<double>(
            "nodeCoord", SetType::NODE, TIMESTEP::NONE, 3, d_mesh->getMesh());
        d_node_coordinates_id = d_mesh->addField(f);
        f->setHasGhost(true);
        d_mesh->allocateField(d_node_coordinates_id);
        Tensor1<double>* fc = d_mesh->getMesh()->nodeCoords();
        for (int i = 0; i < d_mesh->getMesh()->node().getNum(); i++) {
          (*f)(i, 0) = fc[i][0];
          (*f)(i, 1) = fc[i][1];
          (*f)(i, 2) = fc[i][2];
        }
        d_node_coordinates = new pdat::NodeData<DIM, double>(f);
      }
    }
    return d_node_coordinates;
  }

  /**
   * @brief 获取边中点坐标。
   * @return 指针，指向存放边中点坐标的数据片。
   */
  HSF::tbox::Pointer<pdat::EdgeData<DIM, double> > getEdgeCoordinates() {
    if (d_edge_coordinates.isNull()) {
      if (DIM == 2) {
        Field<double>* f = new Field<double>(
            "edgeCoord", SetType::EDGE, TIMESTEP::NONE, 2, d_mesh->getMesh());
        d_edge_coordinates_id = d_mesh->addField(f);
        f->setHasGhost(true);
        d_mesh->allocateField(d_edge_coordinates_id);
        Tensor1<double>* fc = d_mesh->getMesh()->edgeCenters();
        for (int i = 0; i < d_mesh->getMesh()->edge().getNum(); i++) {
          (*f)(i, 0) = fc[i][0];
          (*f)(i, 1) = fc[i][1];
        }
        d_edge_coordinates = new pdat::EdgeData<DIM, double>(f);

      } else {
        Field<double>* f = new Field<double>(
            "edgeCoord", SetType::EDGE, TIMESTEP::NONE, 3, d_mesh->getMesh());
        d_edge_coordinates_id = d_mesh->addField(f);
        f->setHasGhost(true);
        d_mesh->allocateField(d_edge_coordinates_id);
        Tensor1<double>* fc = d_mesh->getMesh()->edgeCenters();
        for (int i = 0; i < d_mesh->getMesh()->edge().getNum(); i++) {
          (*f)(i, 0) = fc[i][0];
          (*f)(i, 1) = fc[i][1];
          (*f)(i, 2) = fc[i][2];
        }
        d_edge_coordinates = new pdat::EdgeData<DIM, double>(f);
      }
    }
    return d_edge_coordinates;
  }

  /**
   * @brief 获取面中心坐标。
   * @return 指针，指向存放面中心坐标的数据片。
   */
  HSF::tbox::Pointer<pdat::FaceData<DIM, double> > getFaceCoordinates() {
    ASSERT(DIM == 3);
    if (d_face_coordinates.isNull()) {
      Field<double>* f = new Field<double>(
          "faceCoord", SetType::FACE, TIMESTEP::NONE, 3, d_mesh->getMesh());
      d_face_coordinates_id = d_mesh->addField(f);
      f->setHasGhost(true);
      d_mesh->allocateField(d_face_coordinates_id);
      Tensor1<double>* fc = d_mesh->getMesh()->faceCenters();
      for (int i = 0; i < d_mesh->getMesh()->face().getNum(); i++) {
        (*f)(i, 0) = fc[i][0];
        (*f)(i, 1) = fc[i][1];
        (*f)(i, 2) = fc[i][2];
      }
      d_face_coordinates = new pdat::FaceData<DIM, double>(f);
    }
    return d_face_coordinates;
  }

  /**
   * @brief 获取指定类型的网格实体坐标。
   * @param entity_type, 枚举类型，可选的值有CELL，NODE，EDGE，FACE。
   * @return 指针，指向存放网格实体坐标数据片。
   */
  HSF::tbox::Pointer<PatchData<DIM> > getCoordinates(
      EntityUtilities::EntityType entity_type) {
    switch (entity_type) {
      case (EntityUtilities::EntityType::CELL):
        return getCellCoordinates();
      case (EntityUtilities::EntityType::NODE):
        return getNodeCoordinates();
      case (EntityUtilities::EntityType::EDGE):
        return getEdgeCoordinates();
      case (EntityUtilities::EntityType::FACE):
        return getFaceCoordinates();
      case (EntityUtilities::EntityType::INVALID_ENTITY):
        return NULL;
    }
  }
  //@}

  /**
   * @brief 根据参数数组所给的结点坐标设置当前网格片的结点的坐标。
   * @param node_coordinates 输入参数，双精度型数组，存储结点坐标。
   * @note
   *  - 二维情况下，node_coordinates[2*i]和node_coordinates[2*i+1]表示
   *    第i个结点的坐标。三维情况类似。
   */
  // void setNodeCoordinates( HSF::tbox::Pointer<pdat::NodeData<DIM, double>
  // > node_coord) {}

  void cleanCellCoordinates() {}

  void cleanEdgeCoordinates() {}

  void cleanFaceCoordinates() {}
  /*!
   * @brief 获取当前网格片上定义的网格实体集合的数目。
   *
   * @return 整型，当前网格片上定义的网格实体集合的数目。
   */
  // int getNumberOfEntitySet() const {}

  /*!
   * @brief 判断当前网格片上是否有指定的网格实体集合。
   *
   * @param set_id   输入参数，整型，网格实体集合的ID。
   * @param set_type 输入参数，枚举型，网格实体集合的类别。
   *
   * @return 逻辑型，真值表示当前网格片上有指定ID和类别的网格实体集合。
   */
  bool hasEntitySet(const int set_id, EntityUtilities::EntityType set_type,
                    const int width = 0) const {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    const SubSet* entity =
        uMesh->getSubSet(set_id, EntityUtilities::entity2setType(set_type));
    if (entity == NULL) return false;
    return !entity->getArray().empty();
  }

  /*!
   * @brief 判断当前网格片上是否有指定的网格实体集合。
   *
   * @param set_id     输入参数，整型，网格实体集合的ID。
   * @param set_type   输入参数，枚举型，网格实体集合的类别。
   * @param num_entity 输入参数，整型，网格实体的数目。
   *
   * @return 逻辑型，真值表示当前网格片上有指定ID和类别的网格实体集合中的实体，
   *         并且这些网格实体的索引的最大值小于参数num_entity。
   *
   * @note 参数num_entity用于指定获取的网格实体的范围。通常该参数取值
   *       为网格片内部网格实体数目或者网格片内部加上一层影像区后的
   *       网格实体数目。
   */
  // bool hasEntitySet(const int set_id, EntityUtilities::EntityType set_type,
  // const int num_entity) const {}

  /*!
   * @brief 获取指定集合中的网格实体的索引。
   *
   * @param set_id   输入参数，整型，网格实体集合的ID。
   * @param set_type 输入参数，枚举型，网格实体集合的类别。
   *
   * @return 整型数组，指定ID和类别的集合包含的网格实体的索引。
   */
  const HSF::tbox::Array<int>& getEntityIndicesInSet(
      const int set_id, EntityUtilities::EntityType entity_type) const {
    USTRMesh* uMesh = dynamic_cast<USTRMesh*>(d_mesh->getMesh());
    const SubSet* entity =
        uMesh->getSubSet(set_id, EntityUtilities::entity2setType(entity_type));
    // if (d_subsets.find(set_id) == d_subsets.end()) {
    if (entity == NULL) {
      d_subsets[set_id].resizeArray(0);
    } else {
      const std::vector<int>& ent = entity->getArray();
      d_subsets[set_id] = ent; /*!< shadow copy */
    }
    //}
    return d_subsets[set_id];
  }

  /*!
   * @brief 获取指定集合中的网格实体的索引。
   *
   * @param set_id     输入参数，整型，网格实体集合的ID。
   * @param set_type   输入参数，枚举型，网格实体集合的类别。
   * @param entity_index_upper_bound 输入参数，整型，实体索引号上限。
   *
   * @return 整型数组，指定ID和类别的集合包含的网格实体的索引。
   *
   * @note
   *参数entity_index_upper_bound用于指定获取的网格实体的范围。通常该参数取值
   *       为网格片内部网格实体数目或者网格片内部加上一层影像区后的
   *       网格实体数目。
   */
  // const std::vector<int>& getEntityIndicesInSet( const int set_id,
  // EntityUtilities::EntityType, const int entity_index_upper_bound) const {}

  /*!
   * @brief 判断某实体是否在某个实体集里。
   *
   * @param entity_id
   * @param set_id   输入参数，整型，网格实体集合的ID。
   * @param set_type 输入参数，枚举型，网格实体集合的类别。
   *
   * @return 整型数组，指定ID和类别的集合包含的网格实体的索引。
   */
  // bool findEntityInSet(const int entity_id, const int set_id,
  // EntityUtilities::EntityType entity_type) const {}

  /*!
   * @brief 清除单元中心、边中点和面中心坐标。
   *
   * @note 在有些应用中，比如Lagrangian方法应用，网格结点坐标会更新。
   *       此时，由旧结点坐标计算出的单元中心、边中点、面中心坐标失效，需要清除。
   */
  void cleanCoordinates() {}

  /*!
   * @brief 清除网格片边界。
   */
  void cleanPatchBoundary() {}

  /*!
   * @brief 设置结点标识。
   */
  // void setNodeIdentity(tbox::Pointer<pdat::NodeData<DIM, char> > node_id) {}

  /*!
   * @brief 获取结点标识数据片。
   */
  // tbox::Pointer<pdat::NodeData<DIM, char> > getNodeIdentity() {}

  /*!
   * @brief 设置单元标识。
   */
  // void setCellPart(tbox::Pointer<pdat::CellData<DIM, int> > cell_part){}

  /*!
   * @brief 获取单元标识数据片。
   */
  HSF::tbox::Pointer<pdat::CellData<DIM, int> > getCellPart() {
    static int cell_part_id = -1;
    if (d_cell_part.isNull()) {
      Field<int>* f = new Field<int>("cellpart", SetType::CELL, TIMESTEP::NONE,
                                     1, d_mesh->getMesh());
      cell_part_id = d_mesh->addField(f);
      f->setHasGhost(true);
      d_mesh->allocateField(cell_part_id);

      const std::vector<int>& id = d_mesh->getMesh()->getAttribute();
      for (int i = 0; i < d_mesh->getMesh()->cell().getNum(); i++) {
        (*f)(i, 0) = id[i];
      }
      d_cell_part = new pdat::CellData<DIM, int>(f);
    }
    return d_cell_part;
  }

 private:
  Region* d_mesh;
  /// @cond DEV
  void setCellCoordinates();
  void setEdgeCoordinates();
  void setFaceCoordinates();

  HSF::tbox::Pointer<pdat::CellData<DIM, double> >
      d_cell_coordinates; /**< 单元中心坐标 */
  HSF::tbox::Pointer<pdat::NodeData<DIM, double> >
      d_node_coordinates; /**< 结点坐标 */
  HSF::tbox::Pointer<pdat::EdgeData<DIM, double> >
      d_edge_coordinates; /**< 边中心坐标 */
  HSF::tbox::Pointer<pdat::FaceData<DIM, double> >
      d_face_coordinates; /**< 面中心坐标 */

  mutable std::map<int, HSF::tbox::Array<int> > d_subsets;

  int d_cell_coordinates_id = -1;
  int d_node_coordinates_id = -1;
  int d_edge_coordinates_id = -1;
  int d_face_coordinates_id = -1;

  HSF::tbox::Pointer<pdat::CellData<DIM, int> > d_cell_part; /**< 单元标识 */
};

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFPATCHGEOMETRY_H */
