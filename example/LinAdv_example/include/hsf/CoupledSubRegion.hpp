/**
 * @file: CoupledSubRegion.hpp
 * @author: Ming Liu
 * @brief 子域
 * @Email:  1420513894@qq.com
 * @Date:   2022-03-13
 * @Last Modified by:   Ming Liu, Hu Ren
 * @Last Modified time: 2022-03-13 14:29
 */

#ifndef Coupled_Sub_Region_hpp
#define Coupled_Sub_Region_hpp

#include "utilities.h"
// #include "utilInterfaces.h"
#include "setType.hpp"
#include "subSet.hpp"
#include "MeshLM.hpp"
#include "utilityInterfaces.h"

namespace HSF
{
/**
 * @brief 耦合子域
 * @tparam[in] SetType 子域对应网格要素类型
 * @tparam[in] Element 子域场基本类型
 */
template<class SetType>
class CoupledSubRegion:
    public SubRegion
{
protected:
    Mesh* neiMesh_; //邻居相关几何信息
    // Region* neiData; //存放与邻居相关的几何信息、场信息。

public:
    /**
     * @brief 构造
     * 
     * @param[in] mesh0       所属计算域网格 
     * @param[in] name0        子域某边界名
     * @param[in] topo_   耦合邻近拓扑关系
     */
    CoupledSubRegion(SetType geoType0, Mesh& mesh0, Word name0, Topology& topo_);

    /**
     * @brief 构造
     * 
     * @param[in] mesh0       所属计算域网格 
     * @param[in] name0        子域某边界名
     * @param[in] topo_   耦合邻近拓扑关系
     * @param[in] topo_   耦合邻近拓扑关系
     */
    // CoupledSubRegion(Mesh& mesh0, Word& name0, label* index0, Topology& topo_);

    /**
     * @brief 获取耦合拓扑
     * 
     * @return Topology& 
     */
    // Topology& getTopo(){neiData->getTopo();};

    /**
     * @brief 获取邻居耦合几何
     * 几何以网格形式存储
     * @return Mesh& 
     */
    // Mesh& getNeiGeometry(){neiData->getMesh();};

    /**
     * @brief 获取邻居耦合场
     * 
     * @return Field& 
     */
    // Field& getNeiField(){neiData->getField();};

    

};

    #include "CoupledSubRegionI.hpp"

}

#endif