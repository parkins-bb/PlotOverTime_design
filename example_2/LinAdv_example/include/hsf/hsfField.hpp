/*! \file field.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-08
 *  \modified 2024-02-20
 *  \brief Field class
 */
#ifndef REGION_FIELD_FIELD_HPP
#define REGION_FIELD_FIELD_HPP

#include <complex>

#include "coefficient.hpp"
#include "connectivity.hpp"
#include "mesh.hpp"
#include "utilities.h"

namespace HSF {
class Region;

/*!  time step info of a field */
typedef enum {
  M3 = -3,
  M2 = -2,
  M1 = -1, /*! t_{n-1} */
  N = 0,   /*!< current */
  P1 = 1,  /*!< t_{n+1} */
  P2 = 2,
  P3 = 3,
  NONE = 100
} TIMESTEP;

/*!
 * @brief Field
 * @tparam T  Field data type
 */
template <typename T>
class Field : public std::vector<T> {
 protected:
  friend class Region;
  Word name_;
  Mesh* mesh_;           /*!< not owned */
  Coefficient<T>* coef_; /*!< not owned */
  SetType type_;         /*!< data location */
  TIMESTEP t_;           /*!< timestep */
  label realNum_;        /*!< real part Num */
  // std::vector<T> tvec_;
  int lengthT_; /*!< field data entity length(auto decided) */
  int length_;  /*!< field data entity true length */
  bool hasGhost_;
  scalar time_; /*!< timestamp */

  void allocateData(bool hasGhost);
  void allocateData();
  void deallocateData();

 public:
  using myType_ = T;
  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  Field(Word name, SetType loc = SetType::DOF, TIMESTEP t = TIMESTEP::NONE,
        int length = -1, Mesh* mesh = NULL, Coefficient<T>* coef = NULL);

  virtual ~Field() {}
  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  /*! field name */
  /*! 获得物理场名称*/
  Word getName() const { return name_; }
  void setName(Word name) { name_ = name; }
  /*! field based mesh */
  /*! 获得物理场依附的网格类*/
  Mesh* getMesh() const { return mesh_; }
  void setMesh(Mesh* mesh) { mesh_ = mesh; }
  /*!< field data location */
  /*! 获得物理场数据位置*/
  SetType getSetType() const { return type_; }
  void setSetType(SetType t) { type_ = t; }
  /*!< field time step */
  /*! 获得时间步信息*/
  TIMESTEP getContext() const { return t_; }
  void setContext(TIMESTEP t) { t_ = t; }
  /*!< field ghost info */
  /*! 判断物理场是否含有影响区*/
  bool getHasGhost() const { return hasGhost_; }
  void setHasGhost(bool torf) { hasGhost_ = torf; }
  /*! 获得物理场本地长度（不含影像区）*/
  label getRealNum() const { return realNum_; }
  /*!< field time stamp */
  scalar getTime() const { return time_; }
  void setTime(scalar t) { time_ = t; }
  
  /*! 获得物理场单个元素长度*/
  label getEntityNum() const { return this->size() / length_; }

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  /*! set field value from coefficient */
  // virtual void projectCoefficient(Coefficient<T>* coef = NULL);
  /*! only boundary values change(only master is guaranteed to be correct) */
  // virtual void projectBdrCoefficient(Coefficient<T>* coef = NULL);
  /*! return internal true-dof vector. If not set yet, build first */
  // std::vector<T>* getTrueVector();
  /*! set field from the given true-dof vector */
  // void setFromTrueVector(const std::vector<T>& tv);
  /*! get value from coordinate and element id */
  virtual T getValue(const Tensor1<scalar>& ip, label elemID) const;
  /*! begin neighbor data exchange */
  void updateBegin();
  /*! finish neighbor data exchange */
  void updateEnd();
  void update();

  /*! begin shared/ghost data reduce */
  void reduceBegin();
  /*! finish shared/ghost data reduce */
  void reduceEnd(const Word op);
  void reduce(const Word op);

  /*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
  Field& operator=(const Field<T>& orig);
  Field& operator=(T vaule);
  /*! 2D index  */
  T& operator()(label i1, label i2);
  // Field& operator=(const std::vector<T>& v);
};

} /* End namespace HSF */
#endif /* end of include guard: REGION_FIELD_FIELD_HPP */
