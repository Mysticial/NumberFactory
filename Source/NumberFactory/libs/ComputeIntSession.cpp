/* ComputeIntSession.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/20/2015
 * Last Modified    : 07/20/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Environment/Environment.h"
#include "ymp/Parallelism.h"
#include "Utils.h"
#include "ComputeIntSession.h"
namespace NumberFactory{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Template Instantiations
template class ComputeIntSession<u32_t>;
template class ComputeIntSession<u64_t>;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype>
void ComputeIntSession<wtype>::run(){
    setup();
    print_header();
    ensure_tables();

    start_time = Time::WallClock::Now();
    watch.Start();
    BigIntO<wtype> x = compute();
    watch.Stop();
    Console::println();

    write_digits(x, name_short, algorithm_short);
    print_stats();
}
template <typename wtype>
void ComputeIntSession<wtype>::setup(){
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
void ComputeIntSession<wtype>::print_header() const{
    Console::println();
    Console::println();

    Console::println_labelc("Computing", name_long, 'G');
    Console::println_labelc("Algorithm", algorithm_long, 'Y');
    Console::println();

    Console::print_labelm(16, "Threading Mode:", "");
    Parallelism::framework_print_details(Parallelism::get_global_framework(), tds);
//    Console::println_labelm_commas(30, "Parallel Task Decomposition:", tds, 'G');
    Console::println();
}
template <typename wtype>
void ComputeIntSession<wtype>::ensure_tables(){
    Console::println("Constructing Twiddle Tables...");
    Console::println();
    LookupTables::get_global_table<wtype>(table_cwordlen);
}
template <typename wtype>
void ComputeIntSession<wtype>::write_digits(const BigInt<wtype>& x, const std::string& name, const std::string& algorithm){
    std::string dec_name, hex_name;
    if (algorithm.empty()){
        dec_name = name + " - Dec.txt";
        hex_name = name + " - Hex.txt";
    }else{
        dec_name = name + " - Dec - " + algorithm + ".txt";
        hex_name = name + " - Hex - " + algorithm + ".txt";
    }


    to_file_dec(x, dec_name, tds, &watch);
    to_file_hex(x, hex_name);
    Console::println();
}
template <typename wtype>
void ComputeIntSession<wtype>::print_stats() const{
    Time::WallClock end_time = Time::WallClock::Now();
    Console::print("Compute Time:  "); Time::println_secs_hrs(watch.get_wall_time(), 'G');
    Console::print("Total Time:    "); Time::println_secs_hrs(end_time - start_time, 'G');
    Console::println();

    watch.print();
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
