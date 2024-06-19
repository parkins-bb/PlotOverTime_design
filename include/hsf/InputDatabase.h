//
// 文件名:    InputDatabase.h
// 软件包:    JAUMIN toolbox
// 版权  :	(c) 2004-2010 北京应用物理与计算数学研究所
// 版本号:    $Revision: 257 $
// 修改  :    $Date: 2012-02-16 12:55:07 +0800 (四, 2012-02-16) $
// 描述  : 输入数据库类, 提供存储和访问输入数据的接口

#ifndef included_tbox_InputDatabase
#define included_tbox_InputDatabase

#ifndef included_JAUMIN_config
#include "JAUMIN_config.h"
#endif
#ifndef included_tbox_Database
#include "Database.h"
#endif
#include <list>

namespace HSF {
namespace tbox {

/**
 * @brief 输入数据库类 tbox::InputDatabase, 提供存储和访问输入数据的接口.
 *
 * 该数据库是类 tbox::Database 的派生类, 具体实现了基类中的接口函数.
 * 通常, 用户需通过类 tbox::Database 的接口使用该类中的函数.
 * 因此, 为了文档的简洁, 隐藏了该类中的函数, 具体的函数介绍请参见类
 * tbox::Database .
 *
 * 用户在使用时, 请注意:
 * - 输入数据库将根据需要提升数值类型, 提升级别为:
 *   int -> float -> double -> complex.
 *    - 如果关键字对应的值为整型数据,
 *      那么调用 getComplex() 时会将其提升为复数.
 *    - 如果对双精度浮点数执行参数为单精度浮点类型的函数,
 *      那么双精度浮点数会被截断成单精度浮点型数.
 * - 该类假设所有进程采用同样的方式访问数据库,
 *   因此缺省情形下将所有的错误信息输出到 tbox::pout 流, 而不是 tbox::perr 流.
 */

class InputDatabase : public Database {
 public:
  ///@cond DEV
  /**
   * @brief 构造函数.
   *
   * @param name 输入参数, 字符串, 表示数据库的名字.
   */
  InputDatabase(const string& name) { d_database_name = name; }

  /**
   * @brief 析构函数.
   */
  ~InputDatabase();

  /**
   * @brief 获取本数据库对象的名称.
   *
   * @return 字符串, 返回本数据库对象的名称.
   */
  string getName() const { return (d_database_name); }

  /**
   * @brief 在当前数据库中查找指定关键字.
   *
   * @param key 输入参数, 字符串, 表示关键字.
   *
   * @return 逻辑型, 真值表示在当前数据库中该关键字存在.
   */
  bool keyExists(const string& key);
  /**
   * @brief 查询指定关键字是否已被某查询成员函数访问.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示已被查询, 假值表示没有或关键字不存在.
   */
  bool keyAccessed(const string& key) {
    KeyData* keydata = findKeyData(key);
    return (keydata ? keydata->d_accessed : false);
  }

  /**
   * @brief 返回一个数组, 包含当前数据库中的所有关键字.
   *
   * @return 数组, 包含当前数据库中的所有关键字.
   */
  Array<string> getAllKeys();

  /**
   * @brief 返回与指定关键字关联的数组的长度.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 整型, 表示数组的长度.
   *
   * @note 如果在当前数据库中该关键字不存在, 则返回零值.
   */
  int getArraySize(const string& key);

  /// @name 获取或存入子数据库.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是数据库类型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是数据库,
   *                 假值表示不是数据库或指定关键字不存在.
   */
  bool isDatabase(const string& key);

