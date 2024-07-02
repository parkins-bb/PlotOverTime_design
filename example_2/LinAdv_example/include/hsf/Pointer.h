//
// 文件名: Pointer.h
// 软件包: JAUMIN toolbox
// 版权  : 2004-1024, 北京应用物理与计算数学研究所
// 版本号: $Revision$
// 修改  : $Date$
// 描述  : 灵敏指针
//

#ifndef JAUMIN_TBOX_POINTER_H
#define JAUMIN_TBOX_POINTER_H

#include <memory>

namespace HSF {
namespace tbox {

/**
 * @brief 灵敏指针类 tbox::Pointer, 定义指针, 可防止内存泄漏.
 *
 * 灵敏指针，又称智能指针，是一种行为类似标准指针的类型. 和标准指针类似，它可
 * 解引用为所指向的对象. 但和标准指针不同，灵敏指针指向的对象不需要显式调用
 * delete 删除. 灵敏指针内部维护一个引用计数，并在析构时将该引用计数减1，当该
 * 计数为0时，调用 delete 或删除器删除指向的对象.
 *
 * 由于 **灵敏指针可防止内存泄漏**, 所以建议用户使用灵敏指针. 除了需用构造函数
 * 定义灵敏指针, 灵敏指针的用法与标准指针相同.
 *
 * 本灵敏指针类基于 std::shared_ptr 实现，但和 std::shared_ptr 有如下不同：
 *   1. 从不同类型的灵敏指针复制构造或赋值时，源指针将被动态转型为指定类型.
 *      这样基类和派生类的灵敏指针可互相转换.
 *   2. 从不同类型的标准指针构造或赋值时，源指针将被动态转型为指定类型. 这样基
 *      类和派生类的标准指针可正确转换为灵敏指针.
 *
 * @see tbox::Array
 */

template <typename T>
class Pointer : public std::shared_ptr<T> {
 public:
  /// @name 构造与析构
  /// @{

  /**
   * @brief 默认构造函数, 创建一个空的灵敏指针.
   */
  Pointer() : std::shared_ptr<T>() {}

  /**
   * @brief 构造函数：从标准指针构造灵敏指针.
   * @param p 输入参数，标准指针
   */
  Pointer(T* p) : std::shared_ptr<T>(p) {}

  /**
   * @brief 构造函数：从标准指针动态转型构造灵敏指针
   * @param p 输入参数, 标准指针.
   */
  template <typename Y>
  explicit Pointer(Y* p) : std::shared_ptr<T>(dynamic_cast<T*>(p)) {}

  /**
   * @brief 构造函数：构造带删除器的灵敏指针
   * @param p 输入参数，标准指针
   * @param d 输入参数，删除器
   *
   * @note
   *  - 删除器是一个 `void (*)(T* p)` 函数或函数对象.
   *  - 删除器在灵敏指针被析构时调用.
   *  - 通过自定义删除器，灵敏指针可管理来源于内存池的指针.
   */
  template <typename D>
  Pointer(T* p, D d) : std::shared_ptr<T>(p, d) {}

  /**
   * @brief 构造函数：动态转型构造带删除器的灵敏指针
   * @param p 输入参数，标准指针
   * @param d 输入参数，删除器
   *
   * @note
   *  - 删除器是一个 `void (*)(T* p)` 函数或函数对象.
   *  - 删除器在灵敏指针被析构时调用.
   *  - 通过自定义删除器，灵敏指针可管理来源于内存池的指针.
   */
  template <typename Y, typename D>
  Pointer(Y* p, D d) : std::shared_ptr<T>(dynamic_cast<T*>(p), d) {}

  /**
   * @brief 复制构造函数
   * @param p 源指针
   * @note 新指针与原指针指向相同内存地址，引用计数+1
   */
  Pointer(const Pointer<T>& p) : std::shared_ptr<T>(p) {}

  /**
   *@brief 动态转型复制构造函数
   *@param p 输入参数, 灵敏指针基类
   *@note 源指针将动态转型为目标类型.
   */
  template <typename Y>
  Pointer(const Pointer<Y>& p)
      : std::shared_ptr<T>(std::dynamic_pointer_cast<T>(p)) {}

  /**
   * @brief 从弱指针构造
   * @param p 弱指针
   * @note 弱指针 p 将先被转化为 std::shared_ptr<T> 强指针，如果该指针已被
   * 析构，将返回空的指针。
   */
  Pointer(const std::weak_ptr<T>& p) : std::shared_ptr<T>(p) {}

  /**
   * @brief 析构函数.
   * @note 先将灵敏指针所指内存空间的引用计数减1, 如果其值为0,
   * 则将灵敏指针设置为空并释放该内存空间,
   * 否则, 仅将灵敏指针设置为空, 不释放内存空间.
   */
  ~Pointer() {}

  /// @}
  /// @name 赋值和转换
  /// @{

  /**
   * @brief 标准指针赋值运算符
   */
  Pointer<T>& operator=(T* p) {
    Pointer<T>(p).swap(*this);
    return *this;
  }

  /**
   * @brief 标准指针动态转型赋值运算符
   */
  template <typename Y>
  Pointer<T>& operator=(Y* p) {
    Pointer<T>(p).swap(*this);
    return *this;
  }

  /**
   * @brief 灵敏指针赋值运算符
   */
  Pointer<T>& operator=(const Pointer<T>& src) {
    Pointer<T>(src).swap(*this);
    return *this;
  }

  /**
   * @brief 灵敏指针动态转型赋值运算符
   */
  template <typename Y>
  Pointer<T>& operator=(const Pointer<Y>& src) {
    Pointer<T>(src).swap(*this);
    return *this;
  }

  /**
   * @brief 灵敏指针隐式转换为标准指针.
   */
  operator T*() const { return this->get(); }

  /**
   * @brief 获取灵敏指针包含的标准指针.
   * @note 返回的标准指针不能被删除，否则灵敏指针析构时将出现重复删除导致程
   *       序崩溃.
   */
  T* getPointer() const { return this->get(); }

  /// @}
  /// @name 逻辑和其他
  /// @{

  /**
   * @brief 判断指针是否为空
   */
  bool operator!() { return this->get() == NULL; }

  /**
   * @brief 布尔值转型，判断指针是否非空
   */
  operator bool() const { return this->get() != NULL; }

  /**
   * @brief 显式判断灵敏指针是否为空.
   * @return 逻辑型, 真值表示灵敏指针为空.
   */
  bool isNull() const { return this->get() == NULL; }

  /**
   * @brief 设置灵敏指针为空.
   **/
  void setNull() { this->reset(); }
};
}  // namespace tbox
}  // namespace HSF

#endif  // JAUMIN_TBOX_POINTER_H
