#ifndef SOURCE_WRAPPER_HSFVARIABLEDATABASE_H
#define SOURCE_WRAPPER_HSFVARIABLEDATABASE_H
/*! \file VariableDatabase.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "Pointer.h"
#include "Variable.h"
#include "hsfField.hpp"
#include "hsfRegion.hpp"
using namespace std;

namespace HSF {
namespace hier {

template <int>
class PatchHierarchy;

class VariableContext {
 public:
  VariableContext(const string& name) {
    d_name = name;
    if (name == "current" || name == "CURRENT") {
      d_index = TIMESTEP::N;
    } else if (name == "new" || name == "NEW") {
      d_index = TIMESTEP::P1;
    } else {
      d_index = TIMESTEP::NONE;
    }
  }
  virtual ~VariableContext() {}
  const string& getName() const { return d_name; }
  TIMESTEP getContext() const { return d_index; }

 private:
  string d_name;    /*!< 变量上下文的名字. */
  TIMESTEP d_index; /*!< 变量上下文的标识号. */
};

/*!
 * @brief 变量数据库类 hier::VariableDatabase
 * 管理变量、变量上下文以及它们和数据片索引号之间的映射关系.
 *
 * 该类主要提供如下功能:
 * - 管理变量和变量上下文.
 * - 创建并维护变量和数据片索引号之间的映射.
 * - 提供用户接口, 指定用于重启动的数据片. 这些数据片将被自动输出到重启动文件.
 *\if DEV
 * - 维护一个网格片描述器类对象, 记录数据片索引号和数据片工厂的映射关系. \endif
 *
 * 该类通过以下方式, 建立变量与数据片索引号之间的映射:
 *  - 为一个<变量, 变量上下文>配对分配一个惟一的数据片索引号.
 *  - 对一个变量, 由用户指定, 或者由该变量数据库类分配一个惟一的数据片索引号,
 *而不考虑上下文.
 *  .
 * \emph 一般情况下, 建议用户采用 <变量, 变量上下文> 方式. \endemph
 *
 * @note
 * - 该类是个孤类, 只有单一实例.
 *   用户只能通过该类的静态成员函数 getDatabase() 获取该类对象.
 * - 变量数据库中, 变量名和变量上下文名字必须都是惟一的.
 *
 * @see hier::Variable, hier::VariableContext, hier::PatchData \if DEV
 * , hier::PatchDescriptor   \endif
 */

template <int DIM>
class VariableDatabase {
  friend PatchHierarchy<DIM>;

 public:
  /*!
   * @brief 获取变量数据库.
   * @return 指针, 指向变量数据库.
   * @note 该函数是用户获取变量数据库的惟一接口函数.
   */
  static VariableDatabase<DIM>* getDatabase() {
    return s_variable_database_instance;
  }

  ///@cond DEV
  /*!
   * @brief 设置变量数据库.
   * @param var_db 指针, 指向变量数据库.
   */
  static void setDatabase(VariableDatabase<DIM>* var_db) {
    s_variable_database_instance = var_db;
  }

  /*!
   * @brief 释放数据库对象.
   */
  static void freeDatabase() { delete s_variable_database_instance; }
  ///@endcond

  ///@name 变量注册与查询函数:
  //@{

  /*!
   * @brief 注册变量到数据库中.
   * @param variable 指针, 指向变量类对象, 表示待注册变量.
   * @note
   * 如果数据库中已存在同名变量, 则该函数打印出错信息并终止运行.
   */
  // virtual void addVariable(const tbox::Pointer<hier::Variable<DIM> >
  // variable){}

  /*!
   * @brief 按名称检索变量.
   *
   * @param name 输入参数, 字符串, 待查变量的名称.
   *
   * @return 指针, 指向与指定名称匹配的变量.
   * @note
   * -# 如果匹配不成功, 则返回空指针.
   * -# 变量数据库中不允许存在同名变量.
   */
  // virtual tbox::Pointer<hier::Variable<DIM> > getVariable( const string&
  // name) const;

  /*!
   * @brief 检查数据库中是否存在指定名称的变量.
   *
   * @param name  输入参数, 字符串, 待查的变量名称.
   *
   * @return 逻辑型, 真值表示变量数据库中存在名称为name的变量对象.
   */
  // virtual bool checkVariableExists(const string& name) const;

