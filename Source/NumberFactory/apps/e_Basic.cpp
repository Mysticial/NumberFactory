/* e_Basic.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 06/20/2015
 * Last Modified    : 07/18/2015
 * 
 *      A very basic (non-parallelized) implementation of the e constant.
 * 
 *  This implementation is optimized to be easy to read. A more advanced
 *  implementation can be found in "e_parallel.cpp".
 * 
 */

#include "NumberFactory/libs/Utils.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
using namespace ymp;
using wtype = u64_t;

//  log(2^64)
const double LOG_WORD = 44.361419555836499802702855773323300356832008599056;

////////////////////////////////////////////////////////////////////////////////
//  Helpers
upL_t e_Basic_get_terms(upL_t p){
    //  Returns the # of terms needed to reach a precision of p.

    //  The taylor series converges to log(x!) / log(10) decimal digits after
    //  x terms. So to find the number of terms needed to reach a precision of p
    //  we need to solve this equation for x:
    //      p = log(x!) / log(2^64)

    //  This function solves this equation via binary search.

    double sizeL = (double)p * LOG_WORD + 1;

    size_t a = 0;
    size_t b = 1;

    //  Double up
    while (logf_approx((double)b) < sizeL)
        b <<= 1;

    //  Binary search
    while (b - a > 1){
        size_t m = (a + b) >> 1;

        if (logf_approx((double)m) < sizeL)
            a = m;
        else
            b = m;
    }

    return b + 2;
}

////////////////////////////////////////////////////////////////////////////////
//  e Implementation
void e_Basic_BSR(BigIntO<wtype>& P, BigIntO<wtype>& Q, upL_t a, upL_t b){
    //  Binary Splitting recursion for exp(1).
    if (b - a == 1){
        P = BigIntO<wtype>(1);
        Q = BigIntO<wtype>(b);
        return;
    }

    upL_t m = (a + b) / 2;

    BigIntO<wtype> P0, Q0, P1, Q1;
    e_Basic_BSR(P0, Q0, a, m);
    e_Basic_BSR(P1, Q1, m, b);

    P = P0 * Q1 + P1;
    Q = Q0 * Q1;
}

class e_Basic_Session : public ComputeFloatSession<wtype>{
public:
    e_Basic_Session(){
        m_name_short = "e";
        m_algorithm_short = "exp(1)";
        m_algorithm_long = "Taylor Series of exp(1) - (Single-Threaded)";

        //  Disable multi-threading for this implementation.
        m_over_decompose = 0;
    }
    virtual BigFloatO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();

        upL_t p = m_precision;
        upL_t terms = e_Basic_get_terms(p);

        Console::print("Summing Series...  ");
        Console::print_commas(terms);
        Console::println(" terms");
        BigIntO<wtype> P, Q;
        e_Basic_BSR(P, Q, 0, terms);

        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time1 - time0, 'T');

        Console::println("Division...");
        BigFloatO<wtype> T = div(BigFloatO<wtype>(P), BigFloatO<wtype>(Q), p);
        T = add(T, BigFloatO<wtype>(1), p);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time2 - time1, 'T');

        return T;
    }
};

void e_Basic(){
    e_Basic_Session().run();
}

}
