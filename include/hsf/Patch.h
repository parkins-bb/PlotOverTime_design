#ifndef SOURCE_WRAPPER_HSFPATCH_H
#define SOURCE_WRAPPER_HSFPATCH_H
/*! \file Patch.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <utility>

#include "EntityUtilities.h"
#include "PatchGeometry.h"
#include "PatchTopology.h"
#include "hsfRegion.hpp"
#include "Utilities.h"
#include "VariableDatabase.h"
// #include "Variable.h"

namespace HSF {

namespace hier {

template <int DIM>
class Patch {
 public:
  Patch() {}

  /**
   * @brief 构造函数，基于指定的网格片几何、网格片拓扑创建网格片。
   * @param patch_geometry
   * 输入参数，指针，指向网格片几何类对象，描述网格片的几何信息。
   * @param patch_topology
   * 输入参数，指针，指向网格片拓扑类对象，描述网格片的拓扑信息。
   * @note
   *  该函数并不为定义在网格片上的数据片分配内存空间。
   */
  Patch(HSF::tbox::Pointer<PatchGeometry<DIM>> patch_geometry,
        HSF::tbox::Pointer<PatchTopology<DIM>> patch_topology) {
    d_patch_geometry = patch_geometry;
    d_patch_topology = patch_topology;
    d_reg = patch_topology.getPointer()->d_mesh;
    d_index = d_reg->getMesh()->comm()->getMyId();
  }

  virtual ~Patch() {}

  int getIndex() const { return d_index; }

  HSF::tbox::Pointer<hier::PatchGeometry<DIM>> getPatchGeometry() const {
    return d_patch_geometry;
  }
  HSF::tbox::Pointer<hier::PatchTopology<DIM>> getPatchTopology() const {
    return d_patch_topology;
  }

  int getNumberOfCells(const int width = 0) const {
    if (DIM == 3) {
      if (width)
        return d_reg->getMesh()->cell().getNum();
      else
        return d_reg->getMesh()->cell().getRealNum();
    } else {
      if (width)
        return d_reg->getMesh()->face().getNum();
      else
        return d_reg->getMesh()->face().getRealNum();
    }
  }

  int getNumberOfNodes(const int width = 0) const {
    if (width)
      return d_reg->getMesh()->node().getNum();
    else
      return d_reg->getMesh()->node().getRealNum();
  }

  int getNumberOfEdges(const int width = 0) const {
    if (width)
      return d_reg->getMesh()->edge().getNum();
    else
      return d_reg->getMesh()->edge().getRealNum();
  }

  int getNumberOfFaces(const int width = 0) const {
    if (width)
      return d_reg->getMesh()->face().getNum();
    else
      return d_reg->getMesh()->face().getRealNum();
  }

  int getNumberOfEntities(EntityUtilities::EntityType type,
                          int width = 0) const {
    switch (type) {
      case (EntityUtilities::EntityType::CELL):
        return getNumberOfCells(width);
      case (EntityUtilities::EntityType::NODE):
        return getNumberOfNodes(width);
      case (EntityUtilities::EntityType::EDGE):
        return getNumberOfEdges(width);
      case (EntityUtilities::EntityType::FACE):
        return getNumberOfFaces(width);
      case (EntityUtilities::EntityType::INVALID_ENTITY):
      default:
        return 0;
    }
  }

  HSF::tbox::Pointer<hier::PatchData<DIM>> getPatchData(const int id) {
    hier::VariableDatabase<DIM>* variable_db =
        hier::VariableDatabase<DIM>::getDatabase();

    std::string dtype = variable_db->getFieldType(id);
    std::string loc = variable_db->getFieldLoc(id);
    if (loc == "Cell") {
      if (dtype == typeid(int).name()) {
        Field<int>& f = d_reg->getField<int>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::CellData<DIM, int>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }
      } else if (dtype == typeid(double).name()) {
        Field<double>& f = d_reg->getField<double>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::CellData<DIM, double>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }

      } else if (dtype == typeid(bool).name()) {
        Field<int>& f = d_reg->getField<int>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::CellData<DIM, bool>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }
      }
    } else if (loc == "Face") {
      if (dtype == typeid(int).name()) {
        Field<int>& f = d_reg->getField<int>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::FaceData<DIM, int>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }

      } else if (dtype == typeid(double).name()) {
        Field<double>& f = d_reg->getField<double>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::FaceData<DIM, double>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }

      } else if (dtype == typeid(bool).name()) {
        Field<int>& f = d_reg->getField<int>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::FaceData<DIM, bool>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }
      }
    } else if (loc == "Edge") {
      if (dtype == typeid(int).name()) {
        Field<int>& f = d_reg->getField<int>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::EdgeData<DIM, int>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }

      } else if (dtype == typeid(double).name()) {
        Field<double>& f = d_reg->getField<double>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::EdgeData<DIM, double>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }

      } else if (dtype == typeid(bool).name()) {
        Field<int>& f = d_reg->getField<int>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::EdgeData<DIM, bool>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }
      }
    } else if (loc == "Node") {
      if (dtype == typeid(int).name()) {
        Field<int>& f = d_reg->getField<int>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::NodeData<DIM, int>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }

      } else if (dtype == typeid(double).name()) {
        Field<double>& f = d_reg->getField<double>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::NodeData<DIM, double>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }

      } else if (dtype == typeid(bool).name()) {
        Field<int>& f = d_reg->getField<int>(id);
        if (d_id2data.find(id) == d_id2data.end()) {
          HSF::tbox::Pointer<hier::PatchData<DIM>> ptr =
              new pdat::NodeData<DIM, bool>(&f);
          d_id2data[id] = ptr;
          return ptr;
        } else {
          return d_id2data[id];
        }
      }
    }
    return NULL;
  }

  // template <typename TYPE>
  // HSF::tbox::Pointer<hier::PatchData<DIM, TYPE> > getPatchData( const
  // HSF::tbox::Pointer<Variable<DIM, TYPE> > variable, const
  // HSF::tbox::Pointer<VariableContext> context) const {}

  /*!
   * @brief 判断当前网格片上是否有指定的网格实体集合。
   *
   * @param set_id   输入参数，整型，网格实体集合的ID。
   * @param set_type 输入参数，枚举型，网格实体集合的类别。
   * @param width 输入参数，整型，表示获取哪些区域的单元的数目。
   *
   * @return 逻辑型，真值表示当前网格片上有指定ID和类别的网格实体集合。
   */
  bool hasEntitySet(const int set_id, EntityUtilities::EntityType set_type,
                    const int width = 0) const {
    return d_patch_geometry->hasEntitySet(set_id, set_type, width);
  }

  /*!
   * @brief 获取指定集合中的网格实体的索引。
   *
   * @param set_id   输入参数，整型，网格实体集合的ID。
   * @param set_type 输入参数，枚举型，网格实体集合的类别。
   * @param width 输入参数，整型，表示获取哪些区域的单元的数目。
   *
   * @return 整型数组，指定ID和类别的集合包含的网格实体的索引。
   */
  const HSF::tbox::Array<int>& getEntityIndicesInSet(
      const int set_id, EntityUtilities::EntityType entity_type,
      const int width = 0) const {
    return d_patch_geometry->getEntityIndicesInSet(set_id, entity_type);
  }

  /*!
   * @brief 判断某实体是否在某个实体集里。
   *
   * @param entity_id
   * @param set_id   输入参数，整型，网格实体集合的ID。
   * @param set_type 输入参数，枚举型，网格实体集合的类别。
   *
   * @return 整型数组，指定ID和类别的集合包含的网格实体的索引。
   */
  bool findEntityInSet(const int entity_id, const int set_id,
                       EntityUtilities::EntityType entity_type) const;

  /**
   * @brief 将指定数据片的时戳设置为指定时刻。
   * @param timestamp 输入参数，双精度浮点型，指定时刻。
   * @param id        输入参数，整型，数据片的索引号。
   */
  void setTime(const double timestamp, const int id) {}

  /**
   * @brief 将所有数据片的时戳设置为指定时刻。
   * @param timestamp 输入参数，双精度浮点型，指定时刻。
   */
  void setTime(const double timestamp) {}

  Region* getRegion() { return d_reg; }

 private:
  int d_index; /*!< 网格片全局索引号 */

  HSF::tbox::Pointer<PatchGeometry<DIM>>
      d_patch_geometry; /*!< 网格片几何，描述网格片的几何信息 */
  HSF::tbox::Pointer<PatchTopology<DIM>>
      d_patch_topology; /*!< 网格片拓扑，描述网格片的拓扑信息 */
  Region* d_reg;

  std::map<int, HSF::tbox::Pointer<hier::PatchData<DIM>>> d_id2data;
};  // namespace hier

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFPATCH_H */