  //@}

  ///@name 变量上下文注册与查询函数:
  //@{

  /*!
   * @brief 按名称检索变量上下文.
   *
   * @param name 输入参数, 字符串, 待查找变量上下文的名称.
   *
   * @return 指针, 指向与指定名称匹配的变量上下文对象.
   * @note
   * -# 如果匹配不成功,
   *    则以指定名字创建一个新的变量上下文对象, 并返回其指针.
   * -# 变量数据库中不允许存在同名上下文.
   */
  virtual HSF::tbox::Pointer<hier::VariableContext> getContext(
      const string& name) {
    HSF::tbox::Pointer<hier::VariableContext> cxt =
        new hier::VariableContext(name);
    return cxt;
  }

  /*!
   * @brief 检查数据库中是否存在指定名称的变量上下文.
   *
   * @param name  输入参数, 字符串, 待查的变量上下文名称.
   *
   * @return 逻辑型, 真值表示变量数据库中存在指定名称的变量上下文对象.
   */
  // virtual bool checkContextExists(const string& name) const{}
  //@}

  ///@name 数据片索引分配函数:
  //@{

  /*!
   * @brief 为指定的<变量, 上下文> 配对返回或分配数据片索引号.
   *
   * @param variable 输入参数, 指针, 指向变量.
   * @param context  输入参数, 指针, 指向变量上下文.
   * @param ghosts   输入参数, 整型, 指定数据片的影像区宽度.[缺省为0]
   *
   * @return 整型, 与<variable,context>关联的数据片索引号.
   * @note
   *   - 如果参数指定的<变量, 上下文>
   *配对已被注册，且影像区宽度一致，则返回数据片索引号.
   *   - 如果参数指定的<变量, 上下文> 配对已被注册，但影像区宽度不一致，
   *     则中断程序的执行并输出相关信息.
   *   - 如果参数指定的<变量, 上下文> 配对没有被注册，则分配并返回数据片索引号.
   *
   * 断言检查:
   *  -# variable非空.
   *  -# context非空.
   *  -# ghosts的值大于等于0.
   */
  // template <typename TYPE>
  // int registerVariableAndContext(
  //    const HSF::tbox::Pointer<hier::Variable<DIM, TYPE>> variable,
  //    const HSF::tbox::Pointer<hier::VariableContext> context,
  //    const int& ghosts = 0) {
  //  HSF::Field<TYPE>* fld = variable->getVariable();
  //  fld->setHasGhost(ghosts);
  //  fld->setContext(context->getContext());
  //  int id = d_reg->addField(fld);
  //  d_types.insert(std::make_pair(id, typeid(TYPE).name()));
  //  d_loc.insert(std::make_pair(id, "NONE"));
  //  return id;
  //}
  template <typename TYPE>
  int registerVariableAndContext(
      const HSF::tbox::Pointer<pdat::CellVariable<DIM, TYPE>> variable,
      const HSF::tbox::Pointer<hier::VariableContext> context,
      const int& ghosts = 0) {
    HSF::Field<TYPE>* fld =
        new HSF::Field<TYPE>(variable->getName(), variable->getSetType(),
                             context->getContext(), variable->getGroup());
    fld->setHasGhost(ghosts);
    fld->setContext(context->getContext());
    int id = d_reg->addField(fld);
    d_types.insert(std::make_pair(id, typeid(TYPE).name()));
    d_loc.insert(std::make_pair(id, "Cell"));
    return id;
  }
  template <typename TYPE>
  int registerVariableAndContext(
      const HSF::tbox::Pointer<pdat::FaceVariable<DIM, TYPE>> variable,
      const HSF::tbox::Pointer<hier::VariableContext> context,
      const int& ghosts = 0) {
    HSF::Field<TYPE>* fld =
        new HSF::Field<TYPE>(variable->getName(), variable->getSetType(),
                             context->getContext(), variable->getGroup());

    fld->setHasGhost(ghosts);
    fld->setContext(context->getContext());
    int id = d_reg->addField(fld);
    d_types.insert(std::make_pair(id, typeid(TYPE).name()));
    d_loc.insert(std::make_pair(id, "Face"));
    return id;
  }
  template <typename TYPE>
  int registerVariableAndContext(
      const HSF::tbox::Pointer<pdat::EdgeVariable<DIM, TYPE>> variable,
      const HSF::tbox::Pointer<hier::VariableContext> context,
      const int& ghosts = 0) {
    HSF::Field<TYPE>* fld =
        new HSF::Field<TYPE>(variable->getName(), variable->getSetType(),
                             context->getContext(), variable->getGroup());

    fld->setHasGhost(ghosts);
    fld->setContext(context->getContext());
    int id = d_reg->addField(fld);
    d_types.insert(std::make_pair(id, typeid(TYPE).name()));
    d_loc.insert(std::make_pair(id, "Edge"));
    return id;
  }
  template <typename TYPE>
  int registerVariableAndContext(
      const HSF::tbox::Pointer<pdat::NodeVariable<DIM, TYPE>> variable,
      const HSF::tbox::Pointer<hier::VariableContext> context,
      const int& ghosts = 0) {
    HSF::Field<TYPE>* fld =
        new HSF::Field<TYPE>(variable->getName(), variable->getSetType(),
                             context->getContext(), variable->getGroup());
    fld->setHasGhost(ghosts);
    fld->setContext(context->getContext());
    int id = d_reg->addField(fld);
    d_types.insert(std::make_pair(id, typeid(TYPE).name()));
    d_loc.insert(std::make_pair(id, "Node"));
    return id;
  }

