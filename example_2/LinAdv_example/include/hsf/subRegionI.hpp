/**
 * @file: subRegionI.hpp
 * @author: Ming Liu
 * @brief 子域
 * @Email:  1420513894@qq.com
 * @Date:   2022-03-10
 * @Last Modified by:   Ming Liu, Hu Ren
 * @Last Modified time: 2022-04-17 18:38
 */

#include "subRegion.hpp"

// template<class SetType>
// HSF::SubRegion<SetType>::SubRegion(SubRegion& subRegion0):
// SubSet<SetType>(subRegion0)
// {
//    if(subRegion0.isLocMemFun()) ///< 深拷贝
//    {
//       *(this->field_) = *(subRegion0.getSubFieldObj());
//    }else ///< 浅拷贝
//    {
//       this->field_ = subRegion0.getSubFieldObj();
//    }
// }

// template<class SetType, class BasicType>
// HSF::SubRegion<SetType,BasicType>::SubRegion(Mesh& mesh0, Word& name0, label* index0):
// SubSet<SetType>(mesh0, name0, index0)
// {
//     field_ = new SubField<SetType, BasicType>(*this);
// }

template <class BasicType>
SubField<BasicType>& SubRegion::getSubField(const Word name0)
{
   if(fields_.count(name0)!=0)
   {
      return *((SubField<BasicType>*)fields_[name0]);
   }
   Terminate("There is no this subfield name in fields_ table.", name0.c_str());
}

template<class BasicType>
void SubRegion::addField(Word name0, SubField<BasicType> *f)
{
   if(fields_.count(name0)==0)
   {
      fields_.insert(make_pair(name0, (SubFieldNoType*)f));
   }
   else
   {
      Terminate("addSubField", "the subfield name has been addressed in the subRegion");
   }
}


