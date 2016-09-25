/* BSR_BBP.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      2-variable Binary Splitting routine template.
 * 
 *          P(a,b) = P(a,m)*Q(m,b) + P(m,b)*Q(a,m)
 *          Q(a,b) = Q(a,m)*Q(m,b)
 * 
 * 
 *      Most BBP formulas fall into this form.
 * 
 */

#pragma once
#ifndef NumberFactory_BSR_BBP_H
#define NumberFactory_BSR_BBP_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/Time/Time.h"
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
class BSR_BBP{
protected:
    //  To prevent memory from blowing up, don't start threading until the
    //  recursion is this deep.
    ukL_t m_thread_depth;

public:
    BSR_BBP(ukL_t thread_depth = 0) : m_thread_depth(thread_depth) {}

public:
    //  Computes the following using the recursion specified above.
    //      P(0, index)
    //      Q(0, index)
    void run(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, upL_t index, upL_t p, upL_t tds) const{
        BSR(P, Q, 0, index, 0, p, tds);
    }

private:
    //  Recursion End-Point:
    //      P = P(b - 1, b)
    //      Q = Q(b - 1, b)
    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, upL_t b, upL_t p) const = 0;

    void BSR(
        BigFloatO<wtype>& P, BigFloatO<wtype>& Q,
        upL_t a, upL_t b, ukL_t recursion_depth,
        upL_t p, upL_t tds
    ) const;

    template <typename wtype> friend class BSR_BBP_Action;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Parallel Helper Object
template <typename wtype>
class BSR_BBP_Action : public BasicAction{
    const BSR_BBP<wtype>& m_BSR;
    BigFloatO<wtype>& m_P;
    BigFloatO<wtype>& m_Q;
    upL_t m_a;
    upL_t m_b;
    ukL_t m_recursion_depth;
    upL_t m_p;
    upL_t m_tds;

public:
    BSR_BBP_Action(
        const BSR_BBP<wtype>& BSR,
        BigFloatO<wtype>& P, BigFloatO<wtype>& Q,
        upL_t a, upL_t b,ukL_t recursion_depth,
        upL_t p, upL_t tds
    )
        : m_BSR(BSR), m_P(P), m_Q(Q)
        , m_a(a), m_b(b), m_recursion_depth(recursion_depth)
        , m_p(p), m_tds(tds)
    {}
    virtual void run() override{
        m_BSR.BSR(m_P, m_Q, m_a, m_b, m_recursion_depth, m_p, m_tds);
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Implementations
template <typename wtype>
void BSR_BBP<wtype>::BSR(
    BigFloatO<wtype>& P, BigFloatO<wtype>& Q,
    upL_t a, upL_t b, ukL_t recursion_depth,
    upL_t p, upL_t tds
) const{
    //  End of recursion
    if (b - a == 1){
        BSR_End(P, Q, b, p);
        return;
    }

    if (recursion_depth == m_thread_depth){
        Console::print("Summing: ( ");
        Console::print_commas(a);
        Console::print(" : ");
        Console::print_commas(b);
        Console::println(" )");
    }

    //  Split down the middle.
    upL_t m = (a + b) / 2;

    //  Perform sub-recursions.
    BigFloatO<wtype> P0, Q0, P1, Q1;
    if (tds <= 1 || b - a < 1000 || recursion_depth < m_thread_depth){
        //  Don't Parallelize
        recursion_depth++;
        BSR(P0, Q0, a, m, recursion_depth, p, tds);
        BSR(P1, Q1, m, b, recursion_depth, p, tds);
    }else{
        //  Parallelize
        recursion_depth++;
        upL_t tds0 = tds / 2;
        upL_t tds1 = tds - tds0;
        BSR_BBP_Action<wtype> tp0(*this, P0, Q0, a, m, recursion_depth, p, tds0);
        BSR_BBP_Action<wtype> tp1(*this, P1, Q1, m, b, recursion_depth, p, tds1);
        Parallelism::run_in_parallel(tp0, tp1);
    }

    //  Combine

    //  P = P0*Q1 + P1*Q0
    P0 = mul(P0, Q1, p, tds);
    P1 = mul(P1, Q0, p, tds);
    P = add(P0, P1, p);
    P0.clear();
    P1.clear();

    //  Q = Q0*Q1
    Q = mul(Q0, Q1, p, tds);
    Q.strip_tzs_all();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Series Wrappers
template <typename wtype, ukL_t d_pow, bool alternate>
class Standard_BBP_BSR : public BSR_BBP<wtype>{
    siL_t m_g_pow;  //  Geometric Power
    wtype m_d0;
    wtype m_d1;

public:
    Standard_BBP_BSR(siL_t g_pow, wtype d0, wtype d1)
        : m_g_pow(g_pow), m_d0(d0), m_d1(d1)
    {
        //  Determine the threading depth using this heuristic.
        double depth = std::log2((double)d_pow / -g_pow) + 4.0;
        if (depth < 0){
            depth = 0;
        }
        this->m_thread_depth = (ukL_t)depth;
    }

    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, upL_t b, upL_t p) const override{
        //  P = 2^(g_pow * b)
        P = BigFloatO<wtype>(1);
        P <<= m_g_pow * b;
        if (alternate && (b % 2 == 1)){
            P.negate();
        }

        //  Q = (d1 * b + d0)^d_pow
        wtype factor = m_d0 + m_d1 * b;
        Q = BigFloatO<wtype>(factor);
        for (ukL_t c = 1; c < d_pow; c++){
            Q *= factor;
        }
    }
};
template <typename wtype, ukL_t d_pow, bool alternate>
BigFloatO<wtype> Standard_BBP_Series(
    siL_t g_pow, wtype d0, wtype d1,
    wtype coefN, wtype coefD, upL_t terms,
    upL_t p, upL_t tds
){
    //  Computes: (alternate == false)
    //
    //   coefN   terms (     2^(g_pow*k)     )
    //  -------  SUM   ( ------------------- )
    //   coefD   k=0   (  (d1*k + d0)^d_pow  )
    //
    //  Computes: (alternate == true)
    //
    //   coefN   terms (    (-2)^(g_pow*k)   )
    //  -------  SUM   ( ------------------- )
    //   coefD   k=0   (  (d1*k + d0)^d_pow  )
    //

    Console::print("Series: (");
    Console::print_marginr(2, d1);
    Console::print("k + ");
    Console::print_marginr(2, d0);
    Console::print(")^");
    Console::print(d_pow);
    Console::print("    ");
    Console::print_commas(terms);
    Console::println(" terms");

    Time::WallClock time0 = Time::WallClock::Now();

    BigFloatO<wtype> P, Q, tmp;
    Standard_BBP_BSR<wtype, d_pow, alternate>(g_pow, d0, d1).run(P, Q, terms, p, tds);
    wtype factor = d0;
    for (ukL_t c = 1; c < d_pow; c++){
        factor *= d0;
    }

    P *= factor * coefN;
    tmp = Q;
    tmp *= coefN;
    P = add(P, tmp, p);
    tmp.clear();

    Q *= factor * coefD;
    P = div(P, Q, p, tds);

    Time::WallClock time1 = Time::WallClock::Now();
    Console::print("Time:    "); Time::println_time_smart(time1 - time0, 'T');
    Console::println();

    return P;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
