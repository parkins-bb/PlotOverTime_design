/**
 * @file: subField.hpp
 * @author: Ming Liu
 * @brief 子域点集
 * @Email:  1420513894@qq.com
 * @Date:   2022-03-09
 * @Last Modified by:   Ming Liu
 * @Last Modified time: 2022-04-16 16:06
 */

#ifndef SUB_FIELD_hpp
#define SUB_FIELD_hpp

#include "subSet.hpp"
#include "utilities.h"
// #include "utilInterfaces.h"
#include "setType.hpp"
#include "utilityInterfaces.h"

namespace HSF
{

/**
 * @brief 无类型子域场
 * 
 */
class SubFieldNoType
{

protected:
    SubSet& sets_; ///< 子区域索引

    void* data_; ///< 子域场数据

    label eleSize_; ///< 元素大小

public:
    /**
     * @brief 构造
     * 分配子域场空间
     * @param[in] set0 子区域点集
     */
    SubFieldNoType(SubSet& set0, label eleSize);

    /**
     * @brief 获取子域场
     */
    void* getDataPtr(){return data_;};

    /**
     * @brief 获取子域场第i个元素的地址
     * 
     */
    void* getValuePtr(label i)
    {
        return (char*)data_ + i*eleSize_;
    }

    label getDataNum(){return sets_.getSize();}

    /**
     * @brief 通过通信完成子域场虚单元更新
     * 更新过程依赖子域索引、Mesh下的通信拓扑、子域场数据源
     * to be imple
     */
    // void update(){printf("update() to be implemented\n");};

    //增删查改 2022/4/7

    //inline函数

    /**
     * @brief 析构
     */
    ~SubFieldNoType(){ DELETE_POINTER(data_); };


};



/**
 * @brief 子域场
 * 
 */
template<class BasicType>
class SubField
{
protected:
    SubSet& sets_; ///< 子区域索引

    BasicType* data_; ///< 子域场数据

    label eleSize_;

public:
    /**
     * @brief 构造
     * 分配子域场空间
     * @param[in] set0 子区域点集
     */
    SubField(SubSet& set0);

    /**
     * @brief 获取子域场数据
     */
    BasicType* getData(){return data_;};

    /**
     * @brief 获取子域场第i个元素的值
     * 
     */
    inline BasicType& operator [](label i)
    {
        return data_[i];
    }

    /**
     * @brief 通过通信完成子域场虚单元更新
     * 更新过程依赖子域索引、Mesh下的通信拓扑、子域场数据源
     * to be imple
     */
    // void update(){printf("update() to be implemented\n");};

    //增删查改 2022/4/7

    //inline函数

    /**
     * @brief 析构
     */
    ~SubField(){ DELETE_POINTER(data_); };

    

};

    #include "subFieldI.hpp"

}

#endif