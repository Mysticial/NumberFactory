/* BinomialCoefficient.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/09/2015
 * Last Modified    : 08/09/2015
 * 
 *      Binomial Coefficient
 * 
 */

#include "PublicLibs/ConsoleIO/Label.h"
#include "NumberFactory/libs/PartialFactorial.h"
#include "NumberFactory/libs/ComputeIntSession.h"

namespace NumberFactory{
using namespace ymp;
using wtype = u64_t;

class Binomial_Session : public ComputeIntSession<wtype>{
    wtype m_n;
    wtype m_k;
    wtype m_c;

public:
    Binomial_Session(wtype n, wtype k)
        : m_n(n)
        , m_k(k)
        , m_c(n - k)
    {
        this->name_short = "Binomial(" + std::to_string(n) + ", " + std::to_string(k) + ")";
        this->algorithm_long = "Binary Splitting";

        //  Optimization: Flip (c) and (n - c) if needed.
        if (k <= n && k < m_c){
            m_k = n - k;
            m_c = k;
        }

        //  Estimate size and ensure table accordingly.
        const double RATIO = 1.4426950408889634073599246810018921374266459541530 / WordTraits<wtype>::BITS; //  1/Log(2)
        double size = logf_approx((double)n) - logf_approx((double)k) * RATIO;
        this->table_cwordlen = ((uiL_t)size + 2) * 2;

        //  Over-decompose
        this->over_decompose = 2;
    }
    virtual BigIntO<wtype> compute() override{
        if (m_k > m_n){
            return BigIntO<wtype>();
        }

        Time::WallClock time0 = Time::WallClock::Now();

        Console::println("Computing: " + std::to_string(m_n) + "! / " + std::to_string(m_k) + "!");
        BigIntO<wtype> T = PartialFactorial<wtype>(m_k, m_n, tds);
        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time1 - time0, 'T');

        Console::println("Computing: " + std::to_string(m_c) + "!");
        BigIntO<wtype> N = PartialFactorial<wtype>(0, m_c, tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time2 - time1, 'T');

        Console::println("Division...");
        T = div(T, N, tds);
        Time::WallClock time3 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time3 - time2, 'T');

        return T;
    }
};

void Binomial(){
    wtype n = (wtype)Console::scan_label_uiL_range("n = ", 1, (wtype)0 - 1);
    wtype k = (wtype)Console::scan_label_uiL_range("k = ", 0, n);
    Binomial_Session(n, k).run();
}


}
