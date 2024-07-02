#ifndef SOURCE_WRAPPER_HSFVARIABLE_H
#define SOURCE_WRAPPER_HSFVARIABLE_H
/*! \file Variable.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <string>

#include "Pointer.h"
#include "connectivity.hpp"
#include "hsfField.hpp"

using namespace std;

namespace HSF {
namespace hier {

template <int DIM, class TYPE>
class Variable {
 public:
  /**
   * @brief 构造函数.
   * @param name 输入参数, 字符串, 变量名称.
   * @param factory 输入参数, 指针, 指向数据片工厂对象, 指明数据片创建方法.
   * @note 以指定的名字和数据片工厂创建一个新的变量对象, 并赋予其惟一的标识号.
   */
  Variable(const string &name, int group = 1, int depth = 1) {
    ASSERT(depth == 1);
    d_name = name;
    d_group = group;
  }

  std::string getName() const { return d_name; }
  HSF::SetType getSetType() const { return d_type; }
  int getGroup() const { return d_group; }
  /**
   * @brief 析构函数.
   */
  virtual ~Variable() {}

 protected:
  std::string d_name;
  HSF::SetType d_type;
  int d_group;
};

}  // namespace hier

namespace pdat {
template <int DIM, class TYPE>
class CellVariable : public hier::Variable<DIM, TYPE> {
 public:
  CellVariable(const string &name, int group = 1, int depth = 1)
      : hier::Variable<DIM, TYPE>(name, group, depth) {
    this->d_type = DIM == 3 ? SetType::CELL : SetType ::FACE;
  }

  /**
   * @brief 析构函数。
   */
  virtual ~CellVariable() {}

 private:
};
template <int DIM, class TYPE>
class FaceVariable : public hier::Variable<DIM, TYPE> {
 public:
  FaceVariable(const string &name, int group = 1, int depth = 1)
      : hier::Variable<DIM, TYPE>(name, group, depth) {
    this->d_type = SetType::FACE;
  }

  /**
   * @brief 析构函数。
   */
  virtual ~FaceVariable() {}

 private:
};
template <int DIM, class TYPE>
class EdgeVariable : public hier::Variable<DIM, TYPE> {
 public:
  EdgeVariable(const string &name, int group = 1, int depth = 1)
      : hier::Variable<DIM, TYPE>(name, group, depth) {
    this->d_type = SetType::EDGE;
  }

  /**
   * @brief 析构函数。
   */
  virtual ~EdgeVariable() {}

 private:
};
template <int DIM, class TYPE>
class NodeVariable : public hier::Variable<DIM, TYPE> {
 public:
  NodeVariable(const string &name, int group = 1, int depth = 1)
      : hier::Variable<DIM, TYPE>(name, group, depth) {
    this->d_type = SetType::NODE;
  }

  /**
   * @brief 析构函数。
   */
  virtual ~NodeVariable() {}

 private:
};

}  // namespace pdat

}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFVARIABLE_H */
