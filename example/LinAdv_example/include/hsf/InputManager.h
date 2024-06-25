//
// 文件名:    InputManager.h
// 软件包:    JAUMIN toolbox
// 版权  :	(c) 2004-2010 北京应用物理与计算数学研究所
// 版本号:    $Revision: 257 $
// 修改  :    $Date: 2012-02-16 12:55:07 +0800 (四, 2012-02-16) $
// 描述  :  输入管理器类, 管理用户的输入数据.
//

#ifndef included_tbox_InputManager
#define included_tbox_InputManager

#ifndef included_JAUMIN_config
#include "JAUMIN_config.h"
#endif
#ifndef included_tbox_InputDatabase
#include "InputDatabase.h"
#endif
#ifndef included_tbox_Pointer
#include "Pointer.h"
#endif
#ifndef included_String
#include <string>
using namespace std;
#define included_String
#endif

namespace HSF {
namespace tbox {

/**
 * @brief 输入管理器类 tbox::InputManager, 管理用户的输入数据.
 *
 * 对需要输入数据的对象, 用户需为其配置输入文件, 格式如下:
  \verbatim
  Object_entry {
     keyword1 = <value1>
     keyword2 = <value2>
     nested_input_database {
        nested_keyword1 = <value3>
     }
  }  \endverbatim
 * 实际应用中, 输入文件通常只有一个, 包含所有对象的输入数据. \n
 *
 * 用户在读取输入数据时, 需要调用以下函数:
 * - getManager(): 获取输入管理器
 * - parseInputFile(): 解析输入文件.
 *
 * 输入管理器解析输入文件, 将输入数据存储于输入数据库.
 * 用户可通过输入管理器获取输入数据库, 并通过数据库接口函数访问这些数据.
 *
 *  请参考 @ref example_toolbox_input_manager "计算参数读取示例".
 *
 * @note
 * - 输入文件的格式兼容:
 *   - C/C++的注释风格.
 *   - 可通过"#include ..."包含其他输入文件.
 *   - 简单表达式.
 * - 该类是孤类, 仅有单一实例.
 * @see tbox::Database, tbox::InputDatabase
 */

class InputManager {
 public:
  /**
   * @brief 获取输入数据库.
   *
   * @return 指针, 指向输入数据库.
   */
  static Pointer<Database> getInputDatabase();

  /**
   * @brief 获取输入管理器.
   *
   * @return 指针, 指向输入管理器.
   *
   * @note 本函数是访问输入管理器对象的唯一接口.
   */
  static InputManager *getManager();

  /**
   * @brief 设置输入管理器.
   *
   * @param manager 输入参数, 指针, 指向输入管理器.
   *
   * @note 该函数仅由派生类调用.
   */
  static void setManager(InputManager *manager);

  /**
   * @brief 判断输入数据库是否已经存在.
   *
   * @return 逻辑型, 真值表示输入数据库已经存在.
   */
  static bool inputDatabaseExists();

  /**
   * @brief 获取输入文件名.
   *
   * @return 字符串, 表示输入文件名.
   */
  static string &getInputFileName();

  /**
   * @brief 解析指定的输入文件, 并返回一个名为“main”的新数据库.
   *
   * @param filename 输入参数, 字符串, 表示输入文件名.
   *
   * @return 指针, 指向输入数据库的指针.
   */
  virtual Pointer<InputDatabase> parseInputFile(const string &filename);

  /**
   * @brief 解析指定的输入文件, 并把其结果存储到指定的数据库中.
   *
   * @param filename 输入参数, 字符串, 表示输入文件名.
   * @param db 输入参数, 指针, 指向指定的数据库.
   *
   * @return 指针, 指向输入数据库的指针.
   */
  virtual void parseInputFile(const string &filename,
                              Pointer<InputDatabase> db);
  ///@cond DEV
  /**
   * @brief 释放输入管理器.
   *
   * @note 该函数由静态资源释放类调用. 在应用程序中无需调用.
   */
  static void freeManager();
  ///@endcond
 protected:
  ///@cond DEV
  /**
   * @brief 构造函数.
   *
   * @note 由于本函数属性为protected,
   * 因此只有派生类才能通过访问本函数来创建输入管理器孤本.
   */
  InputManager();

  /**
   * @brief 虚析构函数.
   *
   * @note 由于本函数属性为protected,
   * 因此只有孤类及其派生类才能删除输入管理器对象.
   */
  virtual ~InputManager();
  ///@endcond
 private:
  static InputManager *s_manager_instance;
  static bool s_registered_callback;

  static Pointer<Database> s_input_db;
  static string s_filename;
};

}  // namespace tbox
}  // namespace HSF

#endif
