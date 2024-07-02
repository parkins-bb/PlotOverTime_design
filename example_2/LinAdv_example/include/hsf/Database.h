//
// 文件名:    Database.h
// 软件包:    JAUMIN toolbox
// 版权  :	(c) 2004-2010 北京应用物理与计算数学研究所
// 版本号:    $Revision: 257 $
// 修改  :    $Date: 2012-02-16 12:55:07 +0800 (四, 2012-02-16) $
// 描述  :  数据库抽象基类, 定义通用数据库接口.
//

#ifndef included_tbox_Database
#define included_tbox_Database

#ifndef included_JAUMIN_config
#include "JAUMIN_config.h"
#endif

#ifndef included_tbox_Array
#include "Array.h"
#endif
#ifndef included_tbox_Pointer
#include "Pointer.h"
#endif
#ifndef included_tbox_Complex
#include "Complex.h"
#endif

#include <iostream>

#include "utilities.h"

#ifndef included_String
#define included_String
#include <string>
using namespace std;
#endif

namespace HSF {
namespace tbox {

class DatabaseBox;

/**
 * @brief 数据库抽象基类 tbox::Database, 用于定义通用数据库接口.
 *
 * 本类是JAUMIN中输入数据库和重启动数据库的抽象基类.
 *
 * JAUMIN中, 数据库是一种嵌套数据库. 它的每条纪录是一个<关键字,值>对,
 * 其中值可能指向另一个子数据库对象, 也可能是逻辑型、整型、单精度浮点型、
 * 双精度浮点型、双精度复型、字符型、字符串或数据库box类型的数据. @if DEV
 * 这里, box以 tbox::DatabaseBox 中定义的结构存储. @endif
 *
 * 对于int, float, double, complex, bool, char型,
 *用户存取数据可调用如下接口函数:
 * - 将数据存入数据库, 并与关键字key关联:
 *  - putScalar(key, TYPE), 存入TYPE型数.
 *  - putArray(key, TYPE array), 存入TYPE型数组.
 *  - putArray(key, TYPE *, int n), 存入包含n个元素的TYPE型数组.
 * - 从数据库中读取与关键字key关联的数据:
 *  - getScalar(key, TYPE),  读取TYPE型数.
 *  - getArray(key, TYPE array), 读取TYPE型数组.
 *  - getArray(key, TYPE *, int n), 读取包含n个元素的TYPE型数组
 *
 *  对于int, float, double, complex, bool, char, string,
 *子数据库和数据库box类型,
 *  用户存取数据可调用如下接口函数:
 * - 将数据存入数据库, 并与关键字key关联:
 *  - putTYPE(key, TYPE), 存入TYPE型数.
 *  - putTYPEArray(key, TYPE array), 存入TYPE型数组.
 * - 从数据库中读取与关键字key关联的数据:
 *   - getTYPE(key), 读取TYPE型数.
 *   - getTYPEArray(key), 读取TYPE型数组.
 *   - getTYPEArray(key, TYPE *, int n), 读取包含n个元素的TYPE型数组.
 *   - getTYPEWithDefault(key, defaultvalue), 读取TYPE型数,
 *     如果关键字不存在, 则返回defaultvalue.
 *
 */

class Database {
 public:
  /**
   * @brief 构造函数.
   */
  Database() {}

  /**
   * @brief 析构函数.
   */
  virtual ~Database() {}

  /// @name 查询或获取关键字.
  //@{

  /**
   * @brief 在当前数据库中查找指定关键字.
   *
   * @param key 输入参数, 字符串, 表示关键字.
   *
   * @return 逻辑型, 真值表示在当前数据库中该关键字存在.
   */
  virtual bool keyExists(const string& key) = 0;

  /**
   * @brief 返回一个数组, 包含当前数据库中的所有关键字.
   *
   * @return 数组, 包含当前数据库中的所有关键字.
   */
  virtual Array<string> getAllKeys() = 0;
  //@}

  /// @name 子数据库存取
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为数据库类型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的是数据库,
   *                 假值表示不是数据库或指定关键字不存在.
   */
  virtual bool isDatabase(const string& key) = 0;

