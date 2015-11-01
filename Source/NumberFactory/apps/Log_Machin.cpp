/* Log_Machin.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      Machin-like formulas for log(x).
 * 
 */

#include "../libs/ArcCot.h"

namespace NumberFactory{
typedef u64_t wtype;

void Log2_Machin_Euler(){
    MachinFormula<wtype> formula{
        {  5,  2},
        {  7,  2},
    };
    Run_MachinFormula<wtype, true>(formula, "Log(2)", "Machin (Euler)");
}
void Log2_Machin_3term(){
    MachinFormula<wtype> formula{
        {  26, 18},
        {4801, -2},
        {8749,  8},
    };
    Run_MachinFormula<wtype, true>(formula, "Log(2)", "Machin (3-terms)");
}
void Log2_Machin_4term(){
    MachinFormula<wtype> formula{
        { 251, 144},
        { 449,  54},
        {4801, -38},
        {8749,  62},
    };
    Run_MachinFormula<wtype, true>(formula, "Log(2)", "Machin (4-terms)");
}

}
