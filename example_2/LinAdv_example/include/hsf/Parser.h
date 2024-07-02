// 文件名:    Parser.h
// 软件包:    JAUMIN toolbox
// 版权  :	(c) 2004-2010 北京应用物理与计算数学研究所
// 版本号:    $Revision: 257 $
// 修改  :    $Date: 2012-02-16 12:55:07 +0800 (四, 2012-02-16) $
// 描述  :  输入文件语法解析器类
// 类别  :  %Internal File% ( Don't delete this line )
//

#ifndef included_tbox_Parser
#define included_tbox_Parser

#ifndef included_JAUMIN_config
#include "JAUMIN_config.h"
#endif

#ifndef included_stdio
#define included_stdio
#include <stdio.h>
#endif

#ifndef included_tbox_Database
#include "Database.h"
#endif
#include <list>
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
 * @brief 输入文件语法解析器类 tbox::Parser,
 * 解析用户输入文件, 并将解析结果存入输入数据库.
 *
 * 该类控制整个文件解析过程, 其中包括出错处理, 和对文件、行号以及光标位置的追踪.
 *
 * 如果并行执行应用程序,
 * 那么仅由0号进程负责从指定的输入文件读取数据, 并广播到其他进程.
 * 此时, 其他进程的输入文件参数可以被忽略或者为NULL.
 *
 * 解析器类还定义了一个通过静态成员函数访问的缺省解析器,
 * 这个解析器可以在文件解析期间被yacc/lex函数访问.
 * 通过这种拟孤类(singelton-like)方法,
 * 无需全局变量就可以实现解析信息在解析器和yacc/lex文件之间的传递.
 *
 * 该类(以及与其相关的yacc和lex文件)假设GNU flex和bison工具包可用.
 * 事实上, JAUMIN的发行版中已经包含了来源于flex和bison的输出文件,
 * 所以仅当语法器或扫描器改变时, 才需要这些工具包.
 */

class Parser {
 public:
  /**
   * @brief 构造函数, 创建一个未初始化的解析器.
   *
   * @note
   *  - 在调用其他成员函数前,
   *    必须先调用成员函数 parse() 初始化解析器对象并解析输入数据.
   *  - 可多次调用成员函数 parse() 以解析多个输入文件,
   *    但在新的解析过程开始时, 所有的状态值都会被复位(例如错误或警告的个数).
   */
  Parser();

  /**
   * @brief 析构函数.
   */
  ~Parser();

  /**
   * @brief 解析指定的输入文件, 并将解析结果存入输入数据库.
   *
   * @param filename 输入参数, 字符串, 输入文件的文件名.
   * @param fstream 输入参数, 文件指针.
   * @param database 输入参数, 指针, 指向数据库.
   *
   * @return 整型, 表示错误语法的个数, 值为0表示解析成功.
   *
   * @note
   *   - 如果按并行方式运行, 那么文件流仅在进程0有效,
   *     在其他进程忽略或设置为NULL.
   *   - 在解析多个文件时, 可以为每个文件执行一次parse()调用,
   *     但每次开始新的解析过程时, 所有的状态值都会被复位清零.
   */
  int parse(const string& filename, FILE* fstream, Pointer<Database> database);

  /**
   * @brief 获取解析过程中出现的错误个数.
   *
   * @return 整型, 表示错误个数.
   */
  int getNumberErrors() const;

  /**
   * @brief 获取解析过程中出现的警告个数.
   *
   * @return 整型, 表示警告个数.
   */
  int getNumberWarnings() const;

  /// @cond DEV

  /**
   * @brief 获取当前默认的解析器对象指针.
   *
   * @note 本函数用于在输入文件解析期间与yacc/lex函数进行通信.
   * 除非某解析器调用了成员函数 parse(), 否则缺省的解析器对象为NULL.
   */
  static Parser* getParser();

  /**
   * @brief 获取当前的数据库作用域.
   *
   * @note 仅有成员函数 enterScope() 和 leaveScope() 能修改当前的作用域.
   */
  Pointer<Database>& getScope();

  /**
   * @brief 创建一个指定名字的新数据库作用域.
   *
   * @note 这个新作用域将作为缺省作用域, 直到调用leaveScope()函数为止.
   */
  void enterScope(const string& name);

