/**
 * @file: CoupledSubRegionI.hpp
 * @author: Ming Liu
 * @brief 耦合子域模板方法
 * @Email:  1420513894@qq.com
 * @Date:   2022-03-13
 * @Last Modified by:   Ming Liu, Hu Ren
 * @Last Modified time: 2022-03-13 14:29
 */

#include "CoupledSubRegion.hpp"

HSF::CoupledSubRegion::CoupledSubRegion(SetType geoType0, Mesh& mesh0, Word name0, Topology& topo_):
SubRegion(geoType0, mesh0, name0)
{
    neiMesh_ = new Mesh(topo_);
    // neiRegion = new Region(*neiMesh); 
}


