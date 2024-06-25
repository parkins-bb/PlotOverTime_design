/**
 * @Author:   Gao Fei
 * @Date:     2022-04-13 15:00:01
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: topoFunctions.hpp
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-07-05 22:20:17
 *	
 * @Description: 
 *	
 */

#ifndef topoFunctions_H
#define topoFunctions_H

#include "dummyTopology.hpp"
#include "Iterators.hpp"

using namespace HSF;

template<typename T>
struct MVFunctor 
{
    inline int operator()(
        int ENT, int ROW, int COL, 
        std::vector<T>& A, std::vector<T>& x, std::vector<T>& b, std::vector<T>& alpha)
    {
        b[ROW] += alpha[0] * ( A[ENT] * x[COL] );   
    }
};

// 辅助函数方案
// 单一算子
template<typename T>
void spMV(
    Matrix<T>& A,      // Matrix
    std::vector<T>&  x,      // Column vector
    std::vector<T>&  b,      // Row vector
    std::vector<T>&  alpha
    )  
{
    // 1. 封装数据
    Topology& M = A.getTopo();
    DataSet<T, Mat, IN > _A(A.getEntries());
    DataSet<T, Col, IN > _x(x);
    DataSet<T, Row, OUT> _b(b);
    DataSet<T, Par, IN > _alpha(alpha);

    // 2. 定义算子
    MVFunctor<T> spmv;

    // 3.启动迭代
    HSF::iterate(M, spmv, _A, _x, _b, _alpha);
}

void spMV_labbda()
{
    //
}

// 耦合算子
/*template<typename T>
void interpolateCoupled(
    const Matrix<T>& w,
    const std::vector<T>& x1,   // Column 1, eg U
          std::vector<T>& fx1,  // Row 1
    const std::vector<T>& x2,   // Column 2, eg p
          std::vector<T>& fx2,  // Row 2
    const T alpha1,       // Parameter 1
    const T alpha2        // Parameter 2
    )
{

    // 1. 封装数据
    const Topology& M = w.getTopo();
    DataSet<Mat, IN > _w( w.getEntries() );
    DataSet<Col, IN > _x1( x1 );
    DataSet<Row, OUT> _fx1( fx1 );
    DataSet<Col, IN > _x2( x2 );
    DataSet<Row, OUT> _fx2( f2 );
    DataSet<Par, IN > _alpha1( alpha1 );
    DataSet<Par, IN > _alpha2( alpha2 );

    // 2. 定义算子
    template<>
    struct MVFunctor<std::vector<T>, T> 
    {
        inline int operator()(
            int ENT, int ROW, int COL, 
            std::vector<T>& w, 
            std::vector<T>& x1,  std::vector<T>& x2, 
            std::vector<T>& fx1, std::vector<T>& fx2, 
            T& alpha1,     T& alpha2)
        {
            fx1[ROW] += alpha1 * dot(w[ENT], x1[COL]);  
            fx2[ROW] += alpha2 * dot(w[ENT], x2[COL]); 
        }
    };
    MVOpt<std::vector<T>, T> intplt_coupled; 

    // 3. 启动迭代
    HSF::iterate(M, intplt_coupled, _w, _x1, _fx1, _x2, _fx2, _alpha1, _alpha2);
}*/


#endif // end topoFunctions_H


/* old code
/*
// 成员函数方案
template<typename T>
void spMV(
    const Matrix<T>& A,      // Matrix
    const std::vector<T>&  x,      // Column vector
          std::vector<T>&  b,      // Row vector
    const T&         alpha   // Parameters 
    )  
{
    // 定义算子
    template<>
    struct MVOpt<std::vector<T>, T> 
    {
        inline int operator()(
            int ENT, int ROW, int COL, 
            std::vector<T>& A, std::vector<T>& x, std::vector<T>& b, T& alpha)
        {
            b[ROW] += alpha * dot(A[ENT], x[COL]);   
        }
    };

    // 生成迭代器
    auto iter = Iterator<MVOpt<std::vector<T>, T>>::New(A.getTopo());

    // 封装数据
    iter.addData<Mat, IN >( A.getEntries() );
    iter.addData<Col, IN >( x );
    iter.addData<Row, OUT >( b );
    iter.addData<Par, IN >( alpha );

    // 启动迭代
    iter.iterate();

    // 清理数据
    iter.flush();
}
template <class T>
class V2E_Functor
{
public:
    void operator()(T& w, T& c, T& f, ParDataSet& alpha)
    {
        f += alpha[0] * w * c;
    }
};

class E2V_Functor
{
public:
    void operator()(T& w, T& c, T& f, ParDataSet& alpha)
    {
        c += alpha[0] * w * f;
    }
};

class V2V_Functor
{
public:
    void operator()(T& A, T& x, T& b, ParDataSet& alpha)
    {
        b += alpha[0] * A * x;
    }
};


Tensor<typename Cmpt, typename Rank>
using Tensor = TensorSpace<Cmpt, Rank, 3, Rank*Rank> ;

struct V2V_Functor
{
    inline void operator(int ENT, int ROW, int COL, ...)???
    {
        // 获取数据
        auto A = ???
        hsf_fetchArgs<const DataSet<Matrix, IN, Tensor<scalar, 2, 3> > >();

        // 执行操作
        b[ROW] += alpha * dot(A[ENT], x[COL]);

    }
}

*/