  std::string getFieldType(int id) const { return d_types.find(id)->second; }
  std::string getFieldLoc(int id) const { return d_loc.find(id)->second; }
  /*!
   * @brief 为指定变量分配数据片索引号.
   *
   * @param  variable 输入参数, 指针, 指向变量.
   * @param  data_id  输入参数, 整型, 数据片索引号.[缺省为-1]
   * @return 整型, 为变量分配的数据片索引号.
   * @note
   *    若参数 data_id 缺省, 则该函数将为变量 variable 分配新的数据片索引号;
   *    否则, 指定变量 variable 的数据片索引号为 data_id. 对后者,
   *    如果数据片索引号 data_id 对应的原始变量和变量 variable 类型不一致,
   *    则该函数打印错误信息并中止运行.
   */
  // virtual int registerPatchDataIndex( const
  // HSF::tbox::Pointer<hier::Variable<DIM> > variable, int data_id = -1);

  /// @cond DEV

  /*!
   * @brief 为指定变量分配数据片索引号,
   *并用另一索引号对应的数据片工厂替代默认工厂.
   *
   * @param  variable 输入参数, 指针, 指向变量.
   * @param  old_id   输入参数, 整型, 已有的数据片索引号.
   *
   * @return 整型, 变量分配到的数据片索引号. 如果分配不成功, 则返回无效值(< 0).
   * @note
   * - 变量 variable 将采用与索引号 old_id 对应的数据片工厂创建其数据片.
   * - 输入参数说明:
   *   - 如果指定的变量不在数据库中, 则该函数将它添加到数据库;
   *   - 如果数据库中存在同名变量, 则打印出错信息并中止运行;
   *   - 如果数据片索引号 old_id 非法, 或者其与变量的类型不匹配,
   *     则打印错误信息后中止运行.
   */
  // virtual int registerClonedPatchDataIndex( const
  // HSF::tbox::Pointer<hier::Variable<DIM> > variable, int old_id);

  /*!
   * @brief 注销数据片索引号, 取消其与任何变量或<变量, 变量上下文>的映射关系.
   * @param  data_id  输入参数, 整型, 待注销的数据片索引.
   */
  // virtual void removePatchDataIndex(int data_id);

  ///@name 数据片索引关联查询函数:
  //@{

  /*!
   * @brief 检查变量与数据片是否匹配.
   *
   * @param  variable  输入参数, 指针, 指向待查变量.
   * @param  data_id   输入参数, 整型, 待查数据片的索引号.
   *
   * @return 逻辑型, 真值表示两者类型匹配.
   */
  // virtual bool checkVariablePatchDataIndex( const
  // HSF::tbox::Pointer<hier::Variable<DIM> > variable, int data_id);
  //@}

