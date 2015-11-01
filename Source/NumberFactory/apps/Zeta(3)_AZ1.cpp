/* Zeta(3)_AZ1.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/10/2015
 * Last Modified    : 07/10/2015
 * 
 *                 1   inf (         (205k^2 + 250k + 77)k!^10  )
 *      Zeta(3) = ---- SUM ( (-1)^k --------------------------- )
 *                 24  k=0 (                 (2k+1)!^5          )
 * 
 */

#include "../libs/BSR_Type0.h"
#include "../libs/ComputeFloatSession.h"

namespace NumberFactory{
typedef u64_t wtype;

class Zeta3_AZ1_BSR final : public BSR_Type0<wtype>{
public:
    Zeta3_AZ1_BSR() : BSR_Type0<wtype>(4) {}

    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R, upL_t b, upL_t p) const override{
        BigFloatO<wtype> tmp;
        if (R == nullptr){
            R = &tmp;
        }

        //  R(b - 1, b) = b^5
        *R = BigFloatO<wtype>(b);
        *R *= b;
        *R *= b;
        *R *= b;
        *R *= b;

        //  P(b - 1, b) = (205 b^2 + 250 b + 77) R(a,b)
        P = BigFloatO<wtype>(205);
        P *= b;
        P = add(P, BigFloatO<wtype>(250), p);
        P *= b;
        P = add(P, BigFloatO<wtype>(77), p);
        P = mul(P, *R, p);
        if (b % 2 == 1)
            P.negate();

        //  Q(b - 1, b) = (4b + 2)^5
        Q = BigFloatO<wtype>(4*b + 2);
        Q *= 4*b + 2;
        Q *= 4*b + 2;
        Q *= 4*b + 2;
        Q *= 4*b + 2;
    }
};

class Zeta3_AZ1_Session : public ComputeFloatSession<wtype>{
public:
    Zeta3_AZ1_Session(){
        this->name_short = "Zeta(3)";
        this->name_long = "Zeta(3) - Apery's Constant";
        this->algorithm_short = "AZ1";
        this->algorithm_long = "Amdeberhan-Zeilberger Formula 1";
        this->over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();
        const double LOG_RATIO = 0.10000000000000000000000000000000000000000000000000;  //  log(2)/log(1024)
        const double TERM_RATIO = LOG_RATIO * CHAR_BIT * sizeof(wtype);
        upL_t terms = (upL_t)((double)p * TERM_RATIO + 1);

        Console::print("Summing Series...  ");
        Console::print_commas(terms);
        Console::println(" terms");
        BigFloatO<wtype> P, Q, tmp;
        Zeta3_AZ1_BSR().run(P, Q, terms, p, tds);

        tmp = Q;
        tmp *= 77;
        P = add(P, tmp, p);
        Q *= 64;
        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');

        Console::println("Division...");
        P = div(P, Q, p, tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        return P;
    }
};

void Zeta3_AZ1(){
    Zeta3_AZ1_Session().run();
}

}
