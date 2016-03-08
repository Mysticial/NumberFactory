/* e_Parallel.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 06/20/2015
 * Last Modified    : 06/20/2015
 * 
 *      An parallelized implementation of exp(1) and 1/exp(-1) for e.
 * 
 */

#include "ymp/Parallelism.h"
#include "NumberFactory/libs/Utils.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
using namespace ymp;
typedef u64_t wtype;

//  log(2^64)
const double LOG_WORD = 44.361419555836499802702855773323300356832008599056;

////////////////////////////////////////////////////////////////////////////////
//  Helpers
upL_t e_Taylor_get_terms(upL_t p){
    //  Returns the # of terms needed to reach a precision of p.

    //  The taylor series converges to log(x!) / log(10) decimal digits after
    //  x terms. So to find the number of terms needed to reach a precision of p
    //  we need to solve this question for x:
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
template <bool alternate>
void e_Taylor_BSR(BigIntO<wtype>& P, BigIntO<wtype>& Q, upL_t a, upL_t b, upL_t tds);

template <bool alternate>
class e_Taylor_BSR_Action : public BasicAction{
    BigIntO<wtype>& P;
    BigIntO<wtype>& Q;
    upL_t a;
    upL_t b;
    upL_t p;
    upL_t tds;

public:
    e_Taylor_BSR_Action(BigIntO<wtype>& P, BigIntO<wtype>& Q, upL_t a, upL_t b, upL_t tds)
        : P(P), Q(Q), a(a), b(b), tds(tds)
    {}
    virtual void run() override{
        e_Taylor_BSR<alternate>(P, Q, a, b, tds);
    }
};

template <bool alternate>
void e_Taylor_BSR(BigIntO<wtype>& P, BigIntO<wtype>& Q, upL_t a, upL_t b, upL_t tds){
    //  Binary Splitting recursion for exp(1).
    if (b - a == 1){
        if (alternate){
            P = BigIntO<wtype>(b % 2 == 0 ? 1 : -1);
        }else{
            P = BigIntO<wtype>(1);
        }
        Q = BigIntO<wtype>(b);
        return;
    }

    upL_t m = (a + b) / 2;

    BigIntO<wtype> P0, Q0, P1, Q1;

    if (tds <= 1 || b - a < 1000){
        //  Don't Parallelize
        e_Taylor_BSR<alternate>(P0, Q0, a, m, 1);
        e_Taylor_BSR<alternate>(P1, Q1, m, b, 1);
    }else{
        //  Parallelize
        upL_t tds0 = tds / 2;
        upL_t tds1 = tds - tds0;
        e_Taylor_BSR_Action<alternate> tp0(P0, Q0, a, m, tds0);
        e_Taylor_BSR_Action<alternate> tp1(P1, Q1, m, b, tds1);
        Parallelism::run_in_parallel(tp0, tp1);
    }

    //  P = P0*Q1 + P1
    //  Q = Q0*Q1
    P = mul(P0, Q1, tds);   //  P0*Q1
    P = P + P1;             //  P0*Q1 + P1
    Q = mul(Q0, Q1, tds);   //  Q0*Q1
}

template <bool alternate>
class e_Taylor_Session : public ComputeFloatSession<wtype>{
public:
    e_Taylor_Session(){
        this->name_short = "e";
        if (!alternate){
            this->algorithm_short = "exp(1)";
            this->algorithm_long = "Taylor Series of exp(1)";
        }else{
            this->algorithm_short = "exp(-1)";
            this->algorithm_long = "Taylor Series of exp(-1)";
        }
        this->over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();
        upL_t terms = e_Taylor_get_terms(p);

        Console::print("Summing Series...  ");
        Console::print_commas(terms);
        Console::println(" terms");
        BigIntO<wtype> P, Q;
        e_Taylor_BSR<alternate>(P, Q, 1, terms, tds);
        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');

        Console::println("Division...");
        BigFloatO<wtype> T;
        if (!alternate){
            T = div(BigFloatO<wtype>(P), BigFloatO<wtype>(Q), p, tds);
            T = add(T, BigFloatO<wtype>(2), p);
        }else{
            T = div(BigFloatO<wtype>(Q), BigFloatO<wtype>(P), p, tds);
        }
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        return T;
    }
};

void e_Taylor1(){
    e_Taylor_Session<false>().run();
}
void e_Taylor2(){
    e_Taylor_Session<true>().run();
}

}