  /**
   * @brief 创建一个与指定关键字关联的子数据库
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 指针, 指向新创建的数据库,
   */
  Pointer<Database> putDatabase(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的子数据库
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 指针, 指向已读取的数据库,
   */
  Pointer<Database> getDatabase(const string& key);

  //@}

  /// @name 获取或存入逻辑型数据.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是逻辑型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是逻辑型,
   *                 假值表示不是逻辑型或指定关键字不存在.
   */
  bool isBool(const string& key);

  /**
   * @brief 将一个逻辑型数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 逻辑型, 表示待存入的逻辑型数.
   */
  void putBool(const string& key, const bool& data);

  /**
   * @brief 将一个逻辑型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 逻辑型数组.
   */
  void putBoolArray(const string& key, const Array<bool>& data);

  /**
   * @brief 将指针指向的一个逻辑型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向逻辑型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putBoolArray(const string& key, const bool* const data,
                    const int nelements);

  /**
   * @brief 读取一个与指定关键字关联的逻辑型数. 若数据库中不存在该关键字,
   * 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 表示已读取的逻辑型数,
   *
   * @note 如果在当前数据库中该关键字不存在, 或者与其关联的不是逻辑型数,
   * 则输出错误信息, 并终止程序.
   */
  bool getBool(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的逻辑型数. 若数据库中不存在该关键字,
   * 则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 逻辑型, 表示缺省值.
   *
   * @return 逻辑型, 表示已读取的逻辑型数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是逻辑型数,
   * 则输出错误信息, 并终止程序.
   */
  bool getBoolWithDefault(const string& key, const bool& defaultvalue);

  /**
   * @brief 读取一个与指定关键字关联的逻辑型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是逻辑型数组,
   * 则输出错误信息, 并终止程序.
   */
  Array<bool> getBoolArray(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的逻辑型数组, 并存储在指定数组中.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输出参数, 指针, 指向逻辑型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是逻辑型数组,
   * 或者指定的元素个数和数据库中的数组元素个数不一致,
   * 则输出错误信息, 并终止程序.
   */
  void getBoolArray(const string& key, bool* data, const int nelements);
  //@}

  /// @name 获取或存入数据库box对象.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是数据库box对象.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是数据库box对象,
   *                   假值表示不是数据库对象或指定关键字不存在.
   */
  bool isDatabaseBox(const string& key);

  /**
   * @brief 将单个数据库box存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 数据库box类, 数据库box对象.
   */
  void putDatabaseBox(const string& key, const DatabaseBox& data);

  /**
   * @brief 将数据库box数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 数据库box数组, 存储待存入的数据库box.
   */
  void putDatabaseBoxArray(const string& key, const Array<DatabaseBox>& data);

  /**
   * @brief 将指针指向的数据库box对象数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向数据库box数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putDatabaseBoxArray(const string& key, const DatabaseBox* const data,
                           const int nelements);

  /**
   * @brief 读取一个与指定关键字关联的数据库box对象. 若数据库不存在该关键字,
   * 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 数据库box对象.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是DatabaseBox对象,
   * 则输出错误信息, 并终止程序.
   */
  DatabaseBox getDatabaseBox(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的数据库box对象. 若数据库不存在该关键字,
   * 则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 数据库box对象, 表示缺省值.
   *
   * @return 数据库box类, 数据库box对象.
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是数据库box对象,
   * 则输出错误信息, 并终止程序.
   */
  DatabaseBox getDatabaseBoxWithDefault(const string& key,
                                        const DatabaseBox& defaultvalue);

  /**
   * @brief 读取一个与指定关键字关联的数据库box对象数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 数据库box类, 数据库box数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是数据库box数组,
   * 则输出错误信息, 并终止程序.
   */
  Array<DatabaseBox> getDatabaseBoxArray(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的数据库box对象数组, 并存储在指定数组中.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输出参数, 指针, 指向数据库box对象.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是数据库box对象数组,
   * 或者指定的元素个数和数据库中的数组元素个数不一致,
   * 则输出错误信息, 并终止程序.
   */
  void getDatabaseBoxArray(const string& key, DatabaseBox* data,
                           const int nelements);
  //@}

  /// @name 获取或存入字符型数据.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是字符型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是字符型,
   *                 假值表示不是字符型或指定关键字不存在.
   */
  bool isChar(const string& key);

  /**
   * @brief 将一个字符存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 字符型, 表示待存入的字符.
   */
  void putChar(const string& key, const char& data);

  /**
   * @brief 将一个字符型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 字符型数组.
   */
  void putCharArray(const string& key, const Array<char>& data);

  /**
   * @brief 将指针指向的一个字符型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向字符型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putCharArray(const string& key, const char* const data,
                    const int nelements);

  /**
   * @brief 读取一个与指定关键字关联的字符. 若数据库中不存在该关键字, 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 字符型, 表示已读取的字符,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是字符,
   * 则输出错误信息, 并终止程序.
   */
  char getChar(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的字符. 若数据库中不存在该关键字,
   * 则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 字符型, 表示缺省值.
   *
   * @return 字符型, 表示已读取的字符,
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是字符,
   * 则输出错误信息, 并终止程序.
   */
  char getCharWithDefault(const string& key, const char& defaultvalue);

  /**
   * @brief 读取一个与指定关键字关联的字符型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 字符型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是字符型数组,
   * 则输出错误信息, 并终止程序.
   */
  Array<char> getCharArray(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的字符型数组, 并存储在指定数组中.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输出参数, 指针, 指向字符型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是字符型数组,
   * 或者指定的元素个数和数据库中的数组元素个数不一致,
   * 则输出错误信息, 并终止程序.
   */
  void getCharArray(const string& key, char* data, const int nelements);
  //@}

  /// @name 获取或存入双精度复型数据.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是双精度复型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是双精度复型,
   *                 假值表示不是双精度复型或指定关键字不存在.
   */
  bool isComplex(const string& key);

  /**
   * @brief 将一个双精度复数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 双精度复型, 表示待存入的双精度复数.
   */
  void putComplex(const string& key, const dcomplex& data);

  /**
   * @brief 将一个双精度复型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 双精度复型数组.
   */
  void putComplexArray(const string& key, const Array<dcomplex>& data);

  /**
   * @brief 将指针指向的一个双精度复型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向双精度复型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putComplexArray(const string& key, const dcomplex* const data,
                       const int nelements);

  /**
   * @brief 读取一个与指定关键字关联的双精度复数. 若数据库中不存在该关键字,
   * 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 双精度复型, 表示已读取的双精度复数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度复数,
   * 则输出错误信息, 并终止程序.
   */
  dcomplex getComplex(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的双精度复数. 若数据库中不存在该关键字,
   * 则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 双精度复型, 表示缺省值.
   *
   * @return 双精度浮点型, 表示已读取的双精度复数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是双精度复数,
   * 则输出错误信息, 并终止程序.
   *
   * 复型数据可以从整型、单精度浮点型或双精度浮点型数据提升而来.
   */
  dcomplex getComplexWithDefault(const string& key,
                                 const dcomplex& defaultvalue);

  /**
   * @brief 读取一个与指定关键字关联的双精度复型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 双精度复型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度复型数组,
   * 则输出错误信息, 并终止程序.
   * 复型数据可以从整型、单精度浮点型或双精度浮点型数据提升而来.
   */
  Array<dcomplex> getComplexArray(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的双精度复型数组, 并存储在指定数组中.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输出参数, 指针, 指向双精度复型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度复型数组,
   * 或者指定的元素个数和数据库中的数组元素个数不一致,
   * 则输出错误信息, 并终止程序.
   * 复型数据可以从整型、单精度浮点型或双精度浮点型数据提升而来.
   */
  void getComplexArray(const string& key, dcomplex* data, const int nelements);
  //@}

  /// @name 获取或存入双精度浮点型数据.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是双精度浮点型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是双精度浮点型,
   *                 假值表示不是双精度浮点型或指定关键字不存在.
   */
  bool isDouble(const string& key);

  /**
   * @brief 将一个双精度浮点数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 双精度浮点型, 表示待存入的双精度浮点数.
   */
  void putDouble(const string& key, const double& data);

  /**
   * @brief 将一个双精度浮点型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 双精度浮点型数组.
   */
  void putDoubleArray(const string& key, const Array<double>& data);

  /**
   * @brief 将指针指向的一个双精度浮点型数组存入数据库,
   * 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向双精度浮点型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putDoubleArray(const string& key, const double* const data,
                      const int nelements);

  /**
   * @brief 读取一个与指定关键字关联的双精度浮点数. 若数据库中不存在该关键字,
   * 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 双精度浮点型, 表示已读取的双精度浮点数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度浮点数,
   * 则输出错误信息, 并终止程序.
   *
   * 双精度浮点型数据可以从整型或单精度浮点型数据提升而来.
   */
  double getDouble(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的双精度浮点数. 若数据库中不存在该关键字,
   * 则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 双精度浮点型, 表示缺省值.
   *
   * @return 双精度浮点型, 表示已读取的双精度浮点数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是双精度浮点数,
   * 则输出错误信息, 并终止程序.
   * 双精度浮点型数据可以从整型或单精度浮点型数据提升而来.
   */
  double getDoubleWithDefault(const string& key, const double& defaultvalue);

  /**
   * @brief 读取一个与指定关键字关联的双精度浮点型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 双精度浮点型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度浮点型数组,
   * 则输出错误信息, 并终止程序.
   * 双精度浮点型数据可以从整型或单精度浮点型数据提升而来.
   */
  Array<double> getDoubleArray(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的双精度浮点型数组, 并存储在指定数组中.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输出参数, 指针, 指向双精度浮点型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度浮点型数组,
   * 或者指定的元素个数和数据库中的数组元素个数不一致,
   * 则输出错误信息, 并终止程序.
   * 双精度浮点型数据可以从整型或单精度浮点型数据提升而来.
   */
  void getDoubleArray(const string& key, double* data, const int nelements);

  //@}

  /// @name 获取或存入单精度浮点型数据.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是单精度浮点型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是单精度浮点型,
   *                 假值表示不是单精度浮点型或指定关键字不存在.
   *
   * @note
   * 如果与关键字关联的数据对象为单精度浮点型, 则返回真, 否则返回假.
   * 如果关键字不存在或者为空, 返回假.
   * 单精度浮点型可以从整型提升而来, 或从双精度浮点型截断而得.
   */
  bool isFloat(const string& key);

  /**
   * @brief 将一个单精度浮点数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 单精度浮点型, 表示待存入的单精度浮点数.
   */
  void putFloat(const string& key, const float& data);

  /**
   * @brief 将一个单精度浮点型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 单精度浮点型数组.
   */
  void putFloatArray(const string& key, const Array<float>& data);

  /**
   * @brief 将指针指向的一个单精度浮点型数组存入数据库,
   * 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向单精度浮点型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putFloatArray(const string& key, const float* const data,
                     const int nelements);

  /**
   * @brief 读取一个与指定关键字关联的单精度浮点数. 若数据库中不存在该关键字,
   * 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 单精度浮点型, 表示已读取的单精度浮点数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是单精度浮点数,
   * 则输出错误信息, 并终止程序.
   *
   * 单精度浮点型数据可以从整型数据提升而来, 或从双精度浮点型数据截断而得.
   */
  float getFloat(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的单精度浮点数. 若数据库中不存在该关键字,
   * 则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 单精度浮点型, 表示缺省值.
   *
   * @return 单精度浮点型, 表示已读取的单精度浮点数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是单精度浮点数,
   * 则输出错误信息, 并终止程序.
   *
   * 单精度浮点型数据可以从整型数据提升而来, 或从双精度浮点型数据截断而得.
   */
  float getFloatWithDefault(const string& key, const float& defaultvalue);

  /**
   * @brief 读取一个与指定关键字关联的单精度浮点型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 单精度浮点型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是单精度浮点型数组,
   * 则输出错误信息, 并终止程序.
   *
   * 单精度浮点型数据可以从整型数据提升而来, 或从双精度浮点型数据截断而得.
   */
  Array<float> getFloatArray(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的单精度浮点型数组, 并存储在指定数组中.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输出参数, 指针, 指向单精度浮点型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是单精度浮点型数组,
   * 或者指定的元素个数和数据库中的数组元素个数不一致,
   * 则输出错误信息, 并终止程序.
   *
   * 单精度浮点型数据可以从整型数据提升而来, 或从双精度浮点型数据截断而得.
   */
  void getFloatArray(const string& key, float* data, const int nelements);
  //@}

  /// @name 获取或存入整型数据.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是整型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是整型,
   *                 假值表示不是整型或指定关键字不存在.
   */
  bool isInteger(const string& key);

  /**
   * @brief 将一个整数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 整型, 表示待存入的整数.
   */
  void putInteger(const string& key, const int& data);

  /**
   * @brief 将一个整型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 整型数组.
   */
  void putIntegerArray(const string& key, const Array<int>& data);

  /**
   * @brief 将指针指向的一个整型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向整型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putIntegerArray(const string& key, const int* const data,
                       const int nelements);

  /**
   * @brief 读取一个与指定关键字关联的整数. 若数据库中不存在该关键字, 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 单精度浮点型, 表示已读取的整数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是整数,
   * 则输出错误信息, 并终止程序.
   */
  int getInteger(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的整数. 若数据库中不存在该关键字,
   * 则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 整型, 表示缺省值.
   *
   * @return 整型, 表示已读取的整数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是整数,
   * 则输出错误信息, 并终止程序.
   */
  int getIntegerWithDefault(const string& key, const int& defaultvalue);

  /**
   * @brief 读取一个与指定关键字关联的整型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 整型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是整型数组,
   * 则输出错误信息, 并终止程序.
   */
  Array<int> getIntegerArray(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的整型数组, 并存储在指定数组中.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输出参数, 指针, 指向整型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是整型数组,
   * 或者指定的元素个数和数据库中的数组元素个数不一致,
   * 则输出错误信息, 并终止程序.
   */
  void getIntegerArray(const string& key, int* data, const int nelements);
  //@}

  /// @name 获取或存入字符串型数据.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是不是字符串型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是字符串型,
   *                 假值表示不是字符串型或指定关键字不存在.
   */
  bool isString(const string& key);

  /**
   * @brief 将一个字符串存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 字符串型, 表示待存入的字符串.
   */
  void putString(const string& key, const string& data);

  /**
   * @brief 将一个字符串数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 字符串数组.
   */
  void putStringArray(const string& key, const Array<string>& data);

  /**
   * @brief 将指针指向的一个字符串数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向字符串数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putStringArray(const string& key, const string* const data,
                      const int nelements);

  /**
   * @brief 读取一个与指定关键字关联的字符串. 若数据库中不存在该关键字, 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 字符串型, 表示已读取的字符串,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是字符串,
   * 则输出错误信息, 并终止程序.
   */
  string getString(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的字符串. 若数据库中不存在该关键字,
   * 则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 字符串型, 表示缺省值.
   *
   * @return 字符串型, 表示已读取的字符串,
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是字符串,
   * 则输出错误信息, 并终止程序.
   */
  string getStringWithDefault(const string& key, const string& defaultvalue);

  /**
   * @brief 读取一个与指定关键字关联的字符串数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 字符串数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是字符串数组,
   * 则输出错误信息, 并终止程序.
   */
  Array<string> getStringArray(const string& key);

  /**
   * @brief 读取一个与指定关键字关联的字符串数组, 并存储在指定数组中.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输出参数, 指针, 指向字符串数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是字符串数组,
   * 或者指定的元素个数和数据库中的数组元素个数不一致,
   * 则输出错误信息, 并终止程序.
   */
  void getStringArray(const string& key, string* data, const int nelements);

  //@}

  /// @name 输出数据库中的数据.
  //@{

  /**
   * @brief 打印数据库中的数据.
   *
   * @param os 输入参数, 输出流类, 缺省为并行标准输出流.
   *
   * @note
   * 打印当前数据库中所有的关键字/值到指定的输出流,
   * 并且在每行纪录后, 打印input(表示值来源于输入文件且已被使用),
   * unused(表示值来源于输入文件但未被使用)或者default(表示值来源于缺省值)标志.
   * 如果没有指定输出流, 则将数据输出到pout流.
   */
  void printClassData(ostream& os = (*POUT.stream()));

  /**
   * @brief 打印数据库中所有未使用的关键字/值配对到指定的输出流.
   *
   * @param os 输入参数, 输出流类, 缺省为并行标准输出流.
   */
  void printUnusedKeys(ostream& os = (*POUT.stream())) const;

  /**
   * @brief 打印数据库中所有由缺省值设置的关键字/值配对到指定的输出流.
   *
   * @param os 输入参数, 输出流对象, 缺省为并行标准输出流.
   *
   */
  void printDefaultKeys(ostream& os = (*POUT.stream())) const;
  //@}
  ///@endcond

 private:
  /*
   * 以下结构类型用于存储数据库中的一个纪录, 即关键字/值对.
   * 对于任何关键字, 除了由d_type指定类型的数组外, 结构中其他数组都是无效的.
   */
  struct KeyData {
    string d_key;                 /*!< 关键字  */
    int d_type;                   /*!< 记录类型  */
    int d_array_size;             /*!< 数组长度  */
    bool d_accessed;              /*!< 表示是否被访问过  */
    bool d_from_default;          /*!< 表示是否是由缺省值设置的  */
    Pointer<Database> d_database; /*!< 子数据库  */
    Array<bool> d_boolean;        /*!< 逻辑型数组  */
    Array<DatabaseBox> d_box;     /*!< box数组  */
    Array<char> d_char;           /*!< 字符型数组  */
    Array<dcomplex> d_complex;    /*!< 复型数组  */
    Array<double> d_double;       /*!<  双精度浮点型数组 */
    Array<float> d_float;         /*!< 单精度浮点型数组  */
    Array<int> d_integer;         /*!< 整型数组  */
    Array<string> d_string;       /*!<  字符串数组 */
  };

  /*
   * 私有辅助函数, 用于管理数据库.
   */
  bool deleteKeyIfFound(const string& key);
  KeyData* findKeyData(const string& key);
  KeyData* findKeyDataOrExit(const string& key);
  static void indentStream(ostream& os, const int indent);
  void printDatabase(ostream& os, const int indent, const int toprint) const;

  string d_database_name;         /*!<  数据库的名字 */
  std::list<KeyData> d_keyvalues; /*!<  关键字/值列表 */
};

}  // namespace tbox
}  // namespace HSF

#endif
