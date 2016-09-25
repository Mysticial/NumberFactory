/* Zeta(5)_Broadhurst.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/11/2015
 * Last Modified    : 07/11/2015
 * 
 *      http://gery.huvent.pagesperso-orange.fr/zeta5/zeta5.htm
 * 
 * 
 *      The Broadhurst and Huvent formulas for Zeta(5) are the same thing.
 *  Expanding out and reducing the Broadhurst formula yields precisely
 *  Huvent's formula.
 * 
 *      Furthermore, the last 3 terms of Huvents formula are numerically equal
 *  to the last 6 terms of Broadhurst's formula.
 * 
 *      Because of this unexpected dependence, the Broadhurst and Huvent formulas
 *  may not suitable as compute/verify pairs.
 * 
 */

#include "NumberFactory/libs/BSR_BBP.h"
#include "NumberFactory/libs/ComputeFloatSession.h"

namespace NumberFactory{
using namespace ymp;
using wtype = u64_t;

class Zeta5_Broadhurst_Session : public ComputeFloatSession<wtype>{
public:
    Zeta5_Broadhurst_Session(){
        m_name_short = "Zeta(5)";
        m_algorithm_short = "Broadhurst";
        m_algorithm_long = "Broadhurst's Formula";
        m_over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        const double LOG_RATIO_0 = 0.25000000000000000000000000000000000000000000000000;    //  log(2)/log(16)
        const double LOG_RATIO_1 = 0.083333333333333333333333333333333333333333333333333;   //  log(2)/log(4096)
        const double LOG_RATIO_2 = 0.050000000000000000000000000000000000000000000000000;   //  log(2)/log(1048576)
        const double TERM_RATIO_0 = LOG_RATIO_0 * CHAR_BIT * sizeof(wtype);
        const double TERM_RATIO_1 = LOG_RATIO_1 * CHAR_BIT * sizeof(wtype);
        const double TERM_RATIO_2 = LOG_RATIO_2 * CHAR_BIT * sizeof(wtype);

        upL_t p = m_precision;
        upL_t terms0 = (upL_t)((double)p * TERM_RATIO_0 + 1);
        upL_t terms1 = (upL_t)((double)p * TERM_RATIO_1 + 1);
        upL_t terms2 = (upL_t)((double)p * TERM_RATIO_2 + 1);

        BigFloatO<wtype> T =
                   Standard_BBP_Series<wtype, 5, false>( -4, 1, 8,     2232 * 128, 62651, terms0, p, m_tds);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>( -4, 2, 8,   116208 * 128, 62651, terms0, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>( -4, 3, 8,     1116 * 128, 62651, terms0, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>( -4, 4, 8,   223632 * 128, 62651, terms0, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>( -4, 5, 8,      558 * 128, 62651, terms0, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>( -4, 6, 8,    29052 * 128, 62651, terms0, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>( -4, 7, 8,      279 * 128, 62651, terms0, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>( -4, 8, 8,   670968 * 128, 62651, terms0, p, m_tds), p);

        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 1, 8,       1240064, 250604, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 2, 8,       1017856, 250604, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 3, 8,        155008, 250604, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 4, 8,        204736, 250604, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 5, 8,         19376, 250604, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 6, 8,         15904, 250604, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 7, 8,          2422, 250604, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 8, 8,           777, 250604, terms1, p, m_tds), p);

        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-20, 1, 8,    48365568, 64154624, terms2, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-20, 3, 8,     1511424, 64154624, terms2, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-20, 4, 8,      377856, 64154624, terms2, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-20, 5, 8,       47232, 64154624, terms2, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-20, 7, 8,        1476, 64154624, terms2, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-20, 8, 8,         369, 64154624, terms2, p, m_tds), p);

        return T;
    }
};
class Zeta5_BroadhurstModified_Session : public ComputeFloatSession<wtype>{
public:
    Zeta5_BroadhurstModified_Session(){
        m_name_short = "Zeta(5)";
        m_algorithm_short = "Broadhurst (modified)";
        m_algorithm_long = "Broadhurst's Formula (modified)";
        m_over_decompose = 2;
    }
    virtual BigFloatO<wtype> compute() override{
        const double LOG_RATIO_1 = 0.083333333333333333333333333333333333333333333333333;   //  log(2)/log(4096)
        const double LOG_RATIO_2 = 0.050000000000000000000000000000000000000000000000000;   //  log(2)/log(1048576)
        const double TERM_RATIO_1 = LOG_RATIO_1 * CHAR_BIT * sizeof(wtype);
        const double TERM_RATIO_2 = LOG_RATIO_2 * CHAR_BIT * sizeof(wtype);

        upL_t p = m_precision;
        upL_t terms1 = (upL_t)((double)p * TERM_RATIO_1 + 1);
        upL_t terms2 = (upL_t)((double)p * TERM_RATIO_2 + 1);

        BigFloatO<wtype> T =
                   Standard_BBP_Series<wtype, 5, false>(-12,  8,  8,    128125,  6766308, terms1, p, m_tds);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  1,  2,      3187,  3492288, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  1,  3,     83871,  2004832, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  2,  3,     83871, 32077312, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  1,  4,     67264,   563859, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  3,  4,      1051,   563859, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  1,  6,     27954,    62651, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  5,  6,     13977,  8019328, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  1,  8,   8354560,  1691577, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  3,  8,   1044320,  1691577, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  5,  8,    130540,  1691577, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  7,  8,     32635,  3383154, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  1, 12,    464832,    62651, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  5, 12,     29052,    62651, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  7, 12,      7263,    62651, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 11, 12,      7263,  1002416, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  1, 24,      9216,     2021, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12,  5, 24,      2304,     2021, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12,  7, 24,      1152,     2021, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 11, 24,       288,     2021, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 13, 24,       144,     2021, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 17, 24,      2232,   125302, terms1, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-12, 19, 24,      1116,   125302, terms1, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-12, 23, 24,       279,   125302, terms1, p, m_tds), p);

        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-20, 1, 8,    48365568, 64154624, terms2, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-20, 3, 8,     1511424, 64154624, terms2, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-20, 4, 8,      377856, 64154624, terms2, p, m_tds), p);
        T = add(T, Standard_BBP_Series<wtype, 5, false>(-20, 5, 8,       47232, 64154624, terms2, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-20, 7, 8,        1476, 64154624, terms2, p, m_tds), p);
        T = sub(T, Standard_BBP_Series<wtype, 5, false>(-20, 8, 8,         369, 64154624, terms2, p, m_tds), p);

        return T;
    }
};

void Zeta5_Broadhurst(){
    Zeta5_Broadhurst_Session().run();
}
void Zeta5_BroadhurstModified(){
    Zeta5_BroadhurstModified_Session().run();
}

}