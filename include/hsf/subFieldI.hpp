/**
 * @file: subSetI.hpp
 * @author: Ming Liu
 * @brief 子域点集模板函数
 * @Email:  1420513894@qq.com
 * @Date:   2022-03-08
 * @Last Modified by:   Ming Liu
 * @Last Modified time: 2022-03-08 20:59
 */

#include "subField.hpp"
#include "subSet.hpp"

template<class BasicType>
HSF::SubField<BasicType>::SubField(SubSet& set0):sets_(set0)
{
    eleSize_ = (sizeof(BasicType));
    data_ = new BasicType[sets_.getSize()];
}