  /*!
   * @brief 打印该类对象的内部数据.
   *
   * @param os 输入参数, 输出流对象, 默认为并行日志流.
   * @param print_only_user_defined_variables 输入参数,
   *                   真值表示只输出用户变量的相关信息.
   * @note 该函数打印变量数据库中所有变量、
   * 变量上下文和数据片索引号映射信息等数据到指定输出流.
   */
  void printClassData(ostream& os = (*POUT.stream())) const {
    d_reg->printFields(os);
  }

 protected:
  /**
   * @brief构造函数, 初始化变量数据库.
   */
  VariableDatabase(Region* reg) { d_reg = reg; }

  /**
   * @brief 析构函数, 释放变量数据库所有内部数据空间.
   */
  virtual ~VariableDatabase() { freeDatabase(); }

 private:
  /**
   * @brief 向变量数据库添加一个变量.
   * @return 逻辑型, 真值表示添加成功.
   * @note
   * - 若变量已注册, 则返回真.
   * - 若数据库中不存在同名变量, 则添加该变量, 并返回真.
   * - 若数据库中已存在其他同名变量, 则
   *   - 若变量为用户变量, 则不执行任何有效操作, 并返回假.
   *   - 若变量为内部工作变量, 用该变量覆盖原有同名变量.
   */

  // bool addVariable_Private(const HSF::tbox::Pointer<hier::Variable<DIM> >
  // variable, bool user_variable);

  static VariableDatabase<DIM>*
      s_variable_database_instance; /*!< 变量数据库单一实例指针. */

  int d_max_context_id; /*!<  变量上下文标识号的最大值.*/

  std::map<int, std::string> d_types;

  std::map<int, std::string> d_loc;

  Region* d_reg;
};
template <>
template <>
int VariableDatabase<3>::registerVariableAndContext<bool>(
    const HSF::tbox::Pointer<pdat::CellVariable<3, bool>> variable,
    const HSF::tbox::Pointer<hier::VariableContext> context, const int& ghosts);
template <>
template <>
int VariableDatabase<3>::registerVariableAndContext<bool>(
    const HSF::tbox::Pointer<pdat::FaceVariable<3, bool>> variable,
    const HSF::tbox::Pointer<hier::VariableContext> context, const int& ghosts);
template <>
template <>
int VariableDatabase<3>::registerVariableAndContext<bool>(
    const HSF::tbox::Pointer<pdat::EdgeVariable<3, bool>> variable,
    const HSF::tbox::Pointer<hier::VariableContext> context, const int& ghosts);
template <>
template <>
int VariableDatabase<3>::registerVariableAndContext<bool>(
    const HSF::tbox::Pointer<pdat::NodeVariable<3, bool>> variable,
    const HSF::tbox::Pointer<hier::VariableContext> context, const int& ghosts);

template <>
template <>
int VariableDatabase<2>::registerVariableAndContext<bool>(
    const HSF::tbox::Pointer<pdat::CellVariable<2, bool>> variable,
    const HSF::tbox::Pointer<hier::VariableContext> context, const int& ghosts);
template <>
template <>
int VariableDatabase<2>::registerVariableAndContext<bool>(
    const HSF::tbox::Pointer<pdat::FaceVariable<2, bool>> variable,
    const HSF::tbox::Pointer<hier::VariableContext> context, const int& ghosts);
template <>
template <>
int VariableDatabase<2>::registerVariableAndContext<bool>(
    const HSF::tbox::Pointer<pdat::EdgeVariable<2, bool>> variable,
    const HSF::tbox::Pointer<hier::VariableContext> context, const int& ghosts);
template <>
template <>
int VariableDatabase<2>::registerVariableAndContext<bool>(
    const HSF::tbox::Pointer<pdat::NodeVariable<2, bool>> variable,
    const HSF::tbox::Pointer<hier::VariableContext> context, const int& ghosts);

template <int DIM>
VariableDatabase<DIM>* VariableDatabase<DIM>::s_variable_database_instance =
    NULL; /*! define */

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFVARIABLEDATABASE_H */