  /**
   * @brief 创建一个与指定关键字关联的子数据库
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 指针, 指向新创建的数据库,
   */
  virtual Pointer<Database> putDatabase(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的子数据库
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 指针, 指向已读取的数据库,
   */
  virtual Pointer<Database> getDatabase(const string& key) = 0;

  //@}

  /// @name 逻辑型数据存取.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为逻辑型数据.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是逻辑型数据,
   *                 假值表示不是逻辑型或指定关键字不存在.
   */
  virtual bool isBool(const string& key) = 0;

  /**
   * @brief 将一个逻辑型数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 逻辑型, 表示待存入的逻辑数.
   */
  virtual void putBool(const string& key, const bool& data) = 0;

  /**
   * @brief 将一个逻辑型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 逻辑型数组.
   */
  virtual void putBoolArray(const string& key, const Array<bool>& data) = 0;

  /**
   * @brief 将指针指向的一个逻辑型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向逻辑型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  virtual void putBoolArray(const string& key, const bool* const data,
                            const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的逻辑型数. 若数据库中不存在该关键字,
   * 则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 表示已读取的逻辑型数.
   *
   * @note 如果在当前数据库中该关键字不存在, 或者与其关联的不是逻辑型数,
   * 则输出错误信息, 并终止程序.
   *
   */
  virtual bool getBool(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的逻辑型数. 若数据库中不存在该关键字,
   *则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 逻辑型, 表示缺省值.
   *
   * @return 逻辑型, 表示已读取的逻辑型数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是逻辑数,
   * 则输出错误信息, 并终止程序.
   */
  virtual bool getBoolWithDefault(const string& key,
                                  const bool& defaultvalue) = 0;

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
  virtual Array<bool> getBoolArray(const string& key) = 0;

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
  virtual void getBoolArray(const string& key, bool* data,
                            const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的逻辑数.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输出参数, 字符型, 表示从数据库中读取的逻辑数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是逻辑数,
   * 则输出错误信息, 并终止程序.
   */
  void getScalar(const string& key, bool& scalar);

  /**
   * @brief 将一个逻辑数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输入参数, 逻辑型, 表示待存入的逻辑数.
   */
  void putScalar(const string& key, const bool scalar);

  /**
   * @brief 读取一个与指定关键字关联的逻辑型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输出参数, 逻辑型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是逻辑型数组,
   * 则输出错误信息, 并终止程序.
   */
  void getArray(const string& key, Array<bool>& array);

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

  void getArray(const string& key, bool* data, const int nelements);

  /**
   * @brief 将一个逻辑型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输入参数, 逻辑型数组.
   */
  void putArray(const string& key, const Array<bool> array);
  /**
   * @brief 将指针指向的一个逻辑型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向逻辑型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putArray(const string& key, const bool* const data, const int nelements);

  //@}

  /// @name 双精度复型数据存取.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为双精度复数.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是双精度复数,
   *                 假值表示不是双精度复数或指定关键字不存在.
   */
  virtual bool isComplex(const string& key) = 0;

  /**
   * @brief 将一个双精度复数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 双精度复型, 表示待存入的双精度复数.
   */
  virtual void putComplex(const string& key, const dcomplex& data) = 0;

  /**
   * @brief 将一个双精度复型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 双精度复型数组.
   */
  virtual void putComplexArray(const string& key,
                               const Array<dcomplex>& data) = 0;

  /**
   * @brief 将指针指向的一个双精度复型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向双精度复型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  virtual void putComplexArray(const string& key, const dcomplex* const data,
                               const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的双精度复数. 若数据库中不存在该关键字,
   *则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 双精度复型, 表示已读取的双精度复数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度复数,
   * 则输出错误信息, 并终止程序.
   */
  virtual dcomplex getComplex(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的双精度复数. 若数据库中不存在该关键字,
   *则返回缺省值.
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
   */
  virtual dcomplex getComplexWithDefault(const string& key,
                                         const dcomplex& defaultvalue) = 0;

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
   */
  virtual Array<dcomplex> getComplexArray(const string& key) = 0;

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
   */
  virtual void getComplexArray(const string& key, dcomplex* data,
                               const int nelements) = 0;
  /**
   * @brief 读取一个与指定关键字关联的双精度复型标量.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输出参数, 双精度复型, 表示从数据库中读取的双精度复型标量.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度复型标量,
   * 则输出错误信息, 并终止程序.
   */
  void getScalar(const string& key, dcomplex& scalar);

  /**
   * @brief 将一个双精度复型标量存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输入参数, 双精度复型, 表示待存入的双精度复型标量.
   */
  void putScalar(const string& key, const dcomplex scalar);

  /**
   * @brief 读取一个与指定关键字关联的双精度复型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输出参数, 双精度复型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度复型数组,
   * 则输出错误信息, 并终止程序.
   */
  void getArray(const string& key, Array<dcomplex>& array);

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
   */

  void getArray(const string& key, dcomplex* data, const int nelements);

  /**
   * @brief 将一个双精度复型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输入参数, 双精度复型数组.
   */
  void putArray(const string& key, const Array<dcomplex> array);

  /**
   * @brief 将指针指向的一个双精度复型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向双精度复型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putArray(const string& key, const dcomplex* const data,
                const int nelements);

  //@}

  /// @name 双精度浮点型数据存取.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为双精度浮点型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是双精度浮点型,
   *                 假值表示不是双精度浮点型或指定关键字不存在.
   */
  virtual bool isDouble(const string& key) = 0;

  /**
   * @brief 将一个双精度浮点数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 双精度浮点型, 表示待存入的双精度浮点数.
   */
  virtual void putDouble(const string& key, const double& data) = 0;

  /**
   * @brief 将一个双精度浮点型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 双精度浮点型数组.
   */
  virtual void putDoubleArray(const string& key, const Array<double>& data) = 0;

  /**
   * @brief 将指针指向的一个双精度浮点型数组存入数据库,
   * 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向双精度浮点型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  virtual void putDoubleArray(const string& key, const double* const data,
                              const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的双精度浮点数. 若数据库中不存在该关键字,
   *则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 双精度浮点型, 表示已读取的双精度浮点数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度浮点数,
   * 则输出错误信息, 并终止程序.
   */
  virtual double getDouble(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的双精度浮点数. 若数据库中不存在该关键字,
   *则返回缺省值.
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
   */
  virtual double getDoubleWithDefault(const string& key,
                                      const double& defaultvalue) = 0;

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
   */
  virtual Array<double> getDoubleArray(const string& key) = 0;

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
   */
  virtual void getDoubleArray(const string& key, double* data,
                              const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的双精度浮点数.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输出参数, 双精度浮点型, 表示从数据库中读取的双精度浮点数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度浮点数,
   * 则输出错误信息, 并终止程序.
   */
  void getScalar(const string& key, double& scalar);

  /**
   * @brief 将一个双精度浮点数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输入参数, 双精度浮点型, 表示待存入的双精度浮点数.
   */
  void putScalar(const string& key, const double scalar);

  /**
   * @brief 读取一个与指定关键字关联的双精度浮点型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输出参数, 双精度浮点型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是双精度浮点型数组,
   * 则输出错误信息, 并终止程序.
   */
  void getArray(const string& key, Array<double>& array);

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
   */

  void getArray(const string& key, double* data, const int nelements);

  /**
   * @brief 将一个双精度浮点型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输入参数, 双精度浮点型数组.
   */

  void putArray(const string& key, const Array<double> array);

  /**
   * @brief 将指针指向的一个双精度浮点型数组存入数据库,
   * 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向双精度浮点型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putArray(const string& key, const double* const data,
                const int nelements);

  //@}

  /// @name 单精度浮点型数据存取.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为单精度浮点型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是单精度浮点型,
   *                 假值表示不是单精度浮点型或指定关键字不存在.
   */
  virtual bool isFloat(const string& key) = 0;

  /**
   * @brief 将一个单精度浮点数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 单精度浮点型, 表示待存入的单精度浮点数.
   */
  virtual void putFloat(const string& key, const float& data) = 0;

  /**
   * @brief 将一个单精度浮点型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 单精度浮点型数组.
   */
  virtual void putFloatArray(const string& key, const Array<float>& data) = 0;

  /**
   * @brief 将指针指向的一个单精度浮点型数组存入数据库,
   * 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向单精度浮点型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  virtual void putFloatArray(const string& key, const float* const data,
                             const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的单精度浮点数. 若数据库中不存在该关键字,
   *则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 单精度浮点型, 表示已读取的单精度浮点数,
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是单精度浮点数,
   * 则输出错误信息, 并终止程序.
   */
  virtual float getFloat(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的单精度浮点数. 若数据库中不存在该关键字,
   *则返回缺省值.
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
   */
  virtual float getFloatWithDefault(const string& key,
                                    const float& defaultvalue) = 0;

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
   */
  virtual Array<float> getFloatArray(const string& key) = 0;

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
   */
  virtual void getFloatArray(const string& key, float* data,
                             const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的单精度浮点数.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输出参数, 单精度浮点型, 表示从数据库中读取的单精度浮点数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是单精度浮点数,
   * 则输出错误信息, 并终止程序.
   */
  void getScalar(const string& key, float& scalar);

  /**
   * @brief 将一个单精度浮点数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输入参数, 单精度浮点型, 表示待存入的单精度浮点数.
   */
  void putScalar(const string& key, const float scalar);

  /**
   * @brief 读取一个与指定关键字关联的单精度浮点型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输出参数, 单精度浮点型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是单精度浮点型数组,
   * 则输出错误信息, 并终止程序.
   */
  void getArray(const string& key, Array<float>& array);

  /**
   * @brief 将一个单精度浮点型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输入参数, 单精度浮点型数组.
   */
  void putArray(const string& key, const Array<float> array);

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
   */

  void getArray(const string& key, float* data, const int nelements);

  /**
   * @brief 将指针指向的一个单精度浮点型数组存入数据库,
   * 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向单精度浮点型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putArray(const string& key, const float* const data,
                const int nelements);

  //@}

  /// @name 整型数据存取.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为整型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是整型,
   *                 假值表示不是整型或指定关键字不存在.
   */
  virtual bool isInteger(const string& key) = 0;

  /**
   * @brief 将一个整数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 整型, 表示待存入的整数.
   */
  virtual void putInteger(const string& key, const int& data) = 0;

  /**
   * @brief 将一个整型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 整型数组.
   */
  virtual void putIntegerArray(const string& key, const Array<int>& data) = 0;

  /**
   * @brief 将指针指向的一个整型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向整型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  virtual void putIntegerArray(const string& key, const int* const data,
                               const int nelements) = 0;

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
  virtual int getInteger(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的整数. 若数据库中不存在该关键字,
   *则返回缺省值.
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
  virtual int getIntegerWithDefault(const string& key,
                                    const int& defaultvalue) = 0;

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
  virtual Array<int> getIntegerArray(const string& key) = 0;

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
  virtual void getIntegerArray(const string& key, int* data,
                               const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的整数.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输出参数, 整型, 表示从数据库中读取的整数.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是整数,
   * 则输出错误信息, 并终止程序.
   */
  void getScalar(const string& key, int& scalar);

  /**
   * @brief 将一个整数存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输入参数, 整型型, 表示待存入的整数.
   */
  void putScalar(const string& key, const int scalar);

  /**
   * @brief 读取一个与指定关键字关联的整型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输出参数, 整型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是整型数组,
   * 则输出错误信息, 并终止程序.
   */
  void getArray(const string& key, Array<int>& array);
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

  void getArray(const string& key, int* data, const int nelements);

  /**
   * @brief 将一个整型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输入参数, 整型数组.
   */
  void putArray(const string& key, const Array<int> array);

  /**
   * @brief 将指针指向的一个整型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向整型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putArray(const string& key, const int* const data, const int nelements);

  //@}

  /// @name 字符型数据存取.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为字符型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值的类型是字符型,
   *                 假值表示不是字符型或指定关键字不存在.
   */
  virtual bool isChar(const string& key) = 0;

  /**
   * @brief 将一个字符存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 字符型, 表示待存入的字符.
   */
  virtual void putChar(const string& key, const char& data) = 0;

  /**
   * @brief 将字符型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 字符型数组.
   */
  virtual void putCharArray(const string& key, const Array<char>& data) = 0;

  /**
   * @brief 将指针指向的字符型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向字符型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  virtual void putCharArray(const string& key, const char* const data,
                            const int nelements) = 0;

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
  virtual char getChar(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的字符. 若数据库中不存在该关键字,
   *则返回缺省值.
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
  virtual char getCharWithDefault(const string& key,
                                  const char& defaultvalue) = 0;

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
  virtual Array<char> getCharArray(const string& key) = 0;

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
  virtual void getCharArray(const string& key, char* data,
                            const int nelements) = 0;
  /**
   * @brief 读取一个与指定关键字关联的字符.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输出参数, 字符型, 表示从数据库中读取的字符.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是字符,
   * 则输出错误信息, 并终止程序.
   */
  void getScalar(const string& key, char& scalar);

  /**
   * @brief 将一个字符存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param scalar 输入参数, 字符型, 表示待存入的字符.
   */
  void putScalar(const string& key, const char scalar);

  /**
   * @brief 读取一个与指定关键字关联的字符型数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输出参数, 字符型数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是字符型数组,
   * 则输出错误信息, 并终止程序.
   */
  void getArray(const string& key, Array<char>& array);

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

  void getArray(const string& key, char* data, const int nelements);

  /**
   * @brief 将一个字符型数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param array 输入参数, 字符型数组.
   */
  void putArray(const string& key, const Array<char> array);
  /**
   * @brief 将指针指向的一个双精度字符型数组存入数据库,
   * 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向字符型数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  void putArray(const string& key, const char* const data, const int nelements);

  //@}

  /// @name 字符串型数据存取.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为字符串型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值的类型是字符串型,
   *                 假值表示不是字符串型或指定关键字不存在.
   */
  virtual bool isString(const string& key) = 0;

  /**
   * @brief 将一个字符串存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 字符串型, 表示待存入的字符串.
   */
  virtual void putString(const string& key, const string& data) = 0;

  /**
   * @brief 将一个字符串数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 字符串数组.
   */
  virtual void putStringArray(const string& key, const Array<string>& data) = 0;

  /**
   * @brief 将指针指向的一个字符串数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向字符串数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  virtual void putStringArray(const string& key, const string* const data,
                              const int nelements) = 0;

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
  virtual string getString(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的字符串. 若数据库中不存在该关键字,
   *则返回缺省值.
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
  virtual string getStringWithDefault(const string& key,
                                      const string& defaultvalue) = 0;

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
  virtual Array<string> getStringArray(const string& key) = 0;

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
  virtual void getStringArray(const string& key, string* data,
                              const int nelements) = 0;

  //@}

  /// @name 数据库box对象存取.
  //@{

  /**
   * @brief 判断与指定关键字关联的值是否为数据库box类型.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 逻辑型, 真值表示与指定关键字关联的值是数据库box对象,
   *                   假值表示不是数据库对象或指定关键字不存在.
   */
  virtual bool isDatabaseBox(const string& key) = 0;

  /**
   * @brief 将单个数据库box对象存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 数据库box类, 数据库box对象.
   */
  virtual void putDatabaseBox(const string& key, const DatabaseBox& data) = 0;

  /**
   * @brief 将数据库box对象数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 数据库box对象数组.
   */
  virtual void putDatabaseBoxArray(const string& key,
                                   const Array<DatabaseBox>& data) = 0;

  /**
   * @brief 将指针指向的数据库box对象数组存入数据库, 并使之与指定关键字相关联.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param data 输入参数, 指针, 指向数据库box对象数组.
   * @param nelements 输入参数, 整型, 数组元素的个数.
   */
  virtual void putDatabaseBoxArray(const string& key,
                                   const DatabaseBox* const data,
                                   const int nelements) = 0;

  /**
   * @brief 读取一个与指定关键字关联的数据库box对象. 若数据库中不存在该关键字,
   *则出错.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 数据库box对象.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是DatabaseBox对象,
   * 则输出错误信息, 并终止程序.
   */
  virtual DatabaseBox getDatabaseBox(const string& key) = 0;

  /**
   * @brief 读取一个与指定关键字关联的数据库box对象. 若数据库中不存在该关键字,
   *则返回缺省值.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   * @param defaultvalue 输入参数, 数据库box类, 表示缺省值.
   *
   * @return  数据库box类, 数据库box对象.
   *
   * @note
   * 如果在当前数据库中该关键字不存在,
   * 则将该关键字与缺省值相关联, 并返回缺省值.
   * 如果关键字已经存在, 但与其关联的不是数据库box对象,
   * 则输出错误信息, 并终止程序.
   */
  virtual DatabaseBox getDatabaseBoxWithDefault(
      const string& key, const DatabaseBox& defaultvalue) = 0;

  /**
   * @brief 读取一个与指定关键字关联的数据库box对象数组.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 数据库box数组.
   *
   * @note
   * 如果在当前数据库中该关键字不存在, 或者与其关联的不是数据库box对象,
   * 则输出错误信息, 并终止程序.
   */
  virtual Array<DatabaseBox> getDatabaseBoxArray(const string& key) = 0;

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
  virtual void getDatabaseBoxArray(const string& key, DatabaseBox* data,
                                   const int nelements) = 0;
  //@}

  /// @name 其它.
  //@{

  /**
   * @brief 打印数据库数据.
   *
   * @param os 输入参数, 输出流类, 缺省为并行标准输出流.
   */
  virtual void printClassData(ostream& os = (*POUT.stream())) = 0;

  /**
   * @brief 返回与指定关键字关联的数组的长度.
   *
   * @param key 输入参数, 字符串, 表示指定的关键字.
   *
   * @return 整型, 表示数组的长度.
   *
   * @note 如果在当前数据库中该关键字不存在, 则返回零值.
   */
  virtual int getArraySize(const string& key) = 0;
  //@}
};
}  // namespace tbox
}  // namespace HSF

namespace HSF {
namespace tbox {

/*
 * Boolean
 */
inline void Database::getScalar(const string& key, bool& scalar) {
  scalar = getBool(key);
}

inline void Database::putScalar(const string& key, const bool scalar) {
  putBool(key, scalar);
}

inline void Database::getArray(const string& key, Array<bool>& array) {
  array = getBoolArray(key);
}

inline void Database::putArray(const string& key, const Array<bool> array) {
  putBoolArray(key, array);
}

/*
 * Char
 */

inline void Database::getScalar(const string& key, char& scalar) {
  scalar = getChar(key);
}

inline void Database::putScalar(const string& key, const char scalar) {
  putChar(key, scalar);
}

inline void Database::getArray(const string& key, Array<char>& array) {
  array = getCharArray(key);
}

inline void Database::putArray(const string& key, const Array<char> array) {
  putCharArray(key, array);
}

/*
 * Complex
 */
inline void Database::getScalar(const string& key, dcomplex& scalar) {
  scalar = getComplex(key);
}

inline void Database::putScalar(const string& key, const dcomplex scalar) {
  putComplex(key, scalar);
}

inline void Database::getArray(const string& key, Array<dcomplex>& array) {
  array = getComplexArray(key);
}

inline void Database::putArray(const string& key, const Array<dcomplex> array) {
  putComplexArray(key, array);
}

/*
 * Float
 */
inline void Database::getScalar(const string& key, float& scalar) {
  scalar = getFloat(key);
}

inline void Database::putScalar(const string& key, const float scalar) {
  putFloat(key, scalar);
}

inline void Database::getArray(const string& key, Array<float>& array) {
  array = getFloatArray(key);
}

inline void Database::putArray(const string& key, const Array<float> array) {
  putFloatArray(key, array);
}

/*
 * Double
 */
inline void Database::getScalar(const string& key, double& scalar) {
  scalar = getDouble(key);
}

inline void Database::putScalar(const string& key, const double scalar) {
  putDouble(key, scalar);
}

inline void Database::getArray(const string& key, Array<double>& array) {
  array = getDoubleArray(key);
}

inline void Database::putArray(const string& key, const Array<double> array) {
  putDoubleArray(key, array);
}

/*
 * Integer
 */

inline void Database::getScalar(const string& key, int& scalar) {
  scalar = getInteger(key);
}

inline void Database::putScalar(const string& key, const int scalar) {
  putInteger(key, scalar);
}

inline void Database::getArray(const string& key, Array<int>& array) {
  array = getIntegerArray(key);
}

inline void Database::putArray(const string& key, const Array<int> array) {
  putIntegerArray(key, array);
}

// 输出数组中的前nelements个元素.
//  bool型
inline void Database::getArray(const string& key, bool* data,
                               const int nelements) {
  getBoolArray(key, data, nelements);
}
inline void Database::putArray(const string& key, const bool* const data,
                               const int nelements) {
  putBoolArray(key, data, nelements);
}
// char型
inline void Database::getArray(const string& key, char* data,
                               const int nelements) {
  getCharArray(key, data, nelements);
}
inline void Database::putArray(const string& key, const char* const data,
                               const int nelements) {
  putCharArray(key, data, nelements);
}
// dcomplex型
inline void Database::getArray(const string& key, dcomplex* data,
                               const int nelements) {
  getComplexArray(key, data, nelements);
}
inline void Database::putArray(const string& key, const dcomplex* const data,
                               const int nelements) {
  putComplexArray(key, data, nelements);
}
// double型
inline void Database::getArray(const string& key, double* data,
                               const int nelements) {
  getDoubleArray(key, data, nelements);
}

inline void Database::putArray(const string& key, const double* const data,
                               const int nelements) {
  putDoubleArray(key, data, nelements);
}
// float型
inline void Database::getArray(const string& key, float* data,
                               const int nelements) {
  getFloatArray(key, data, nelements);
}

inline void Database::putArray(const string& key, const float* const data,
                               const int nelements) {
  putFloatArray(key, data, nelements);
}

// int型
inline void Database::getArray(const string& key, int* data,
                               const int nelements) {
  getIntegerArray(key, data, nelements);
}

inline void Database::putArray(const string& key, const int* const data,
                               const int nelements) {
  putIntegerArray(key, data, nelements);
}

}  // namespace tbox
}  // namespace HSF

#endif
