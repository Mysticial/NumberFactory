/* Catalan_Huvent.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      http://www.numberworld.org/y-cruncher/internals/formulas.html
 * 
 */

#include "NumberFactory/libs/BSR_BBP.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
using namespace ymp;
using wtype = u64_t;

class Catalan_Huvent_Session : public ComputeFloatSession<wtype>{
public:
    Catalan_Huvent_Session(){
        m_name_short = "Catalan";
        m_name_long = "Catalan's Constant";
        m_algorithm_short = "Huvent";
        m_algorithm_long = "Huvent's Formula";
        m_over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        const double LOG_RATIO_0 = 0.50000000000000000000000000000000000000000000000000;    //  log(2)/log(4)
        const double LOG_RATIO_1 = 0.16666666666666666666666666666666666666666666666667;    //  log(2)/log(64)
        const double TERM_RATIO_0 = LOG_RATIO_0 * CHAR_BIT * sizeof(wtype);
        const double TERM_RATIO_1 = LOG_RATIO_1 * CHAR_BIT * sizeof(wtype);

        upL_t p = m_precision;
        upL_t terms0 = (upL_t)((double)p * TERM_RATIO_0 + 1);
        upL_t terms1 = (upL_t)((double)p * TERM_RATIO_1 + 1);

        BigFloatO<wtype> T =
                   Standard_BBP_Series<wtype, 2, true>(-2, 1, 4, 6,  4, terms0, p, m_tds);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-2, 2, 4, 6,  4, terms0, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 2, true>(-2, 3, 4, 3,  4, terms0, p, m_tds), p);

        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6, 1, 4, 8, 32, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6, 2, 4, 4, 32, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6, 3, 4, 1, 32, terms1, p, m_tds), p);

        return T;
    }
};
class Catalan_HuventModified_Session : public ComputeFloatSession<wtype>{
public:
    Catalan_HuventModified_Session(){
        m_name_short = "Catalan";
        m_name_long = "Catalan's Constant";
        m_algorithm_short = "Huvent (modified)";
        m_algorithm_long = "Huvent's Formula (modified)";
        m_over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        const double LOG_RATIO_1 = 0.16666666666666666666666666666666666666666666666667;    //  log(2)/log(64)
        const double TERM_RATIO_1 = LOG_RATIO_1 * CHAR_BIT * sizeof(wtype);

        upL_t p = m_precision;
        upL_t terms1 = (upL_t)((double)p * TERM_RATIO_1 + 1);

        BigFloatO<wtype> T =
                   Standard_BBP_Series<wtype, 2, true>(-6,  1, 12, 576, 384, terms1, p, m_tds);
        T = add(T, Standard_BBP_Series<wtype, 2, true>(-6, 11, 12,  18, 384, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6,  7, 12,  72, 384, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6,  5, 12, 144, 384, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6,  5,  6,   9, 384, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6,  1,  6, 144, 384, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6,  3,  4,   8, 384, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6,  1,  4,  64, 384, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 2, true>(-6,  1,  2,   8, 384, terms1, p, m_tds), p);

        return T;
    }
};

void Catalan_Huvent(){
    Catalan_Huvent_Session().run();
}
void Catalan_HuventModified(){
    Catalan_HuventModified_Session().run();
}

}
