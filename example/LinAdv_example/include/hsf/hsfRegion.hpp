/*! \file hsfRegion.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2024-02-20
 *  \brief field,equation management
 */

#ifndef SRC_REGION_REGION_HPP
#define SRC_REGION_REGION_HPP

#include <typeindex>
#include <typeinfo>

#include "hsfField.hpp"

namespace HSF {

class Region {
 protected:
  Word regName_;  ///< 计算域名
  Mesh* mesh_;
  label fieldNum_;
  label idcount_;
  map<Word, map<std::pair<SetType, TIMESTEP>, int> > fieldsMap_;
  map<int, void*> fields_;
  map<int, std::type_index> fieldsType_;
  std::map<int, Word> plotFields_;

 public:
  /*-----------------------------构造析构----------------------------*/
  /// 空构造
  Region(const Word& name, Mesh* mesh);

  /// 析构
  virtual ~Region();

  /*-------------------------基本信息获取----------------------------*/
  /// 获取计算域上的网格对象引用
  Word getName() const { return regName_; }
  /// 获取计算域上的网格对象引用
  Mesh* getMesh() { return mesh_; };

  /*-------------------------场操作----------------------------*/
  /// 获得计算域上场的数量
  label getFieldNum() { return fieldNum_; };

  /// 增加物理场
  template <typename T>
  int addField(Field<T>* f);

  /// 删除场
  void deleteField(int index);
  /// 将场移出region的管理
  void removeField(int index);

  /// 获取物理场
  int getFieldIndex(Word name, SetType type, TIMESTEP t = TIMESTEP::N) const;
  template <typename T>
  Field<T>& getField(int index);
  template <typename T>
  Field<T>& getField(Word name, SetType type, TIMESTEP t = TIMESTEP::N);

  /*! 为场更新网格指针 */
  void setMeshForFields();

  /*! 为场分配空间 */
  void allocateField(int index, bool hasGhost);
  void allocateField(int index);
  void allocateFields(bool forceUpdate = false);

  /*! 设置field的时间戳 */
  void setFieldTimeStamp(scalar t, int index);

  /*! 场空间销毁 */
  void deallocateField(int index);
  void deallocateFields();

  /*! 复制场 */
  void copyField(int from, int to);

  /// 启动进程边界场更新
  void updateFieldBegin(int index);
  /// 完成更新场
  void updateFieldEnd(int index);
  /// 启动进程边界场reduce
  void reduceFieldBegin(int index);
  /// 完成场reduce
  void reduceFieldEnd(int index, Word op);
  /// 输出注册的场信息
  void printFields(ostream& os = COUT);

  /// 更新所有场，谨慎使用（可能有大量通信）
  void updateAllFields();

  /*-------------------------子域操作----------------------------*/
  /*-------------------------方程操作----------------------------*/
  /*-------------------------拓扑迭代与过程栈----------------------------*/
  /*-----------------负载均衡--------------------*/

  /*--------------数据存储-----------------*/
  /*! 注册绘图量 */
  void registerFieldPlot(int index, Word name = "");

  /*! layout 0: one xdmf and one h5 file */
  void plotData(label tiemstep, scalar time, int layout = 0,
                bool replotMesh = false);

};  // end Region

}  // end namespace HSF

#endif /* end of include guard: SRC_REGION_REGION_HPP */
