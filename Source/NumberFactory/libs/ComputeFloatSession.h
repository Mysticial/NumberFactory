/* ComputeFloatSession.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/12/2015
 * Last Modified    : 07/12/2015
 * 
 *      A helper class for computing large floats.
 * 
 *  This class centralizes all the boilerplate code for computing floats.
 * 
 */

#pragma once
#ifndef NumberFactory_ComputeFloatSession_H
#define NumberFactory_ComputeFloatSession_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/Types.h"
#include "PublicLibs/Time/Time.h"
#include "PublicLibs/Time/StopWatch.h"
#include "ymp/BigFloatO.h"
namespace NumberFactory{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype>
class ComputeFloatSession{
protected:
    std::string name_short = "Output";
    std::string name_long;
    std::string algorithm_short;
    std::string algorithm_long;

    //  Set the task decomposition to this times the # of logical cores.
    //   -  Setting to zero disables multi-threading.
    //   -  Setting to greater than 1 implies over-decomposition. This may be
    //      useful for algorithms that suffer from load-imbalance.
    ukL_t over_decompose = 1;

    upL_t dec;
    upL_t hex;
    upL_t p;
    upL_t tds;

    Time::StopWatch watch;
    Time::WallClock start_time;

public:
    virtual BigFloatO<wtype> compute() = 0;
    virtual void run();

protected:
    void setup();
    void print_header() const;
    void ensure_tables();
    void write_digits(const BigFloat<wtype>& x, const std::string& name, const std::string& algorithm);
    void print_stats() const;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
