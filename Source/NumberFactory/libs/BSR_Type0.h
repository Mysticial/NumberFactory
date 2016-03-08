/* BSR_Type0.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      Standard 3-variable Binary Splitting routine template.
 * 
 *          P(a,b) = P(a,m)*Q(m,b) + P(m,b)*R(a,m)
 *          Q(a,b) = Q(a,m)*Q(m,b)
 *          R(a,b) = R(a,m)*R(m,b)
 * 
 * 
 *      The vast majority of Binary Splitting recursions have this form. So we
 *  put a reusable implementation here.
 * 
 */

#pragma once
#ifndef NumberFactory_BSR_Type3_H
#define NumberFactory_BSR_Type3_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "ymp/Parallelism.h"
#include "ymp/BigFloatO.h"
namespace NumberFactory{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Binary Splitting base class
template <typename wtype>
class BSR_Type0{
protected:
    //  To prevent memory from blowing up, don't start threading until the
    //  recursion is this deep.
    ukL_t thread_depth;

public:
    BSR_Type0(ukL_t thread_depth = 0) : thread_depth(thread_depth) {}

    //  Computes the following using the recursion specified above.
    //      P(0, index)
    //      Q(0, index)
    void run(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, upL_t index, upL_t p, upL_t tds) const{
        BSR(P, Q, nullptr, 0, index, 0, p, tds);
    }

private:
    //  Recursion End-Point:
    //      P = P(b - 1, b)
    //      Q = Q(b - 1, b)
    //      R = R(b - 1, b)
    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R, upL_t b, upL_t p) const = 0;

    void BSR(
        BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R,
        upL_t a, upL_t b, ukL_t recursion_depth,
        upL_t p, upL_t tds
    ) const;

    template <typename wtype> friend class BSR_Type0_Action;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Parallel Helper Object
template <typename wtype>
class BSR_Type0_Action : public BasicAction{
    const BSR_Type0<wtype>& BSR;
    BigFloatO<wtype>& P;
    BigFloatO<wtype>& Q;
    BigFloatO<wtype>* R;
    upL_t a;
    upL_t b;
    ukL_t recursion_depth;
    upL_t p;
    upL_t tds;

public:
    BSR_Type0_Action(
        const BSR_Type0<wtype>& BSR,
        BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R,
        upL_t a, upL_t b, ukL_t recursion_depth,
        upL_t p, upL_t tds
    )
        : BSR(BSR)
        , P(P), Q(Q), R(R)
        , a(a), b(b), recursion_depth(recursion_depth)
        , p(p), tds(tds)
    {}
    virtual void run() override{
        BSR.BSR(P, Q, R, a, b, recursion_depth, p, tds);

    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Implementations
template <typename wtype>
void BSR_Type0<wtype>::BSR(
    BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R,
    upL_t a, upL_t b, ukL_t recursion_depth,
    upL_t p, upL_t tds
) const{
    //  End of recursion
    if (b - a == 1){
        BSR_End(P, Q, R, b, p);
        return;
    }

    if (recursion_depth == thread_depth){
        Console::print("Summing: ( ");
        Console::print_commas(a);
        Console::print(" : ");
        Console::print_commas(b);
        Console::println(" )");
    }

    //  Split down the middle.
    upL_t m = (a + b) / 2;

    //  Perform sub-recursions.
    BigFloatO<wtype> P0, Q0, R0, P1, Q1, R1;

    if (tds <= 1 || b - a < 1000 || recursion_depth < thread_depth){
        //  Don't Parallelize
        recursion_depth++;
        BSR(P0, Q0, &R0, a, m, recursion_depth, p, tds);
        BSR(P1, Q1, &R1, m, b, recursion_depth, p, tds);
    }else{
        //  Parallelize
        recursion_depth++;
        upL_t tds0 = tds / 2;
        upL_t tds1 = tds - tds0;
        BSR_Type0_Action<wtype> tp0(*this, P0, Q0, &R0, a, m, recursion_depth, p, tds0);
        BSR_Type0_Action<wtype> tp1(*this, P1, Q1, &R1, m, b, recursion_depth, p, tds1);
        Parallelism::run_in_parallel(tp0, tp1);
    }

    //  Combine

    //  P = P0*Q1 + P1*R0
    P0 = mul(P0, Q1, p, tds);
    P1 = mul(P1, R0, p, tds);
    P = add(P0, P1, p);
    P0.clear();
    P1.clear();

    //  Q = Q0*Q1
    Q = mul(Q0, Q1, p, tds);
    Q.strip_tzs_all();
    Q0.clear();
    Q1.clear();

    //  R = R0*R1
    if (R != nullptr){
        *R = mul(R0, R1, p, tds);
        R->strip_tzs_all();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
