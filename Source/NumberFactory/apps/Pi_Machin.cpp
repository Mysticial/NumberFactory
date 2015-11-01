/* Pi_Machin.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      Machin-like formulas for Pi.
 * 
 */

#include "../libs/ArcCot.h"

namespace NumberFactory{
typedef u64_t wtype;

void Pi_Machin_Original(){
    MachinFormula<wtype> formula{
        {  5,  4 * 4},
        {239, -1 * 4},
    };
    Run_MachinFormula<wtype, false>(formula, "Pi", "Machin's Formula (Original)");
}
void Pi_Machin_Takano(){
    MachinFormula<wtype> formula{
        {    49,  12 * 4},
        {    57,  32 * 4},
        {   239,  -5 * 4},
        {110443,  12 * 4},
    };
    Run_MachinFormula<wtype, false>(formula, "Pi", "Machin (Takano)");
}
void Pi_Machin_Stormer(){
    MachinFormula<wtype> formula{
        {    57,  44 * 4},
        {   239,   7 * 4},
        {   682, -12 * 4},
        { 12943,  24 * 4},
    };
    Run_MachinFormula<wtype, false>(formula, "Pi", "Machin (Stormer)");
}
void Pi_Machin_Hwang1997(){
    MachinFormula<wtype> formula{
        {    239,  183 * 4},
        {   1023,   32 * 4},
        {   5832,  -68 * 4},
        { 110443,   12 * 4},
        {4841182,  -12 * 4},
        {6826318, -100 * 4},
    };
    Run_MachinFormula<wtype, false>(formula, "Pi", "Machin (Hwang-1997)");
}

}
