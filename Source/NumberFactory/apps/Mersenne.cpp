/* Mersenne.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/20/2015
 * Last Modified    : 07/20/2015
 * 
 *      Mersenne Numbers
 * 
 */

#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "NumberFactory/libs/ComputeIntSession.h"

namespace NumberFactory{
using namespace ymp;
using wtype = u64_t;

class MersenneNumber : public ComputeIntSession<wtype>{
    wtype m_pow;

public:
    MersenneNumber(wtype pow)
        : m_pow(pow)
    {
        this->name_short = "M" + std::to_string(pow);
        this->name_long = "Mersenne Number: " + std::string("2^") + std::to_string(pow) + " - 1";
        this->table_cwordlen = (pow / WordTraits<wtype>::BITS) * 2 + 1;
    }
    virtual BigIntO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();

        Console::println("Computing...");
        BigIntO<wtype> x(1);
        x <<= (upL_t)m_pow;
        x -= BigIntO<wtype>(1);

        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time1 - time0, 'T');

        return x;
    }
};

void M57885161(){
    MersenneNumber(57885161).run();
}

}
