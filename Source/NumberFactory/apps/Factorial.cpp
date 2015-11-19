/* Factorial.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/20/2015
 * Last Modified    : 07/20/2015
 * 
 *      Factorial using Binary Splitting
 * 
 */

#include "PublicLibs/ConsoleIO/Label.h"
#include "NumberFactory/libs/PartialFactorial.h"
#include "NumberFactory/libs/ComputeIntSession.h"

namespace NumberFactory{
using namespace ymp;
typedef u64_t wtype;

class Factorial_Session : public ComputeIntSession<wtype>{
    wtype x;

public:
    Factorial_Session(wtype x)
        : x(x)
    {
        this->name_short = std::to_string(x) + "!";
        this->algorithm_long = "Binary Splitting";

        //  Estimate size and ensure table accordingly.
        const double RATIO = 1.4426950408889634073599246810018921374266459541530 / WordTraits<wtype>::BITS; //  1/Log(2)
        double size_words = logf_approx((double)x) * RATIO;
        this->table_cwordlen = ((uiL_t)size_words + 2) * 2;

        //  Over-decompose
        this->over_decompose = 2;
    }
    virtual BigIntO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();

        Console::println("Computing...");
        BigIntO<wtype> T = PartialFactorial<wtype>(0, x, tds);

        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');

        return T;
    }
};

void Factorial(){
    wtype x = (wtype)Console::scan_label_uiL_range("x = ", 1, (wtype)0 - 1);
    Factorial_Session(x).run();
}

}
