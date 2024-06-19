/**
 * @Author:   Gao Fei
 * @Date:     2022-04-15 17:08:45
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: dummyTopology.hpp
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-05-22 13:07:16
 *	
 * @Description: 
 *	
 */

#ifndef dummyTopology_H
#define dummyTopology_H

#include <cstdlib>
#include <cstdio>
#include <vector>

//#define Matrix dummyMatrix
//#define Topology dummyTopology

#define std::vector vector
typedef int label;

using namespace std;

namespace HSF {

template<typename... Args>
struct MVFunctors
{
    inline int operator()(
        int ENT, int ROW, int COL, Args... args )
    {
        printf("this class template must be specialized");
        std::abort();
        //BOOST_ASSERT_MSG("this class template must be specialized");
    }
};

class Topology
{
private:
    /**
     * @brief      two label pair, for nozero coordinate
     */
    class IndexSet
    {
        label rowId;
        label colId;
    public:
        label& row(){return rowId;}
        label& col(){return colId;}
    };

protected:
    // - core member data
    std::vector<IndexSet> nzCoord;
    label m_;
    label n_;


public:
    // - constructors and destructor
    Topology(label* row, label* col, label nnz, label m, label n)
    :m_(m), n_(n)
    {
        define(row, col, nnz, m, n);
    }
    Topology(){}
    void define(label* row, label* col, label nnz, label m, label n)
    {
        m_=m;
        n_=n;
        nzCoord.resize(nnz);
        for (int i = 0; i<nzCoord.size();i++)
        {
            nzCoord[i].row() = row[i];
            nzCoord[i].col() = col[i];
        }
    }
    ~Topology(){};

    // - member functions operations
    //virtual void construct()=0;

    // - member functions access
    IndexSet& operator[](label i){ return nzCoord[i]; }
    label getNumNZ(){return nzCoord.size();}

private:
    // - member functions relization

    // - member data relization

};// end class Topology


/*Topology& consTopologyIncEdge()
{
    //
}*/

template<typename T>
class Matrix
{
    Topology& topo;
    std::vector<T>& nonz;

public:
    Matrix(Topology& topo_, std::vector<T>& nonz_)
        :topo(topo_), nonz(nonz_)
    {
        //
    }
    ~Matrix(){};

    T& operator[](label i)
    {
        return nonz[i];
    }

    Topology& getTopo()
    {
        return topo;
    }

    std::vector<T>& getEntries()
    {
        return nonz;
    }

    label getNumNZ(){return nonz.size();}

}; // end class dummyMatrix

void consTopoAdjFromGraph(
    Topology& M, int order, std::vector<int>& owner, std::vector<int>& neighbor );

} // end namespace

#endif // Topology_H


// old code
/*
class TopoCOO
    :public Topology
{

public:
    GraphTopology()
    {
        iter = new IteratorCOO();
    }

public:
    virtual void construct()=0;
};

class TopoMultiBand
    :public Topology
{
public:
    TopologyMultiBand()
    {
        iter = new IteratorMultiBand();
    }

public:
    virtual void construct();
};


*/

/******************************** old code ***********************************/
/**
 * @brief      for unstructure grid intepolate
 * 
 * @details    M * v = e, or M * c = f
 */
/*class TopoCooIncEdge
    :public GraphTopology
{
public:
    virtual void construct();
};*/

/**
 * @brief      for unstructure grid integrate
 *
 * @details    M * e = v, or M * f = c 
 */
/*class TopoCooIncVert
    :public GraphTopology
{
public:
    virtual void construct();
};*/

/**
 * @brief      for unstructure grid spmv
 *
 * @details    A * x = b
 */
/*class TopoCooAdj
    :public GraphTopology
{
public:
    virtual void construct();
};*/


