/* Sqrt.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/10/2015
 * Last Modified    : 07/10/2015
 * 
 */

#include <string>

#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
using namespace ymp;

using wtype = u64_t;

class Sqrt_Native_Session : public ComputeFloatSession<wtype>{
    wtype m_x;

public:
    Sqrt_Native_Session(wtype x)
        : m_x(x)
    {
        m_name_short = "Sqrt(" + std::to_string(x) + ")";
        m_algorithm_short = "Newton";
        m_algorithm_long = "Newton's Method (native implementation)";
    }
    virtual BigFloatO<wtype> compute() override{
        Console::println("InvSqrt...");
        Time::WallClock time0 = Time::WallClock::Now();

        upL_t p = m_precision;

        BigFloatO<wtype> T = invsqrt_uW(m_x, p, m_tds);
        T *= m_x;

        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time1 - time0, 'T');

        return T;
    }
};

void Sqrt_Native(){
    wtype x = Console::scan_label_upL_range("x = ", 1);
    Sqrt_Native_Session(x).run();
}

}
