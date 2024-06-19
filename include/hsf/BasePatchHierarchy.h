#ifndef SOURCE_WRAPPER_HSFBASEPATCHHIERARCHY_H
#define SOURCE_WRAPPER_HSFBASEPATCHHIERARCHY_H

#include "Pointer.h"

namespace HSF {
namespace hier {
/*!
 * @brief 网格片层次结构基类 hier::BasePatchHierarchy
 * 为网格片层次结构提供统一的访问接口。
 *
 * @if DEV
 * 该类允许 JAUMIN 及其应用程序中的高层类采用抽象的方式访问网格片层次结构，
 * 而无需知道网格片层次结构的具体类型。
 * @endif
 *
 * @see hier::PatchHierarchy
 */
template <int DIM>
class BasePatchHierarchy {
 public:
  /*!
   * @brief 构造函数。
   */
  BasePatchHierarchy() {}

  /*!
   * @brief 析构函数。
   */
  virtual ~BasePatchHierarchy() {}

  /*!
   * @brief 获取该网格片层次结构的网格层总数。
   * @return 整型，表示网格层总数。
   */
  virtual int getNumberOfLevels() const = 0;

 private:
};

}  // namespace hier
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFBASEPATCHHIERARCHY_H */
