/* Main.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 06/21/2015
 * Last Modified    : Always...
 * 
 *      The main test-bed for NumberFactory.
 * 
 */

#include <iostream>
using std::cout;
using std::endl;

#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/Time/Time.h"
#include "PublicLibs/Time/StopWatch.h"
#include "PublicLibs/Time/Benchmark.h"
#include "PublicLibs/Environment/Environment.h"
#include "PublicLibs/FileIO/FileIO.h"
#include "ymp/ymp.h"
using namespace ymp;
typedef u64_t wtype;

namespace NumberFactory{
    void MainMenu();
}
using namespace NumberFactory;


int main(){
    //  Use Cilk Plus instead of Windows Thread Pools.
    //  Only available for AVX and AVX2.
//    auto framework = Parallelism::make_framework_by_name("cilk");
//    Parallelism::set_global_framework(*framework);

    //  Run the main Number Factory app.
    NumberFactory::MainMenu();


    system("pause");
}
