/* Catalan_Lupas.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *                 1   inf (         2^(8k) (40k^2 - 24k + 3) (2k)!^3 (k!)^2    )
 *      Catalan = ---- SUM ( (-1)^k ------------------------------------------- )
 *                 64  k=1 (                   k^3 (2k - 1) (4k)!^2             )
 * 
 */

#include "NumberFactory/libs/BSR_Type0.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
using wtype = u64_t;

class Catalan_Lupas_BSR final : public BSR_Type0<wtype>{
public:
    Catalan_Lupas_BSR() : BSR_Type0<wtype>(6) {}

    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R, upL_t b, upL_t p) const override{
        BigFloatO<wtype> tmp;
        if (R == nullptr){
            R = &tmp;
        }

        //  R(b - 1, b) = 32 (2b - 1) b^3
        *R = BigFloatO<wtype>(32);
        *R *= b;
        *R *= b;
        *R *= b;
        *R *= 2*b - 1;

        //  P(b - 1, b) = (40b^2 + 56b + 19) R(a,b)
        P = BigFloatO<wtype>(40);
        P *= b; P = add(P, BigFloatO<wtype>(56), p);
        P *= b; P = add(P, BigFloatO<wtype>(19), p);
        P = mul(P, *R, p);
        if (b % 2 == 1)
            P.negate();

        //  Q(b - 1, b) = (4b + 1)^2 (4b + 3)^2
        Q = BigFloatO<wtype>(4*b + 1);
        Q *= 4*b + 1;
        Q *= 4*b + 3;
        Q *= 4*b + 3;
    }
};

class Catalan_Lupas_Session : public ComputeFloatSession<wtype>{
public:
    Catalan_Lupas_Session(){
        m_name_short = "Catalan";
        m_name_long = "Catalan's Constant";
        m_algorithm_short = "Lupas";
        m_algorithm_long = "Lupas Formula";
        m_over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();
        const double LOG_RATIO = 0.5;   //  log(2)/log(4)
        const double TERM_RATIO = LOG_RATIO * CHAR_BIT * sizeof(wtype);

        upL_t p = m_precision;
        upL_t terms = (upL_t)((double)p * TERM_RATIO + 1);

        Console::print("Summing Series...  ");
        Console::print_commas(terms);
        Console::println(" terms");
        BigFloatO<wtype> P, Q, tmp;
        Catalan_Lupas_BSR().run(P, Q, terms, p, m_tds);

        tmp = Q;
        tmp *= 19;
        P = add(P, tmp, p);
        Q *= 18;
        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time1 - time0, 'T');

        Console::println("Division...");
        P = div(P, Q, p, m_tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time2 - time1, 'T');

        return P;
    }
};

void Catalan_Lupas(){
    Catalan_Lupas_Session().run();
}

}
