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
    //  Run the main Number Factory app.
    NumberFactory::MainMenu();

    //  Sandbox: Force twiddle tables to their maximum sizes.
//    ensure_global_table_bits();






    system("pause");
}
