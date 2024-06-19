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

class SubSet {
 private:
  std::string name_; /*!< subet名称 */
  Mesh* mesh_;
  label num_;                /*!< subset元素个数, 包含ghost */
  label realNum_;            /*!< subset元素个数, 不包含ghost */
  label ghostNum_;           /*!< ghost数量 */
  SetType type_;             /*!< subset类型 */
  int id_;                   /*!< subset对应id */
  std::vector<label> index_; /*!< subset index(局部编号) */
  static int count_;

 public:
  SubSet();
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
