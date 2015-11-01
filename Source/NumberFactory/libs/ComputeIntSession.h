/* ComputeIntSession.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/20/2015
 * Last Modified    : 07/20/2015
 * 
 *      A helper class for computing large integers.
 * 
 *  This class centralizes all the boilerplate code for computing integers.
 * 
 */

#pragma once
#ifndef NumberFactory_ComputeIntSession_H
#define NumberFactory_ComputeIntSession_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "../../PublicLibs/Types.h"
#include "../../PublicLibs/Time/Time.h"
#include "../../PublicLibs/Time/StopWatch.h"
#include "../../ymp/BigIntO.h"
namespace NumberFactory{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype>
class ComputeIntSession{
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

    uiL_t table_cwordlen = (uiL_t)0 - 1;
    upL_t tds;

    Time::StopWatch watch;
    Time::WallClock start_time;

public:
    virtual BigIntO<wtype> compute() = 0;
    virtual void run();

protected:
    virtual void setup();
    void print_header() const;
    void ensure_tables();
    void write_digits(const BigInt<wtype>& x, const std::string& name, const std::string& algorithm);
    void print_stats() const;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
