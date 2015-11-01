/* Sqrt.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/10/2015
 * Last Modified    : 07/10/2015
 * 
 */

#include <string>

#include "../libs/ComputeFloatSession.h"

namespace NumberFactory{
using namespace ymp;

typedef u64_t wtype;

class Sqrt_Native_Session : public ComputeFloatSession<wtype>{
    wtype x;

public:
    Sqrt_Native_Session(wtype x)
        : x(x)
    {
        this->name_short = "Sqrt(" + std::to_string(x) + ")";
        this->algorithm_short = "Newton";
        this->algorithm_long = "Newton's Method (native implementation)";
    }
    virtual BigFloatO<wtype> compute() override{
        Console::println("InvSqrt...");
        Time::WallClock time0 = Time::WallClock::Now();

        BigFloatO<wtype> T = invsqrt_uW(x, p, tds);
        T *= x;

        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');

        return T;
    }
};

void Sqrt_Native(){
    wtype x = Console::scan_label_upL_range("x = ", 1);
    Sqrt_Native_Session(x).run();
}

}
