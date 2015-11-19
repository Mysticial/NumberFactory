/* Pi_AGM.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/08/2015
 * Last Modified    : 07/11/2015
 * 
 * 
 *  The Brent-Salamin AGM computes two numbers P and Q:
 *      P = 1.694426169...
 *      Q = 0.913893162...
 * 
 *  With these you can compute:
 *      Pi          =   P^2 / Q                     =   3.1415926535...
 *      Lemniscate  =   (4 * P) / (Q * sqrt(2))     =   5.2441151085...
 *      Gamma(1/4)  =   P/Q * (16*Q)^(1/4)          =   3.6256099082...
 * 
 */

#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
using namespace ymp;
typedef u64_t wtype;

template <typename wtype>
void Pi_AGM(BigFloatO<wtype>& P, BigFloatO<wtype>& Q, upL_t p, upL_t tds){
    //  Runs the Brent-Salamin AGM which computes:
    //      P = 1.694426169...
    //      Q = 0.913893162...

    Console::println("Computing AGM...");
    Time::WallClock time0 = Time::WallClock::Now();

    BigFloatO<wtype>& a = P;
    a = BigFloatO<wtype>(1);

    BigFloatO<wtype> b = invsqrt_uW<wtype>(2, p, tds);

    BigFloatO<wtype>& t = Q;
    t = BigFloatO<wtype>(1);

    siL_t error = 0;
    siL_t stop_error = -(siL_t)p / 2;

    int c = 0;
    while (error >= stop_error){
        Console::print("Iteration");
        Console::print_marginr(4, c, 'G');
        Console::print(" :  ", 'w');

        BigFloatO<wtype> m = mul(a, b, p, tds);
        BigFloatO<wtype> s = sub(a, b, p);

        //  a' = (a + b) / 2
        a = add(a, b, p);
        a <<= -1;
        b.clear();

        //  b' = sqrt(a * b)
        b = sqrt(m, p, tds);
        m.clear();

        //  t' = t - 2^(c) (a - b)^2
        s = sqr(s, p, tds);
        s <<= c;
        t = sub(t, s, p);

        error = s.get_mag();
        Console::println_labelc_commas("Error Magnitude", error, 'Y');

        c++;
    }

    a = add(a, b, p);

    Time::WallClock time1 = Time::WallClock::Now();
    Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');
}

class Pi_AGM_Session : public ComputeFloatSession<wtype>{
public:
    Pi_AGM_Session(){
        this->name_short = "Pi";
        this->algorithm_short = "AGM";
        this->algorithm_long = "Brent-Salamin AGM";
    }
    virtual BigFloatO<wtype> compute() override{
        BigFloatO<wtype> P, Q;
        Pi_AGM(P, Q, p, tds);
        Time::WallClock time1 = Time::WallClock::Now();

        Console::println("Finishing...");
        P = sqr(P, p, tds);
        P = div(P, Q, p, tds);
        Q.clear();
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        return P;
    }
};
class Lemniscate_AGM_Session : public ComputeFloatSession<wtype>{
public:
    Lemniscate_AGM_Session(){
        this->name_short = "Lemniscate";
        this->algorithm_short = "AGM";
        this->algorithm_long = "Brent-Salamin AGM";
    }
    virtual BigFloatO<wtype> compute() override{
        BigFloatO<wtype> P, Q;
        Pi_AGM(P, Q, p, tds);
        Time::WallClock time1 = Time::WallClock::Now();

        Console::println("Finishing...");
        P = div(P, Q, p, tds);
        Q.clear();
        P <<= 2;
        P = mul(P, invsqrt_uW<wtype>(2, p, tds), p, tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        return P;
    }
};
class Gamma1d4_AGM_Session : public ComputeFloatSession<wtype>{
public:
    Gamma1d4_AGM_Session(){
        this->name_short = "Gamma(0.25)";
        this->name_long = "Gamma(1/4)";
        this->algorithm_short = "AGM";
        this->algorithm_long = "Brent-Salamin AGM";
    }
    virtual BigFloatO<wtype> compute() override{
        BigFloatO<wtype> P, Q;
        Pi_AGM(P, Q, p, tds);
        Time::WallClock time1 = Time::WallClock::Now();

        Console::println("Finishing...");
        P = div(P, Q, p, tds);
        Q <<= 4;
        Q = sqrt(Q, p, tds);
        Q = sqrt(Q, p, tds);
        P = mul(P, Q, p, tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        return P;
    }
};

class AGM_TriCompute_Session : public ComputeFloatSession<wtype>{
    BigFloatO<wtype> Pi, Lemniscate, Gamma;

public:
    AGM_TriCompute_Session(){
        this->name_long = "Pi, Lemniscate, and Gamma(1/4)";
        this->algorithm_short = "AGM";
        this->algorithm_long = "Brent-Salamin AGM";
    }
    virtual BigFloatO<wtype> compute() override{
        BigFloatO<wtype> P, Q;
        Pi_AGM(P, Q, p, tds);
        Time::WallClock time1 = Time::WallClock::Now();

        Console::println("Finishing...");
        BigFloatO<wtype> T;
        T = div(P, Q, p, tds);

        //  Pi
        Pi = mul(T, P, p, tds);

        //  Lemniscate
        Q <<= 4;
        Q = sqrt(Q, p, tds);
        Q = sqrt(Q, p, tds);
        Gamma = mul(T, Q, p, tds);

        //  Gamma(1/4)
        T <<= 2;
        Lemniscate = mul(T, invsqrt_uW<wtype>(2, p, tds), p, tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_secs_hrs(time2 - time1, 'T');

        return BigFloatO<wtype>();
    }
    virtual void run() override{
        setup();
        print_header();
        ensure_tables();

        watch.Start();
        start_time = Time::WallClock::Now();
        compute();
        watch.Stop();
        Console::println();

        Console::println("Outputing Pi...");
        write_digits(Pi, "Pi", "AGM");

        Console::println("Outputing Lemniscate...");
        write_digits(Lemniscate, "Lemniscate", "AGM");

        Console::println("Outputing Gamma(1/4)...");
        write_digits(Gamma, "Gamma(0.25)", "AGM");

        print_stats();
    }
};

void Pi_AGM(){
    Pi_AGM_Session().run();
}
void Lemniscate_AGM(){
    Lemniscate_AGM_Session().run();
}
void Gamma1d4_AGM(){
    Gamma1d4_AGM_Session().run();
}
void AGM_TriCompute(){
    AGM_TriCompute_Session().run();
}

}
