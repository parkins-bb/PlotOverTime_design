#ifndef REGION_MESH_SUBSET_HPP
#define REGION_MESH_SUBSET_HPP

/*! \file subSet.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2024-02-05
 *  \brief subset(cell/face/edge/node), includes mesh boundary
 */

#include <string>

#include "connectivity.hpp"
#include "utilityType.h"

namespace HSF {
class Mesh;
/*
 * SubSet类用于表示网格的子集，可能是单元(cell),面(face),边(edge)或节点(node)
 * 该类包含子集的各种信息，包括名称、类型、元素数量等。
 * */
class SubSet {
 private:
  std::string name_; /*!< subet名称 */  /*用于标识子集的名称，便于识别和操作*/
  Mesh* mesh_; /*！< 指向Mesh对象的指针*/  /*关联到包含该子集的网格对象*/
  label num_;                /*!< subset元素个数, 包含ghost */  /*存储子集中的元素数量*/
  label realNum_;            /*!< subset元素个数, 不包含ghost */  /*存储实际的元素数量*/
  label ghostNum_;           /*!< ghost数量 */  /*存储ghost元素数量*/
  SetType type_;             /*!< subset类型 */  /*子集的类型，如cell,face,edge,node*/
  int id_;                   /*!< subset对应id */  /*子集的id*/
  std::vector<label> index_; /*!< subset index(局部编号) */  /*子集的局部编号索引，存储子集中元素的局部编号*/
  static int count_;         /*子集对象的计数器，统计子集对象的数量，用于分配唯一的ID*/

 public:
  SubSet();
  /*
   * @biref 
   *
   * */
  SubSet(Mesh* mesh, SetType t, int id, string name,
         const std::vector<label>& lId, label ghostNum);
  virtual ~SubSet();
  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  std::string getName() const { return name_; }
  label getNum() const { return num_; }
  label getRealNum() const { return realNum_; }
  label getGhostNum() const { return ghostNum_; }
  SetType getSetType() const { return type_; }
  int getId() const { return id_; }
  const std::vector<label>& getArray() const { return index_; }
  static void countReset() { count_ = 0; }
  /*! resize subset(keep real part unchanged) */
  void resize(label num);
};
}  // namespace HSF

#endif /* end of include guard: REGION_MESH_SUBSET_HPP */
