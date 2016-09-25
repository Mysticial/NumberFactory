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

    m_start_time = Time::WallClock::Now();
    m_watch.Start();
    BigFloatO<wtype> x = compute();
    m_watch.Stop();
    Console::println();

    write_digits(x, m_name_short, m_algorithm_short);
    print_stats();
}
template <typename wtype>
void ComputeFloatSession<wtype>::setup(){
    m_dec = Console::scan_label_upL_suffix_range("Decimal Digits: ");
    m_hex = (upL_t)(m_dec * 0.83048202372184058696757985737234754396620784825615) + 1;
    m_precision = get_p<wtype>(m_dec);

    //  Get the parallel task decomposition.
    m_tds = Environment::GetLogicalProcessors();
    m_tds *= m_over_decompose;
    if (m_tds == 0){
        m_tds = 1;
    }

    if (m_name_long.empty()){
        m_name_long = m_name_short;
    }
    if (m_algorithm_long.empty()){
        m_algorithm_long = m_algorithm_short;
    }
}
template <typename wtype>
void ComputeFloatSession<wtype>::print_header() const{
    Console::println();
    Console::println();

    Console::println_labelc("Computing", m_name_long, 'G');
    Console::println_labelc("Algorithm", m_algorithm_long, 'Y');
    Console::println();

    Console::println_labelm_commas(30, "Target Precision (digits):", m_dec, 'G');
    Console::println_labelm_commas(30, "Working Precision (words):", m_precision, 'G');
    Console::print_labelm(30, "Threading Mode:", "");
    Parallelism::framework_print_details(Parallelism::get_global_framework(), m_tds);
//    Console::println_labelm_commas(30, "Parallel Task Decomposition:", tds, 'G');
    Console::println();
}
template <typename wtype>
void ComputeFloatSession<wtype>::ensure_tables(){
    Console::println("Constructing Twiddle Tables...");
    Console::println();
    LookupTables::get_global_table<wtype>(2*m_precision);
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
    upL_t dec_digits = m_dec;
    upL_t hex_digits = m_hex;
    if (x.get_mag() == 1){
        wtype top_word = x[0];
        dec_digits += std::to_string(top_word).size();
        while (top_word != 0){
            top_word >>= 4;
            hex_digits++;
        }
    }

    to_file_dec(x, dec_name, dec_digits, m_tds, &m_watch);
    to_file_hex(x, hex_name, hex_digits);
    Console::println();
}
template <typename wtype>
void ComputeFloatSession<wtype>::print_stats() const{
    Time::WallClock end_time = Time::WallClock::Now();
    Console::print("Compute Time:  "); Time::println_time_smart(m_watch.get_wall_time(), 'G');
    Console::print("Total Time:    "); Time::println_time_smart(end_time - m_start_time, 'G');
    Console::println();

    m_watch.print();
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
