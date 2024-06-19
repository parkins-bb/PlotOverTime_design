/**
 * @file: Region.hpp
 * @author: Ming Liu
 * @brief 计算域
 * @Email:  1420513894@qq.com
 * @Date:   2022-04-13
 * @Last Modified by:   Ming Liu, Hu Ren
 * @Last Modified time: 2022-04-22 10:35
 */

#ifndef Region_hpp
#define Region_hpp

#include "utilities.h"
// #include "utilInterfaces.h"
#include "setType.hpp"
#include "subSet.hpp"
#include "MeshLM.hpp"
#include "subField.hpp"
#include "hsfField.hpp"
#include "subRegion.hpp"
#include "utilityInterfaces.h"

namespace HSF
{
/**
 * @brief 计算域
 */
class Region:
    public Mesh
{
protected:
    const Word regName_;   ///< 计算域名

    //数据

    Table<Word, void*> fields_;  ///< 场数据

    Table<Word, void*> subRegions_; ///< 子域

    Table<Word, void*> topologies_; ///< 拓扑

    Table<Word, void*> equations_;  ///< 方程

    Table<Word, Procedure*> procedures_; ///< 流程

    Parameter* regPara_; ///< 参数

    Iterator* iterator_; ///<迭代器

    Communicator *commcator_; ///< 通信器

public:
    /*-----------------------------构造析构----------------------------*/
    //空构造
    Region(const Word& name, Communicator& other_comm);

    //从数据库Region参数构造
    Region(const DataTree& region, Communicator& other_comm);

    //从数据库网格参数构造
    Region(const Word& name, const DataTree& mesh, Communicator& other_comm);

    //析构
    ~Region();

    //读取参数
    void readPara(DataTree& para);

    //读取网格
    void readMesh(DataTree& mesh);

    //读取物理场
    void readField(DataTree& mesh);

    /*-------------------------基本信息获取----------------------------*/
    //获取计算域上的网格对象引用
    Mesh& getMesh(){return mesh_;};

    //获取计算域上的网格对象引用
    Word& getName(){return regName_;}

    //获得分布式计算域上本地单元数量
    llabel getLocalSize(){return mesh_.getLocalSize(Cell);}

    //获得分布式计算域上全局单元数量
    llabel getSize(){return mesh_.getGlobalSize(Cell);}

    //获得分布式计算域上的网格子域数量
    llabel getSubRegionNum(){return subRegions_.size();}

    /*-------------------------场操作----------------------------*/
        //获德计算域上场的数量
    label getFieldNum(){return fields_.size();};
    
    //增加物理场
    template<class BasicType>
    void addField(Word name0, Field<BasicType> *f);

    //删除场
    int deleteField(const Word name0);

    //获取物理场
    template<class BasicType>
    Field<BasicType>& getField(const Word name0);

    //启动进程边界场更新
    int updateField(const Word name0){ /*to be imple*/   };

    //完成更新场
    int finishUpdateField(const Word name0){ /*to be imple*/   };

    /*-------------------------子域操作----------------------------*/
    //添加子域
    void addSubRegion(Word name0, SubRegion *subReg0);

    //获取子域
    SubRegion& getSubRegion(Word name0);

    /*-------------------------方程操作----------------------------*/
    //增加方程
    template<class BasicType > 
    int addEquation(const Word& name0, const SetType& mainSet, const SetType& connectSet);

    //获得计算域上方程
    template<class BasicType>
    Equation<BasicType>& getEquation (const Word& name);

    //删除方程
    int deleteEquation(const Word& name0);
    
    //获得计算域上方程的数量
    label getEquationNum (){return equations_.size();};

    /*-------------------------拓扑迭代与过程栈----------------------------*/
    //根据名称，返回对应类型拓扑对象组
    Topology& getTopology (const Word& name);

    //根据主副位置类型返回拓扑对象引用
    Topology& getTopology (const SetType& mainSet, const SetType& connectSet );


    //创建迭代器
    void createIterator();

    //网格拓扑迭代计算
    int Iterate (Topology& topo, OperateFunction op, 
                std::vector<FieldNoType> field, std::vector<int32> para);

    //场迭代计算
    int Iterate (OperateFunction op, 
                std::vector<FieldNoType> field, std::vector<int32> para);
    
    //矩阵迭代计算
    //to be done

    //注册一个流程
    void procedureRegister(const Word& name0, Procedure* proc0);

    //获取一个流程
    Procedure& getProcedure (const Word& name0); 

    //执行一个流程
    int process (const Word& name0);

    //规约基本类型序列
    template<class BasicType> 
    int reduce(BasicType* result, const BasicType* para, 
        llabel num, RegRedcType type);
    

    /*-----------------负载均衡--------------------*/
    //负载均衡
    void loadBalance();


    /*--------------数据存储-----------------*/
    //全集IO
    void outputAll();

    //空间局部IO
    void output(BoundingBox box0);

    //数据子集IO
    void output(std::vector<label>& pointSets);

}; //end Region

/*----------template function----------------*/
template<class BasicType>
void Region::addField(Word name0, Field<BasicType> *f)
{
    if(fields_.count(name0)==0)
    {
        Region::fields_.insert(make_pair(name0, (FieldNoType*)f ));
    }
    else
    {
        Terminate("addField", "the field name has been addressed in the region");
    }
    
}

template<class BasicType > 
Equation<BasicType>& Region::getEquation (const Word& name0)
{
    if(equations_.count(name0)!=0)
    {
        return *((Equation<BasicType>*)equations_.find(name0));
    }
    else
    {
        Terminate("addEquation", "the equation name has been addressed in the region");
    }
}

template<class BasicType>
Field<BasicType>& Region::getField(const Word name0)
{
    if(fields_.count(name0)!=0)
    {
        return *((Field<BasicType>*)fields_.find(name0));
    }
    Terminate("There is no this field name in fields_ table.", name0.c_str());
    
}

Equation<BasicType>& HSF::Region::getEquation (const Word& name0)
{
    if(equations_.count(name0)!=0)
    {
        return *( (Equations<BasicType>*)(equations_.find(name0)) );
    }
    Terminate("There is no this equation name in equations_ table.", name0.c_str());
}

template<class BasicType > 
int addEquation(const Word& name0, const SetType& mainSet, const SetType& connectSet)
{
    //to be imple
}

template<class BasicType> 
int Region::reduce(BasicType* result, const BasicType* para, llabel num, RegRedcType type)
{
    //to be imple
}

}//end namespace HSF

#endif