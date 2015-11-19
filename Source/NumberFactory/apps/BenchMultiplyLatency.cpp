/* BenchMultiplyLatency.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/09/2015
 * Last Modified    : 08/09/2015
 * 
 *      Measure how long it takes to perform a single multiply of size N using
 *  a fixed level of task decomposition.
 * 
 */

#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Time/Benchmark.h"
#include "PublicLibs/Environment/Environment.h"
#include "PublicLibs/AlignedMalloc.h"
#include "ymp/LowLevel.h"

namespace NumberFactory{
using namespace ymp;

template <typename wtype>
bool bench_multiply(upL_t memory_limit, upL_t L, upL_t tds = 1){
    //  Compute memory requirement
    uiL_t ML = LowLevel::mul_Psize<wtype>(2*L, tds);
    uiL_t bytes = 4*L * sizeof(wtype) + ML;
    if (bytes > memory_limit)
        return false;

    //  Allocate Operands
    auto A_uptr = SmartPointer<wtype>::malloc_uptr(L);
    auto B_uptr = SmartPointer<wtype>::malloc_uptr(L);
    auto C_uptr = SmartPointer<wtype>::malloc_uptr(2*L);
    wtype* A = A_uptr.get();
    wtype* B = B_uptr.get();
    wtype* C = C_uptr.get();

    //  Generate Random Data
    for (upL_t c = 0; c < L; c++){
        A[c] = (wtype)c * c * c + 1718498867;
        B[c] = (wtype)c * c + 1404340939;
    }
    memset(C, 0, 2*L * sizeof(wtype));

    //  Prepare Multiply Parameters
    ensure_global_table<wtype>(2*L);
    BasicParametersO mp(get_global_table(), (upL_t)ML, tds);
    memset(mp.M, 0, (upL_t)ML); //  Force commit memory

    Console::print("L = ");
    Console::print_marginr_commas(12, L);
    Console::print(" : ");
    Time::IterationBenchmark bench;
    do{
        LowLevel::mul(mp, C, A, L, B, L);
        bench++;
    }while (bench.ShouldContinue());

    Console::println_unitl_float("secs / multiply", bench.GetLatency());

    return true;
}

void bench_sequential_multiply_latency(){
    Console::println("Please Select a Memory Cap:\n");
    Console::println("Enter a memory limit: (e.g. 768MB, 2.5GB, etc...)");
    upL_t bytes = (upL_t)Console::scan_label_bytes();

    Environment::LockToCore0();
    Console::println();
    Console::println("Benchmarking: L x L -> 2L  (64-bit word multiply)");
    Console::println();

    upL_t L = 8;
    while (bench_multiply<u64_t>(bytes, L)){
        L += L >> 3;
    }
}
void bench_parallel_multiply_latency(){
    Console::println("Please Select a Memory Cap:\n");
    Console::println("Enter a memory limit: (e.g. 768MB, 2.5GB, etc...)");
    upL_t bytes = (upL_t)Console::scan_label_bytes();

    upL_t tds = Environment::GetLogicalProcessors();
    Console::println();
    Console::println("Benchmarking: L x L -> 2L  (64-bit word multiply)");
    Console::println();

    upL_t L = 8;
    while (bench_multiply<u64_t>(bytes, L, tds)){
        L += L >> 3;
    }
}

}
