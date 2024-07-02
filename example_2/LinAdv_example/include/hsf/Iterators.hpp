/**
 * @Author:   Gao Fei
 * @Date:     2022-04-14 17:45:11
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: Iterators.hpp
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-07-05 21:48:10
 *	
 * @Description: 
 *	
 */

#ifndef Iterators_H
#define Iterators_H

#include "dummyTopology.hpp"

namespace HSF 
{

enum  TopoPosi //class
{
    Mat=1,
    Row,
    Col,
    Arr,
    Par
};

enum  ItrInOut //class
{
  IN=1, OUT, INOUT
};

template<typename T, int DataPosi, int DataUpdt>
struct DataSet
{
    DataSet(std::vector<T>& data);
    ~DataSet(){};

    // 数据属性
    std::vector<T>& arr;
    TopoPosi posi;
    ItrInOut updt;
    
    // 数据描述
    // data array holder. 多个数组的指针
    //char **arrays;
    // data array dimensions。 数组一个元素内浮点数数量
    //label *dims;
    // data array memory length。 一个数组的占用内存长度
    //label *length;
    // data array action。 一个数组的数据传输属性
    ////label *InOuts;
    // data array number。 数组的数量
    //label arrayNum;
    // data array sizes。 一个数组的元素数量
    //label arraySize;
};

template<typename SprsFunc, typename... Args>
void iterate(Topology& M, SprsFunc opt, Args... args);

} // end namespace HSF

#include "Iterators.cpp"

#endif // Iterators_H


// old code
/*
#define DS_TYPE_NUM 4

template <typename SprsFunc>
class Iterator
{
    // - core member data
    Topology& M;
    DataSet dataSet[DS_TYPE_NUM];
    SprsFunc spOpt;
    label optNum;

public:
    // - constructors and destructor
    static Iterator& New(Topology& topo)
    {
        return new ???
    }

    Iterator()
    {
        initData();
    }

    // - member functions operations
    void addData();

    virtual void iterate();

    void flush();
private:
    void initData();
}

class IteratorCOO
    :public Iterator
{
    //
}

class IteratorCooX86
    :public IteratorCOO
{
    // - core member data

public:
    // - constructors and destructor
    New();

    // - member functions operations
    virtual void iterate()
    {
        // 分段. 需获取排序信息

        // 染色

        // 启动线程
    }
    // - member functions access

private:
    // - member functions relization
    virtual void spwn()
    {
        //
    }

    // - member data relization

};

auto& hsf_fetchArgs()
{
    //
}

} // end namespace ITER

} // end namespace HSF

#endif //end ifndef


class IteratorCooX86
    :public IteratorCOO
{
    void operator()(
        Topology& M, 
        SparsFunc& sparseOpt, 
        MatDataSet& matData, 
        ColDataSet& colData, 
        RowDataSet& rowData,
        ParDataSet& parData)
    {
        for (label i = 0; i < M.numNz(); ++i)
        {
            label row = M[i].row();
            label col = M[i].col();
            sparseOpt(
                matData[i],
                colData[col],
                rowData[row],
                parData
                );
        }
    }
}
*/
