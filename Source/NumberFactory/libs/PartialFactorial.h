/* PartialFactorial.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/20/2015
 * Last Modified    : 08/09/2015
 * 
 *      b! / a! using Binary Splitting
 * 
 */

#pragma once
#ifndef NumberFactory_PartialFactorial_H
#define NumberFactory_PartialFactorial_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "ymp/BigIntO.h"
#include "ymp/Parallelism.h"
#include "Utils.h"
namespace NumberFactory{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype>
BigIntO<wtype> Factorial_BSR(wtype a, wtype b, upL_t tds);

template <typename wtype>
class Factorial_Action : public BasicAction{
    BigIntO<wtype>& P;
    wtype a;
    wtype b;
    upL_t tds;

public:
    Factorial_Action(BigIntO<wtype>& P, wtype a, wtype b, upL_t tds)
        : P(P), a(a), b(b), tds(tds)
    {}
    virtual void run() override{
        P = Factorial_BSR<wtype>(a, b, tds);
    }
};

template <typename wtype>
BigIntO<wtype> Factorial_BSR(wtype a, wtype b, upL_t tds){
    if (b - a == 1){
        return b;
    }

    wtype m = (a + b) / 2;

    BigIntO<wtype> P0, P1;

    if (tds <= 1 || b - a < 1000){
        //  Don't Parallelize
        P0 = Factorial_BSR(a, m, tds);
        P1 = Factorial_BSR(m, b, tds);
    }else{
        upL_t tds0 = tds / 2;
        upL_t tds1 = tds - tds0;
        Factorial_Action<wtype> tp0(P0, a, m, tds0);
        Factorial_Action<wtype> tp1(P1, m, b, tds1);
        Parallelism::run_in_parallel(tp0, tp1);
    }

    return mul(P0, P1, tds);
}

template <typename wtype>
BigIntO<wtype> PartialFactorial(wtype a, wtype b, upL_t tds){
    if (b < a){
        return BigIntO<wtype>();
    }
    if (b == a){
        return BigIntO<wtype>(1);
    }
    return Factorial_BSR<wtype>(a, b, tds);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
