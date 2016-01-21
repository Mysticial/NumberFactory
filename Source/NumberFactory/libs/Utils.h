/* Utils.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/09/2015
 * Last Modified    : 07/11/2015
 * 
 *      All filepaths are assumed to be UTF-8.
 * 
 */

#pragma once
#ifndef _ymp_Utils_H
#define _ymp_Utils_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <vector>
#include "PublicLibs/Types.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/Time/Time.h"
#include "PublicLibs/Time/StopWatch.h"
#include "PublicLibs/FileIO/BasicFile.h"
#include "ymp/BigIntO.h"
#include "ymp/BigFloatO.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Misc.
template <typename wtype>
upL_t get_p(upL_t dec_digits){
    const double LOG10_2 = 3.3219280948873623478703194294893901758648313930246;
    const double RATIO = LOG10_2 / (CHAR_BIT * sizeof(wtype));
    return (upL_t)(dec_digits * RATIO) + 2;
}
inline double logf_approx(double x){
    //  Returns a very good approximation to log(x!).
    //  log(x!) ~ (x + 1/2) * (log(x) - 1) + (log(2*pi) + 1) / 2
    //  This approximation gets better as x is larger.
    if (x <= 1) return 0;
    return (x + .5) * (std::log(x) - 1) + (1.4189385332046727417803297364056176398613974736378);
}
////////////////////////////////////////////////////////////////////////////////
//  Dump a BigInt directly to disk.
template <typename wtype>
void to_file_hex(const BigInt<wtype>& x, const std::string& path){
    Console::println("Writing Hexadecimal Digits...");
    FileIO::BasicFile file(0, path);
    auto str = to_string_hex(x);
    file.write(str);
}
template <typename wtype>
void to_file_dec(const BigInt<wtype>& x, const std::string& path, upL_t tds = 1, Time::StopWatch* watch = nullptr){
    //  The time spent in the base conversion will be added to the specified stopwatch.

    Console::println("Base Converting:");

    if (watch != nullptr)
        watch->Start();

    Time::WallClock time0 = Time::WallClock::Now();
    auto str = to_string_dec(x, tds);
    Time::WallClock time1 = Time::WallClock::Now();
    Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');
    Console::println();

    if (watch != nullptr)
        watch->Stop();

    Console::println("Writing Decimal Digits...");
    FileIO::BasicFile file(0, path);
    file.write(str);
}
////////////////////////////////////////////////////////////////////////////////
//  Dump a BigFloat directly to disk.
template <typename wtype>
void to_file_hex(const BigFloat<wtype>& x, const std::string& path, upL_t digits){
    Console::println("Writing Hexadecimal Digits...");
    FileIO::BasicFile file(0, path);
    auto str = to_string_hex(x, digits);
    file.write(str);
}
template <typename wtype>
void to_file_dec(const BigFloat<wtype>& x, const std::string& path, upL_t digits, upL_t tds = 1, Time::StopWatch* watch = nullptr){
    //  The time spent in the base conversion will be added to the specified stopwatch.

    Console::println("Base Converting:");

    if (watch != nullptr)
        watch->Start();

    Time::WallClock time0 = Time::WallClock::Now();
    auto str = to_string_dec(x, digits, tds);
    Time::WallClock time1 = Time::WallClock::Now();
    Console::print("Time:    "); Time::println_secs_hrs(time1 - time0, 'T');
    Console::println();

    if (watch != nullptr)
        watch->Stop();

    Console::println("Writing Decimal Digits...");
    FileIO::BasicFile file(0, path);
    file.write(str);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
