/**
 * @Author:   Gao Fei
 * @Date:     2022-04-13 15:00:01
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: topoOperators.hpp
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-04-15 16:21:34
 *	
 * @Description: 
 *	
 */

namespace hsf {

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

class SPMV_Functor
{
public:
    void operator()(T& A, T& x, T& b, ParDataSet& alpha)
    {
        b += alpha[0] * A * x;
    }
};

}

