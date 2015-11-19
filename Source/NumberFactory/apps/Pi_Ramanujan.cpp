/* Pi_Ramanujan.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      An implementation of Pi using Ramanujan's Formula.
 * 
 */

#include "NumberFactory/libs/BSR_Type0.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
typedef u64_t wtype;

class Pi_Ramanujan_BSR final : public BSR_Type0<wtype>{
public:
    Pi_Ramanujan_BSR() : BSR_Type0<wtype>(2) {}

    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R, upL_t b, upL_t p) const override{
        //  P(b - 1, b) = (1103 + 26390 b)(2b-1)(4b-3)(4b-1)
        P = BigFloatO<wtype>(b);
        P *= 26390;
        P = add(P, BigFloatO<wtype>(1103), p);
        P *= 2*b - 1;
        P *= 4*b - 3;
        P *= 4*b - 1;

        //  Q(b - 1, b) = 3073907232 * b^3
        Q = BigFloatO<wtype>(b);
        Q *= b;
        Q *= b;
        Q *= 3073907232;

        //  R(b - 1, b) = (2b-1)(4b-3)(4b-1)
        if (R != nullptr){
            *R = BigFloatO<wtype>(2*b - 1);
            *R *= 4*b - 3;
            *R *= 4*b - 1;
        }
    }
};

class Pi_Ramanujan_Session : public ComputeFloatSession<wtype>{
public:
    Pi_Ramanujan_Session(){
        this->name_short = "Pi";
        this->algorithm_short = "Ramanujan";
        this->algorithm_long = "Ramanujan's Formula";
        this->over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();
        const double LOG_RATIO = 0.037711056563288531764530841789572309442247749126585;   //  log(2)/log(96059301)
        const double TERM_RATIO = LOG_RATIO * CHAR_BIT * sizeof(wtype);
        upL_t terms = (upL_t)((double)p * TERM_RATIO + 1);

        Console::print("Summing Series...  ");
        Console::print_commas(terms);
        Console::println(" terms");
        BigFloatO<wtype> P, Q, tmp;
        Pi_Ramanujan_BSR().run(P, Q, terms, p, tds);

        tmp = Q;
        tmp *= 1103;
        P = add(P, tmp, p);
        Q *= 9801;
        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');

        Console::println("Division...");
        P = div(Q, P, p, tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        Console::println("InvSqrt...");
        Q = invsqrt_uW<wtype>(8, p, tds);
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

void Pi_Ramanujan(){
    Pi_Ramanujan_Session().run();
}

}
