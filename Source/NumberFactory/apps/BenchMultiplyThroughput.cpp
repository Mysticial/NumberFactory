/* BenchMultiplyThroughput.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/09/2015
 * Last Modified    : 08/09/2015
 * 
 *      For a fixed amount of time, perform as many multiplications of size N
 *  as possible using the entire system.
 * 
 *  Given a system with p logical cores, attempt to perform p simultaneous
 *  multiplies of size N using 1 thread each. This usually gives the best
 *  throughput since there is minimal sychronization.
 * 
 *  If there is insufficient memory to do that, reduce the number of instances
 *  and use multiple threads per instance. If there is insufficient memory
 *  to perform one instance, the maximum size N is reached and the benchmark
 *  is complete.
 * 
 */

#include <vector>

#include "../../PublicLibs/ConsoleIO/Margin.h"
#include "../../PublicLibs/ConsoleIO/Label.h"
#include "../../PublicLibs/Time/Benchmark.h"
#include "../../PublicLibs/Environment/Environment.h"
#include "../../PublicLibs/AlignedMalloc.h"
#include "../../ymp/Parallelism.h"
#include "../../ymp/LowLevel.h"

namespace NumberFactory{
using namespace ymp;
template <typename wtype>
class BenchAction : public Threads::IndexAction{
    const wtype* A;
    const wtype* B;
    wtype* C;
    upL_t L;
    upL_t CL;
    void* M;
    upL_t ML;
    upL_t tds;

public:
    std::vector<double>& wall_time;
    std::vector<uiL_t>& iterations;

public:
    BenchAction(
        std::vector<double>& wall_time,
        std::vector<uiL_t>& iterations,
        const wtype* A, const wtype* B, wtype* C,
        upL_t L, upL_t CL,
        void* M, upL_t ML, upL_t tds
    )
        : A(A), B(B), C(C)
        , L(L), CL(CL)
        , M(M), ML(ML), tds(tds)
        , wall_time(wall_time), iterations(iterations)
    {}

    virtual void run(upL_t index) override{
        wtype* current_C = C + CL * index;
        void* current_M = (char*)M + ML * index;

        BasicParameters mp(get_global_table(), current_M, ML, tds);

        Time::IterationBenchmark bench;
        do{
            LowLevel::mul(mp, current_C, A, L, B, L);
            bench++;
        }while (bench.ShouldContinue());

        wall_time[index] = bench.GetElapsed();
        iterations[index] = bench.GetIterations();
    }
};

template <typename wtype>
bool bench_multiply(upL_t memory_limit, upL_t L, upL_t tds, upL_t instances){
    //  Compute memory requirement
    const upL_t FALSE_SHARING_PADDING = DEFAULT_ALIGNMENT / sizeof(wtype);
    upL_t CL = 2*L + FALSE_SHARING_PADDING;
    uiL_t ML = LowLevel::mul_Psize<wtype>(CL, tds);

    //  Align up
    ML += DEFAULT_ALIGNMENT - 1;
    ML &= ~(DEFAULT_ALIGNMENT - 1);

    uiL_t bytes = (2*L + CL * instances) * sizeof(wtype) + ML * instances;
    if (bytes > memory_limit)
        return false;

    //  Allocate Operands
    auto A_uptr = SmartPointer<wtype>::malloc_uptr(L);
    auto B_uptr = SmartPointer<wtype>::malloc_uptr(L);
    auto C_uptr = SmartPointer<wtype>::malloc_uptr(CL * instances);
    wtype* A = A_uptr.get();
    wtype* B = B_uptr.get();
    wtype* C = C_uptr.get();

    //  Generate Random Data
    for (upL_t c = 0; c < L; c++){
        A[c] = (wtype)c * c * c + 1718498867;
        B[c] = (wtype)c * c + 1404340939;
    }
    memset(C, 0, CL * instances * sizeof(wtype));

    //  Prepare Multiply Parameters
    ensure_global_table<wtype>(2*L);
    auto M_uptr = SmartPointer<>::malloc_uptr((upL_t)ML * instances, DEFAULT_ALIGNMENT);
    void* M = M_uptr.get();
    memset(M, 0, (upL_t)ML * instances);    //  Force commit memory

    Console::print("L = ");
    Console::print_marginr_commas(12, L);
    Console::print(" | tds/inst = ");
    Console::print_marginr(2, tds);
    Console::print("/");
    Console::print_marginr(2, instances);
    Console::print(" | ");

    std::vector<double> wall_time(instances);
    std::vector<uiL_t> iterations(instances);

    BenchAction<wtype> action(wall_time, iterations, A, B, C, L, CL, M, (upL_t)ML, tds);
    Threads::RunInParallel(action, 0, instances);

    double total_wall_time = 0;
    for (double time : wall_time){
        total_wall_time += time;
    }

    uiL_t total_iterations = 0;
    for (uiL_t iter : iterations){
        total_iterations += iter;
    }

    Console::println_unitl_float("secs / multiply", total_wall_time / (total_iterations * instances));

    return true;
}

void bench_parallel_multiply_throughput(){
    Console::println("Please Select a Memory Cap:\n");
    Console::println("Enter a memory limit: (e.g. 768MB, 2.5GB, etc...)");
    upL_t bytes = (upL_t)Console::scan_label_bytes();

    Environment::LockToCore0();
    Console::println();
    Console::println("Benchmarking: L x L -> 2L  (64-bit word multiply)");
    Console::println();

    upL_t tds = Environment::GetLogicalProcessors();

    //  Start with tds instances and 1 thread per instance.
    upL_t current_tds = 1;
    upL_t current_instances = tds;

    upL_t L = 8;
    while (true){
        if (bench_multiply<u64_t>(bytes, L, current_tds, current_instances)){
            L += L >> 3;
            continue;
        }

        //  Insufficient memory to run even 1 instance. We are done.
        if (current_instances == 1)
            break;

        //  Decrease # of instances.
        current_tds *= 2;
        current_instances /= 2;
        if (current_tds * current_instances < tds)
            current_instances++;
    }
}

}
