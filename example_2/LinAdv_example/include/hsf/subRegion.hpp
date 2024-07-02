/**
 * @file: subRegion.hpp
 * @author: Ming Liu
 * @brief 子域
 * @Email:  1420513894@qq.com
 * @Date:   2022-03-10
 * @Last Modified by:   Ming Liu, Hu Ren
 * @Last Modified time: 2022-04-17 17:32
 */

#ifndef Sub_Region_hpp
#define Sub_Region_hpp

#include "utilities.h"
// #include "utilInterfaces.h"
#include "setType.hpp"
#include "subSet.hpp"
#include "MeshLM.hpp"
#include "subField.hpp"
#include "utilityInterfaces.h"

namespace HSF
{
/**
 * @brief 子域
 * @tparam[in] SetType 子域对应网格要素类型
 */
class SubRegion:
    public SubSet
{
private:
    // bool isLocMem_; ///< 是否本地分配场空间。

protected:
    Table<Word, void*> fields_; ///< 子域场

//通讯：指针
public:
    /**
     * @brief 构造1
     * 
     * @param[in] geoType0    子域集合中元素的几何类型
     * @param[in] mesh0       所属计算域网格 
     * @param[in] name0        子域某边界名
     */
    SubRegion(SetType geoType0, Mesh& mesh0, Word name0);


    /**
     * @brief 拷贝构造
     * 应考虑SubRegion源是深拷贝还是浅拷贝
     */
    // SubRegion(SubRegion<SetType>& subRegion0);

    /**
     * @brief 获取子域场数据指针集合
     * 
     */
    Table<Word, void*>& getSubFields(){return fields_;}

    /**
     * @brief 获取一个子域场
     * 
     * @tparam BasicType   基本数据元素类型
     * @param name0        子域场名
     */
    template <class BasicType>
    SubField<BasicType> &getSubField(const Word name0);

    /**
     * @brief 添加一个子域场
     * 
     * @tparam BasicType   基本数据元素类型
     * @param name0        子域场名
     * @param f            子域场对象指针
     */
    template<class BasicType>
    void addField(Word name0, SubField<BasicType> *f);
        
    /**
     * @brief 更新子域场
     * 
     */
    void update(){ printf("update() to be implemented\n");};

    /**
     * @brief 
     * 
     */
    // bool isLocMemFun(){return isLocMem_;}

    /**
     * @brief 子域迭代器
     * 
     * @param subField0   子域场
     * @param fields      计算域场序列
     * @param paras       参数
     * @param op          自定义点操作算子
     * @return int 
     */
    // int iteration(SubField& subField0, std::vector<Field*> fields, Parameter& paras, PointWiseOperator op);

    /**
     * @brief 析构
     * 
     */
    ~SubRegion();  // to be implement};

    

};

    #include "subRegionI.hpp"

}

#endif