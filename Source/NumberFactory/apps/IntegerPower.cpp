/* IntegerPower.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/20/2015
 * Last Modified    : 07/20/2015
 * 
 *      Integer Powering
 * 
 */

#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "NumberFactory/libs/ComputeIntSession.h"

namespace NumberFactory{
using namespace ymp;
typedef u64_t wtype;

template <typename wtype>
BigIntO<wtype> pow(wtype a, wtype b, upL_t tds = 1){
    const ukL_t BITS = WordTraits<wtype>::BITS;
    const wtype HIGH_BIT = (wtype)1 << (BITS - 1);

    if (b == 0)
        return 1;

    ukL_t c = 0;
    while ((b & HIGH_BIT) == 0){
        b <<= 1;
        c++;
    }

    BigIntO<wtype> x(1);

    //  Exponentiation by Squaring
    for (; c < BITS; c++){
        x = sqr(x, tds);

        if (b & HIGH_BIT)
            x *= a;

        b <<= 1;
    }

    return x;
}

class IntegerPower_Session : public ComputeIntSession<wtype>{
    wtype base;
    wtype power;

public:
    IntegerPower_Session(wtype base, wtype power)
        : base(base)
        , power(power)
    {
        this->name_short = std::to_string(base) + "^" + std::to_string(power);
//        this->table_cwordlen = (pow / WordTraits<wtype>::BITS) * 2 + 1;
    }
    virtual BigIntO<wtype> compute() override{
        Time::WallClock time0 = Time::WallClock::Now();

        Console::println("Powering...");
        BigIntO<wtype> x = pow(base, power, tds);

        Time::WallClock time1 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');

        return x;
    }
};

void IntegerPower(){
    wtype x = (wtype)Console::scan_label_uiL_range("x = ", 1, (wtype)0 - 1);
    wtype y = (wtype)Console::scan_label_uiL_range("y = ", 1, (wtype)0 - 1);
    IntegerPower_Session(x, y).run();
}

}
