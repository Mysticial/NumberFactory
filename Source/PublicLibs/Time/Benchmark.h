/* Benchmark.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/16/2014
 * Last Modified    : 09/16/2014
 * 
 *      A helper class for performing iteration-based benchmarks.
 * 
 */

#pragma once
#ifndef ymp_Time_Benchmark_H
#define ymp_Time_Benchmark_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "Time.h"
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class IterationBenchmark{
    const char* units;
    uiL_t iterations;
    double seconds;
    WallClock start;

public:
    IterationBenchmark(const char* units, double seconds = 4.0)
        : units(units)
        , iterations(0)
        , seconds(seconds)
        , start(WallClock::Now())
    {}
    IterationBenchmark(double seconds = 4.0)
        : units(nullptr)
        , iterations(0)
        , seconds(seconds)
        , start(WallClock::Now())
    {}
    ~IterationBenchmark(){
        if (units != nullptr)
            Console::println_unitl_float(units, GetThroughput());
    }

public:
    void operator++(int){
        iterations++;
    }
    void operator+=(uiL_t new_iterations){
        iterations += new_iterations;
    }
    bool ShouldContinue() const{
        auto elapsed = start.SecondsElapsed();
        return elapsed < seconds;
    }

public:
    double GetElapsed() const{
        return start.SecondsElapsed();
    }
    uiL_t GetIterations() const{
        return iterations;
    }
    double GetLatency() const{
        return start.SecondsElapsed() / iterations;
    }
    double GetThroughput() const{
        return iterations / start.SecondsElapsed();
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