  /**
   * @brief 离开当前的数据库作用域, 返回到前一数据库.
   *
   * @note 离开最外层作用域会导致错误.
   */
  void leaveScope();

  /**
   * @brief 获取数据库指针, 指向第一个包含指定关键字的数据库.
   *
   * @note 如果所有数据库中均无该关键字, 那么返回一个NULL指针.
   */
  Pointer<Database> getDatabaseWithKey(const string& name);

  /**
   * @brief 保存当前文件的上下文, 然后切换现场到指定的输入文件.
   *
   * @return 逻辑型, 真值表示指定的文件存并且切换成功.
   */
  bool pushIncludeFile(const string& filename);

  /**
   * @brief 关闭当前输入文件, 并将现场切换到先前的输入文件.
   */
  void popIncludeFile();

  /**
   * @brief 报告一个解析错误, 并将指定的错误信息输出到 tbox::pout 流.
   *
   * @note
   *  - 只有解析器或扫描器才会调用本函数.
   *  - 本类假设所有进程都在解析相同的输入文件,
   *    因此错误信息会被输出到 tbox::pout 流.
   */
  void error(const string& message);

  /**
   * @brief 报告一个解析警告, 并将指定的警告信息输出到 tbox::pout 流.
   *
   * @note
   *  - 只有解析器或扫描器才会调用本函数.
   *  - 本类假设所有进程都在解析相同的输入文件,
   *    因此警告信息会被输出到 tbox::pout 流.
   */
  void warning(const string& message);

  /**
   * @brief 设置当前将被解析的输入数据行.
   */
  void setLine(const string& line);

  /**
   * @brief 将行号前移指定的行数. 若不指定行数则前进1行.
   */
  void advanceLine(const int nline = 1);

  /**
   * @brief 根据指定的字符串内容, 在当前行中移动光标位置.
   *
   * @note
   * - 字符串中的Tab符号表示前移8个字符直至tab终止处.
   * - 只要行号改变, 光标的位置就会被自动设置成1.
   */
  void advanceCursor(const string& token);

  /**
   * @brief 以字符流形式从当前输入文件读入一定长度的数据.
   *
   * @note
   * - 在MPI环境下, 由0号进程负责从输入文件中读取数据然后广播到所有进程.
   * - 本函数主要为flex提供文件读取功能.
   */
  int yyinput(char* buffer, const int max_size);
  /// @endcond

 private:
  struct ParseData {
    string d_filename;   /*!< 文件名 */
    FILE* d_fstream;     /*!< 输入文件流 */
    string d_linebuffer; /*!< 当前正在解析的数据行 */
    int d_linenumber;    /*!< 当前数据行在输入文件流中的行号 */
    int d_cursor;        /*!< 当前光标位置 */
    int d_nextcursor;    /*!< 下一光标位置 */
  };

  int d_errors;   /*!< 解析错误总数 */
  int d_warnings; /*!< 解析警告总数 */

#ifdef LACKS_NAMESPACE_IN_DECLARE
  std::list<ParseData> d_parse_stack;
#else
  std::list<Parser::ParseData> d_parse_stack;
#endif

  std::list<Pointer<Database> > d_scope_stack;

  static Parser* s_default_parser;

  static bool s_static_tables_initialized;

  string d_pathname; /*!< 头文件路径名 */
};

}  // namespace tbox
}  // namespace HSF

namespace HSF {
namespace tbox {

inline Parser* Parser::getParser() { return (s_default_parser); }

inline Pointer<Database>& Parser::getScope() {
  return (*(d_scope_stack.begin()));
}

inline void Parser::enterScope(const string& name) {
  d_scope_stack.push_front((*(d_scope_stack.begin()))->putDatabase(name));
}

inline void Parser::leaveScope() { d_scope_stack.pop_front(); }

inline int Parser::getNumberErrors() const { return (d_errors); }

inline int Parser::getNumberWarnings() const { return (d_warnings); }

#ifdef DEBUG_NO_INLINE
#undef inline
#endif

}  // namespace tbox
}  // namespace HSF

#endif
///@endcond
