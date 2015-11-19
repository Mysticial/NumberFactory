/* Zeta(3)_AZ2.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/10/2015
 * Last Modified    : 07/10/2015
 * 
 *                 1   inf (         P(k) ((2k+1)!(2k)! k!)^3  )
 *      Zeta(3) = ---- SUM ( (-1)^k -------------------------- )
 *                 24  k=0 (            (3k+2)! ((4k+3)!)^5    )
 * 
 *      P(k) = 126392*k^5 + 412708*k^4 + 531578*k^3 + 336367*k^2 + 104000*k + 12463
 * 
 */

#include "NumberFactory/libs/BSR_Type0.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
typedef u64_t wtype;

class Zeta3_AZ2_BSR final : public BSR_Type0<wtype>{
public:
    Zeta3_AZ2_BSR() : BSR_Type0<wtype>(4) {}

    virtual void BSR_End(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, BigFloatO<wtype>* R, upL_t b, upL_t p) const override{
        BigFloatO<wtype> tmp;
        if (R == nullptr){
            R = &tmp;
        }

        //  R(b - 1, b) = b^5 (2b - 1)^3
        *R = BigFloatO<wtype>(b);
        *R *= b;
        *R *= b;
        *R *= b;
        *R *= b;
        *R *= 2*b - 1;
        *R *= 2*b - 1;
        *R *= 2*b - 1;

        //  P(b - 1, b) = (126392*b^5 + 412708*b^4 + 531578*b^3 + 336367*b^2 + 104000*b + 12463) R(a,b) (-1)^b
        P = BigFloatO<wtype>(126392);
        P *= b; P = add(P, BigFloatO<wtype>(412708), p);
        P *= b; P = add(P, BigFloatO<wtype>(531578), p);
        P *= b; P = add(P, BigFloatO<wtype>(336367), p);
        P *= b; P = add(P, BigFloatO<wtype>(104000), p);
        P *= b; P = add(P, BigFloatO<wtype>(12463), p);
        P = mul(P, *R, p);
        if (b % 2 == 1)
            P.negate();

        //  Q(b - 1, b) = 24 (3b + 1) (3b + 2) (4b + 1)^3 (4b + 3)^3
        Q = BigFloatO<wtype>(24);
        Q *= 3*b + 1;
        Q *= 3*b + 2;
        Q *= 4*b + 1;
        Q *= 4*b + 1;
        Q *= 4*b + 1;
        Q *= 4*b + 3;
        Q *= 4*b + 3;
        Q *= 4*b + 3;
    }
};

class Zeta3_AZ2_Session : public ComputeFloatSession<wtype>{
public:
    Zeta3_AZ2_Session(){
        this->name_short = "Zeta(3)";
        this->name_long = "Zeta(3) - Apery's Constant";
        this->algorithm_short = "AZ2";
        this->algorithm_long = "Amdeberhan-Zeilberger Formula 2";
        this->over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();
        const double LOG_RATIO = 0.059684077250347122669110675540411694499888089577970; //  log(2)/log(110592)
        const double TERM_RATIO = LOG_RATIO * CHAR_BIT * sizeof(wtype);
        upL_t terms = (upL_t)((double)p * TERM_RATIO + 1);

        Console::print("Summing Series...  ");
        Console::print_commas(terms);
        Console::println(" terms");
        BigFloatO<wtype> P, Q, tmp;
        Zeta3_AZ2_BSR().run(P, Q, terms, p, tds);

        tmp = Q;
        tmp *= 12463;
        P = add(P, tmp, p);
        Q *= 10368;
        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');

        Console::println("Division...");
        P = div(P, Q, p, tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        return P;
    }
};

void Zeta3_AZ2(){
    Zeta3_AZ2_Session().run();
}

}
