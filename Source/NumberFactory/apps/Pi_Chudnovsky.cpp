/* Pi_Chudnovsky.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/10/2015
 * Last Modified    : 07/10/2015
 * 
 *      An implementation of Pi using the Chudnovsky Formula.
 * 
 */

#include "NumberFactory/libs/BSR_Type0.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
typedef u64_t wtype;

class Pi_Chudnovsky_BSR final : public BSR_Type0<wtype>{
public:
    Pi_Chudnovsky_BSR() : BSR_Type0<wtype>(2) {}

    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R, upL_t b, upL_t p) const override{
        //  P = (13591409 + 545140134 b)(2b-1)(6b-5)(6b-1) (-1)^b
        P = BigFloatO<wtype>(b);
        P *= 545140134;
        P = add(P, BigFloatO<wtype>(13591409), p);
        P *= 2*b - 1;
        P *= 6*b - 5;
        P *= 6*b - 1;
        if (b % 2 == 1)
            P.negate();

        //  Q = 10939058860032000 * b^3
        Q = BigFloatO<wtype>(b);
        Q *= b;
        Q *= b;
        Q *= 10939058860032000;

        //  R = (2b-1)(6b-5)(6b-1)
        if (R != nullptr){
            *R = BigFloatO<wtype>(2*b - 1);
            *R *= 6*b - 5;
            *R *= 6*b - 1;
        }
    }
};

class Pi_Chudnovsky_Session : public ComputeFloatSession<wtype>{
public:
    Pi_Chudnovsky_Session(){
        this->name_short = "Pi";
        this->algorithm_short = "Chudnovsky";
        this->algorithm_long = "Chudnovsky Formula";
        this->over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();
        const double LOG_RATIO = 0.021226729578153553821919551585989682400156215921462;   //  log(2)/log(151931373056000)
        const double TERM_RATIO = LOG_RATIO * CHAR_BIT * sizeof(wtype);
        upL_t terms = (upL_t)((double)p * TERM_RATIO + 1);

        Console::print("Summing Series...  ");
        Console::print_commas(terms);
        Console::println(" terms");
        BigFloatO<wtype> P, Q, tmp;
        Pi_Chudnovsky_BSR().run(P, Q, terms, p, tds);

        tmp = Q;
        tmp *= 13591409;
        P = add(P, tmp, p);
        Q *= 4270934400;
        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');

        Console::println("Division...");
        P = div(Q, P, p, tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        Console::println("InvSqrt...");
        Q = invsqrt_uW<wtype>(10005, p, tds);
        Time::WallClock time3 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time3 - time2, 'T');

        Console::println("Final Multiply...");
        P = mul(P, Q, p, tds);
        Q.clear();
        Time::WallClock time4 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time4 - time3, 'T');

        return P;
    }
};

void Pi_Chudnovsky(){
    Pi_Chudnovsky_Session().run();
}

}
