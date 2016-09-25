/* ArcCot.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      Inverse Cotangent and Hyperbolic Cotangent
 * 
 */

#pragma once
#ifndef NumberFactory_ArcCot_H
#define NumberFactory_ArcCot_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <vector>
#include "BSR_Type0.h"
#include "ComputeFloatSession.h"
namespace NumberFactory{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Binary Splitting Class
template <typename wtype, bool hyperbolic>
class ArcCot_BSR final : public BSR_Type0<wtype>{
    wtype m_x;

public:
    ArcCot_BSR(wtype x)
        : m_x(x)
    {
        //  Determine the threading depth using this heuristic.
        double fat_estimate = 1 / std::log(x);
        double depth = std::log2(fat_estimate) + 4.0;
        if (depth < 0){
            depth = 0;
        }
        this->m_thread_depth = (ukL_t)depth;
    }

    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R, upL_t b, upL_t p) const override{
        //  P(b - 1, b) = (-1)^b
        P = BigFloatO<wtype>(1);
        if (!hyperbolic && b % 2 == 1){
            P.negate();
        }

        //  Q(b - 1, b) = (2b+1) x^2
        Q = BigFloatO<wtype>(2*b + 1);
        Q *= m_x;
        Q *= m_x;

        //  R(b - 1, b) = (2b+1)
        if (R != nullptr){
            *R = BigFloatO<wtype>(2*b + 1);
        }
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Main Function
template <typename wtype, bool hyperbolic>
BigFloatO<wtype> ArcCot_T(wtype x, wtype coef, upL_t p, upL_t tds){
    //  Computes:
    //      hyperbolic = false      ->  coef * ArcCot(x)
    //      hyperbolic = true       ->  coef * ArcCoth(x)

    const upL_t BITS_PER_WORD = CHAR_BIT * sizeof(wtype);
    const double RLOG2 = 0.69314718055994530941723212145817656807550013436026;
    const double RATIO = RLOG2 * BITS_PER_WORD * 0.5;

    if (x < 2){
        throw "ArcCot(x): x < 2";
    }

    upL_t terms = (upL_t)(RATIO * p / std::log((double)x)) + 1;

    Console::print(coef);
    if (hyperbolic){
        Console::print(" * ArcCoth(");
    }else{
        Console::print(" * ArcCot(");
    }
    Console::print(x);
    Console::print(")    ");
    Console::print_commas(terms);
    Console::println(" terms");

    Time::WallClock time0 = Time::WallClock::Now();

    BigFloatO<wtype> P, Q;
    ArcCot_BSR<wtype, hyperbolic>(x).run(P, Q, terms, p, tds);
    P = add(P, Q, p);
    Q *= x;
    P = div(P, Q, p, tds);
    Q.clear();

    if (coef != 1)
        P *= coef;

    Time::WallClock time1 = Time::WallClock::Now();
    Console::print("Time:    "); Time::println_time_smart(time1 - time0, 'T');
    Console::println();

    return P;
}
template <typename wtype>
BigFloatO<wtype> ArcCot(wtype x, wtype coef, upL_t p, upL_t tds){
    return ArcCot_T<wtype, false>(x, coef, p, tds);
}
template <typename wtype>
BigFloatO<wtype> ArcCoth(wtype x, wtype coef, upL_t p, upL_t tds){
    return ArcCot_T<wtype, true>(x, coef, p, tds);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Machin Formula
template <typename wtype>
struct MachinTerm{
    wtype x;
    siL_t coef;
};
template <typename wtype>
using MachinFormula = std::vector<MachinTerm<wtype>>;

template <typename wtype, bool hyperbolic>
class MachinFormula_Session : public ComputeFloatSession<wtype>{
    const MachinFormula<wtype>& m_formula;

public:
    MachinFormula_Session(const MachinFormula<wtype>& formula, const std::string& name, const std::string& algorithm)
        : m_formula(formula)
    {
        this->m_name_short = name;
        this->m_algorithm_short = algorithm;
        this->m_over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();

        upL_t p = m_precision;
        BigFloatO<wtype> T;
        for (auto& term : m_formula){
            if (term.coef > 0){
                T = add(T, ArcCot_T<wtype, hyperbolic>(term.x, term.coef, p, m_tds), p);
            }else{
                T = sub(T, ArcCot_T<wtype, hyperbolic>(term.x, -term.coef, p, m_tds), p);
            }
        }
        Time::WallClock time1 = Time::WallClock::Now();
        return T;
    }

    void operator=(const MachinFormula_Session&) = delete;
};

template <typename wtype, bool hyperbolic>
void Run_MachinFormula(const MachinFormula<wtype>& formula, const std::string& name, const std::string& algorithm){
    //  Sums up the specified machin-like formula.
    MachinFormula_Session<wtype, hyperbolic>(formula, name, algorithm).run();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
