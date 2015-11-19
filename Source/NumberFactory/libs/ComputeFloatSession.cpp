/* ComputeFloatSession.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/12/2015
 * Last Modified    : 07/12/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/StringTools/ToString.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Environment/Environment.h"
#include "ymp/Parallelism.h"
#include "Utils.h"
#include "ComputeFloatSession.h"
namespace NumberFactory{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Template Instantiations
template class ComputeFloatSession<u32_t>;
template class ComputeFloatSession<u64_t>;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype>
void ComputeFloatSession<wtype>::run(){
    setup();
    print_header();
    ensure_tables();

    start_time = Time::WallClock::Now();
    watch.Start();
    BigFloatO<wtype> x = compute();
    watch.Stop();
    Console::println();

    write_digits(x, name_short, algorithm_short);
    print_stats();
}
template <typename wtype>
void ComputeFloatSession<wtype>::setup(){
    dec = Console::scan_label_upL_range("Decimal Digits: ");
    hex = (upL_t)(dec * 0.83048202372184058696757985737234754396620784825615) + 1;
    p = get_p<wtype>(dec);

    //  Get the parallel task decomposition.
    tds = Environment::GetLogicalProcessors();
    tds *= over_decompose;
    if (tds == 0)
        tds = 1;

    if (name_long.empty())
        name_long = name_short;
    if (algorithm_long.empty())
        algorithm_long = algorithm_short;
}
template <typename wtype>
void ComputeFloatSession<wtype>::print_header() const{
    Console::println();
    Console::println();

    Console::println_labelc("Computing", name_long, 'G');
    Console::println_labelc("Algorithm", algorithm_long, 'Y');
    Console::println();

    Console::println_labelm_commas(30, "Target Precision (digits):", dec, 'G');
    Console::println_labelm_commas(30, "Working Precision (words):", p, 'G');
    Console::print_labelm(30, "Threading Mode:", "");
    Threads::PrintFrameworkDetails(Threads::GetParallelFramework(), tds);
//    Console::println_labelm_commas(30, "Parallel Task Decomposition:", tds, 'G');
    Console::println();
}
template <typename wtype>
void ComputeFloatSession<wtype>::ensure_tables(){
    Console::println("Constructing Twiddle Tables...");
    Console::println();
    ensure_global_table<wtype>(2*p);
}
template <typename wtype>
void ComputeFloatSession<wtype>::write_digits(const BigFloat<wtype>& x, const std::string& name, const std::string& algorithm){
    std::string dec_name, hex_name;
    if (algorithm.empty()){
        dec_name = name + " - Dec.txt";
        hex_name = name + " - Hex.txt";
    }else{
        dec_name = name + " - Dec - " + algorithm + ".txt";
        hex_name = name + " - Hex - " + algorithm + ".txt";
    }

    //  Special Case: If the number is close to one, don't count the digits in
    //  front of the decimal place. This keeps the behavior consistent with
    //  y-cruncher and other Pi programs that start counting digits after the
    //  decimal place.
    upL_t dec_digits = dec;
    upL_t hex_digits = hex;
    if (x.get_mag() == 1){
        wtype top_word = x[0];
        dec_digits += std::to_string(top_word).size();
        while (top_word != 0){
            top_word >>= 4;
            hex_digits++;
        }
    }

    to_file_dec(x, StringTools::a_to_w_direct(dec_name).c_str(), dec_digits, tds, &watch);
    to_file_hex(x, StringTools::a_to_w_direct(hex_name).c_str(), hex_digits);
    Console::println();
}
template <typename wtype>
void ComputeFloatSession<wtype>::print_stats() const{
    Time::WallClock end_time = Time::WallClock::Now();
    Console::print("Compute Time:  "); Time::println_secs_hrs(watch.GetWallTime(), 'G');
    Console::print("Total Time:    "); Time::println_secs_hrs(end_time - start_time, 'G');
    Console::println();

    watch.PrintCpuUtilization();
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
