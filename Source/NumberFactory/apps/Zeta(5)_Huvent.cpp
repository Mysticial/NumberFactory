/* Zeta(5)_Huvent.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      http://gery.huvent.pagesperso-orange.fr/zeta5/zeta5.htm
 * 
 */

#include "../libs/BSR_BBP.h"
#include "../libs/ComputeFloatSession.h"

namespace NumberFactory{
using namespace ymp;
typedef u64_t wtype;

class Zeta5_Huvent_Session : public ComputeFloatSession<wtype>{
public:
    Zeta5_Huvent_Session(){
        this->name_short = "Zeta(5)";
        this->algorithm_short = "Huvent";
        this->algorithm_long = "Huvent's Formula";
        this->over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        const double LOG_RATIO_0 = 0.083333333333333333333333333333333333333333333333333;   //  log(2)/log(4096)
        const double LOG_RATIO_1 = 0.10000000000000000000000000000000000000000000000000;    //  log(2)/log(1024)
        const double TERM_RATIO_0 = LOG_RATIO_0 * CHAR_BIT * sizeof(wtype);
        const double TERM_RATIO_1 = LOG_RATIO_1 * CHAR_BIT * sizeof(wtype);
        upL_t terms0 = (upL_t)((double)p * TERM_RATIO_0 + 1);
        upL_t terms1 = (upL_t)((double)p * TERM_RATIO_1 + 1);

        BigFloatO<wtype> T =
                   Standard_BBP_Series<wtype, 5, false>(-12,  1, 24,    126976 * 9, 250604, terms0, p, tds);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  1, 12,    206592 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  3, 24,  33418240 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  1,  6,     12424 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  5, 24,     31744 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  6, 24,  25829376 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  7, 24,     15872 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  2,  6,     37276 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  9, 24,   4177280 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  5, 12,     12912 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 11, 24,      3968 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  6, 12,    197594 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 13, 24,      1984 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  7, 12,      3228 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 15, 24,    522160 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  8, 12,     74552 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 17, 24,       496 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  9, 12,     12612 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 19, 24,       248 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 10, 12,      1553 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 21, 24,     65270 * 9, 250604, terms0, p, tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 22, 24,      6456 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 23, 24,        62 * 9, 250604, terms0, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 24, 24,    128125 * 9, 250604, terms0, p, tds), p);

        T = sub(T, Standard_BBP_Series<wtype, 5, true>(-10,  1,  4,      128 * 369,  62651, terms1, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, true>(-10,  3,  4,        4 * 369,  62651, terms1, p, tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, true>(-10,  4,  4,        1 * 369,  62651, terms1, p, tds), p);

        return T;
    }
};

void Zeta5_Huvent(){
    Zeta5_Huvent_Session().run();
}

}