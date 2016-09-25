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
using wtype = u64_t;

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
    Console::print("Time:    "); Time::println_time_smart(time1 - time0, 'T');
}

class Pi_AGM_Session : public ComputeFloatSession<wtype>{
public:
    Pi_AGM_Session(){
        m_name_short = "Pi";
        m_algorithm_short = "AGM";
        m_algorithm_long = "Brent-Salamin AGM";
    }
    virtual BigFloatO<wtype> compute() override{
        upL_t p = m_precision;

        BigFloatO<wtype> P, Q;
        Pi_AGM(P, Q, p, m_tds);
        Time::WallClock time1 = Time::WallClock::Now();

        Console::println("Finishing...");
        P = sqr(P, p, m_tds);
        P = div(P, Q, p, m_tds);
        Q.clear();
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time2 - time1, 'T');

        return P;
    }
};
class Lemniscate_AGM_Session : public ComputeFloatSession<wtype>{
public:
    Lemniscate_AGM_Session(){
        m_name_short = "Lemniscate";
        m_algorithm_short = "AGM";
        m_algorithm_long = "Brent-Salamin AGM";
    }
    virtual BigFloatO<wtype> compute() override{
        upL_t p = m_precision;

        BigFloatO<wtype> P, Q;
        Pi_AGM(P, Q, p, m_tds);
        Time::WallClock time1 = Time::WallClock::Now();

        Console::println("Finishing...");
        P = div(P, Q, p, m_tds);
        Q.clear();
        P <<= 2;
        P = mul(P, invsqrt_uW<wtype>(2, p, m_tds), p, m_tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time2 - time1, 'T');

        return P;
    }
};
class Gamma1d4_AGM_Session : public ComputeFloatSession<wtype>{
public:
    Gamma1d4_AGM_Session(){
        m_name_short = "Gamma(0.25)";
        m_name_long = "Gamma(1/4)";
        m_algorithm_short = "AGM";
        m_algorithm_long = "Brent-Salamin AGM";
    }
    virtual BigFloatO<wtype> compute() override{
        upL_t p = m_precision;

        BigFloatO<wtype> P, Q;
        Pi_AGM(P, Q, p, m_tds);
        Time::WallClock time1 = Time::WallClock::Now();

        Console::println("Finishing...");
        P = div(P, Q, p, m_tds);
        Q <<= 4;
        Q = sqrt(Q, p, m_tds);
        Q = sqrt(Q, p, m_tds);
        P = mul(P, Q, p, m_tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time2 - time1, 'T');

        return P;
    }
};

class AGM_TriCompute_Session : public ComputeFloatSession<wtype>{
    BigFloatO<wtype> Pi, Lemniscate, Gamma;

public:
    AGM_TriCompute_Session(){
        m_name_long = "Pi, Lemniscate, and Gamma(1/4)";
        m_algorithm_short = "AGM";
        m_algorithm_long = "Brent-Salamin AGM";
    }
    virtual BigFloatO<wtype> compute() override{
        upL_t p = m_precision;

        BigFloatO<wtype> P, Q;
        Pi_AGM(P, Q, p, m_tds);
        Time::WallClock time1 = Time::WallClock::Now();

        Console::println("Finishing...");
        BigFloatO<wtype> T;
        T = div(P, Q, p, m_tds);

        //  Pi
        Pi = mul(T, P, p, m_tds);

        //  Lemniscate
        Q <<= 4;
        Q = sqrt(Q, p, m_tds);
        Q = sqrt(Q, p, m_tds);
        Gamma = mul(T, Q, p, m_tds);

        //  Gamma(1/4)
        T <<= 2;
        Lemniscate = mul(T, invsqrt_uW<wtype>(2, p, m_tds), p, m_tds);
        Time::WallClock time2 = Time::WallClock::Now();
        Console::print("Time:    "); Time::println_time_smart(time2 - time1, 'T');

        return BigFloatO<wtype>();
    }
    virtual void run() override{
        setup();
        print_header();
        ensure_tables();

        m_watch.Start();
        m_start_time = Time::WallClock::Now();
        compute();
        m_watch.Stop();
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
