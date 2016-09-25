/* NumberFactory.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/10/2015
 * Last Modified    : 07/12/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "ymp/LowLevel_dll.h"
#include "ymp/Functions_dll.h"
namespace NumberFactory{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
    void M57885161();
    void IntegerPower();
    void Factorial();
    void Binomial();
    void Sqrt_Native();
    void e_Basic();
    void e_Taylor1();
    void e_Taylor2();
    void Pi_Machin_Original();
    void Pi_Machin_Takano();
    void Pi_Machin_Stormer();
    void Pi_Machin_Hwang1997();
    void Pi_Chudnovsky();
    void Pi_Ramanujan();
    void Pi_AGM();
    void Lemniscate_AGM();
    void Gamma1d4_AGM();
    void AGM_TriCompute();
    void Log2_Machin_Euler();
    void Log2_Machin_3term();
    void Log2_Machin_4term();
    void Zeta3_AZ1();
    void Zeta3_AZ2();
    void Zeta5_Broadhurst();
    void Zeta5_BroadhurstModified();
    void Zeta5_Huvent();
    void Catalan_Lupas();
    void Catalan_Huvent();
    void Catalan_HuventModified();
    void bench_sequential_multiply_latency();
    void bench_parallel_multiply_latency();
    void bench_parallel_multiply_throughput();
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct Option{
    void (*fp_run)();
    const char* label;
};

std::vector<Option> OPTIONS{
    //  Integer Apps
    {M57885161                  , "Mersenne Prime: M(57,885,161)"},
    {IntegerPower               , "Integer Power x^y"},
    {Factorial                  , "Factorial x!"},
    {Binomial                   , "Binomial Coefficient: n! / (k!(n-k)!)\n"},

    //  Floating-Point Apps
    {Sqrt_Native                , "Square Root of x"},
    {e_Basic                    , "e - Basic (Single-Threaded)"},
    {e_Taylor1                  , "e - exp(1)"},
    {e_Taylor2                  , "e - exp(-1)"},
    {Pi_Machin_Original         , "Pi - Machin's Formula      (2 terms)"},
    {Pi_Machin_Takano           , "Pi - Machin (Takano)       (4 terms)"},
    {Pi_Machin_Stormer          , "Pi - Machin (Stormer)      (4 terms)"},
    {Pi_Machin_Hwang1997        , "Pi - Machin (Hwang-1997)   (6 terms)"},
    {Pi_Chudnovsky              , "Pi - Chudnovsky"},
    {Pi_Ramanujan               , "Pi - Ramanujan"},
    {Pi_AGM                     , "Pi - AGM"},
    {Lemniscate_AGM             , "Lemniscate - AGM"},
    {Gamma1d4_AGM               , "Gamma(1/4) - AGM"},
    {AGM_TriCompute             , "Pi, Lemniscate, and Gamma(1/4)"},
    {Log2_Machin_Euler          , "Log(2) - Machin (Euler)    (2 terms)"},
    {Log2_Machin_3term          , "Log(2) - Machin            (3 terms)"},
    {Log2_Machin_4term          , "Log(2) - Machin            (4 terms)"},
    {Zeta3_AZ1                  , "Zeta(3) - Amdeberhan-Zeilberger 1"},
    {Zeta3_AZ2                  , "Zeta(3) - Amdeberhan-Zeilberger 2"},
    {Zeta5_Broadhurst           , "Zeta(5) - Broadhurst"},
    {Zeta5_BroadhurstModified   , "Zeta(5) - Broadhurst (modified)"},
    {Zeta5_Huvent               , "Zeta(5) - Huvent"},
    {Catalan_Lupas              , "Catalan - Lupas"},
    {Catalan_Huvent             , "Catalan - Huvent"},
    {Catalan_HuventModified     , "Catalan - Huvent (modified)\n"},

    //  Other
    {PrintVersionInfo                   , "Library Version Info"},
    {CompilationOptions                 , "Library Compilation Options"},
    {bench_sequential_multiply_latency  , "Benchmark Sequential Multiply Latency"},
    {bench_parallel_multiply_latency    , "Benchmark Parallel Multiply Latency"},
    {bench_parallel_multiply_throughput , "Benchmark Parallel Multiply Throughput"},
};

void MainMenu(){
    Console::println("Number Factory v1.0.2 (YMP 1.1)", 'G');
    Console::println();
    Console::SetColor('w');

    Console::println("Select an Option:\n");
    upL_t c = 0;
    for (auto& option : OPTIONS){
        Console::print_marginr(3, c);
        Console::print("       ");
        Console::println(option.label);
        c++;
    }
    Console::println();

    int choice = (int)Console::scan_label_upL_range("option: ", 0, c);
    Console::println();

    auto fp = OPTIONS[choice].fp_run;
    if (fp == nullptr){
        Console::Warning("This feature has not been implemented yet.");
        Console::println();
    }else{
        fp();
    }

}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
