/* BigIntO.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/18/2015
 * Last Modified    : 07/18/2015
 * 
 *      Functions Library for BigIntO.
 * 
 */

#pragma once
#ifndef _ymp_BigIntO_H
#define _ymp_BigIntO_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <ostream>
#include "PublicLibs/AlignedMalloc.h"
#include "Objects/GenericInt/GenericIntOwner.h"
#include "Functions_dll.h"
#include "BigIntR.h"
#include "BigFloatO.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Conversions
template <typename wtype>
inline std::string to_string_hex(const BigInt<wtype>& x){
    upL_t Bsize = (upL_t)BigInt_to_string_hex_sizes<wtype>(x.get_L());
    auto buffer = SmartPointer<char>::malloc_uptr(Bsize, DEFAULT_ALIGNMENT);
    return to_string_hex(buffer.get(), x);
}
template <typename wtype>
inline std::string to_string_dec(const BigInt<wtype>& x, upL_t tds = 1){
    uiL_t Msize;
    upL_t Bsize = (upL_t)BigInt_to_string_dec_sizes<wtype>(Msize, x.get_L(), tds);
    auto buffer = SmartPointer<char>::malloc_uptr(Bsize, DEFAULT_ALIGNMENT);
    const char* str;
    {
        BasicParametersO mp(get_global_table(), (upL_t)Msize, tds);
        str = to_string_dec(mp, buffer.get(), x);
    }
    return str;
}
template <typename wtype>
std::ostream& operator<<(std::ostream& os, const BigInt<wtype>& x){
    os << to_string_dec(x);
    return os;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Division
template <typename wtype>
void divmod_correct_pn(BigIntO<wtype>& Q, BigIntO<wtype>& R, const BigInt<wtype>& B){
    if (R.get_sign() >= 0)
        return;

    //  1st Correction
//    cout << "divmod_correct_pn()" << endl;
    R += B;
    Q -= BigIntO<wtype>(1);
    if (R.get_sign() >= 0)
        return;

    //  2nd Correction
//    cout << "divmod_correct_pn()" << endl;
    R += B;
    Q -= BigIntO<wtype>(1);
    if (R.get_sign() >= 0)
        return;

    throw ym_exception("divmod() failed to converge after 2 pn corrections.");
}
template <typename wtype>
void divmod_correct_pp(BigIntO<wtype>& Q, BigIntO<wtype>& R, const BigInt<wtype>& B){
    if (R < B)
        return;

    //  1st Correction
//    cout << "divmod_correct_pp()" << endl;
    R -= B;
    Q += BigIntO<wtype>(1);
    if (R < B)
        return;

    //  2nd Correction
//    cout << "divmod_correct_pp()" << endl;
    R -= B;
    Q += BigIntO<wtype>(1);
    if (R < B)
        return;

    throw ym_exception("divmod() failed to converge after 2 pp corrections.");
}
template <typename wtype>
void divmod_correct_np(BigIntO<wtype>& Q, BigIntO<wtype>& R, const BigInt<wtype>& B){
    if (R.get_sign() <= 0)
        return;

    //  1st Correction
//    cout << "divmod_correct_np()" << endl;
    R += B;
    Q -= BigIntO<wtype>(1);
    if (R.get_sign() <= 0)
        return;

    //  2nd Correction
//    cout << "divmod_correct_np()" << endl;
    R += B;
    Q -= BigIntO<wtype>(1);
    if (R.get_sign() <= 0)
        return;

    throw ym_exception("divmod() failed to converge after 2 np corrections.");
}
template <typename wtype>
void divmod_correct_nn(BigIntO<wtype>& Q, BigIntO<wtype>& R, const BigInt<wtype>& B){
    if (R > B)
        return;

    //  1st Correction
//    cout << "divmod_correct_nn()" << endl;
    R -= B;
    Q += BigIntO<wtype>(1);
    if (R > B)
        return;

    //  2nd Correction
//    cout << "divmod_correct_nn()" << endl;
    R -= B;
    Q += BigIntO<wtype>(1);
    if (R > B)
        return;

    throw ym_exception("divmod() failed to converge after 2 nn corrections.");
}
template <typename wtype>
void divmod(BigIntO<wtype>& Q, BigIntO<wtype>& R, const BigInt<wtype>& A, const BigInt<wtype>& B, upL_t tds = 1){
    //  Divide and Modulus

    //  This implementation works as follows:
    //      1.  Use floating-point to get a fast approximation of the quotient.
    //      2.  Round/truncate the quotient to an integer.
    //      3.  Multiply-subtract to get the modulus using the quotient.
    //      4.  Make the necessary corrections to get the modulus into the desired range.

    //  Because both quotient and modulus are needed in the algorithm, there is
    //  no benefit to computing either the quotient or the modulus alone.

    //  Integer division is slower than floating-point division due the need for
    //  the multiply-subtract to resolve boundary cases for an exact result.

    if (B.is_zero()){
        throw ym_exception("Divide by 0.");
    }
    if (A.is_zero()){
        Q.set_zero();
        R.set_zero();
        return;
    }

    upL_t AL = A.get_L();
    upL_t BL = B.get_L();

    //  Compute Quotient
    if (BL > AL){
        Q.set_zero();
    }else{
        BigFloatA<wtype> Af(A);
        BigFloatA<wtype> Bf(B);
        upL_t QL = AL - BL + 1;
        BigFloatO<wtype> Qf = div(Af, Bf, QL, tds);
        Q.set_BigFloat(Qf);
    }
//    Q += BigIntO<wtype>(1);

    //  Compute Modulus
    R.set_sub(A, mul(Q, B, tds));

//    cout << Q << endl;
//    cout << R << endl;

    //  Make Correction
    if (R.is_zero())
        return;

    if (B.get_signbool()){
        //  Positive Modulus
        divmod_correct_pn(Q, R, B);
        divmod_correct_pp(Q, R, B);
    }else{
        //  Negative Modulus
        divmod_correct_np(Q, R, B);
        divmod_correct_nn(Q, R, B);
    }
}
template <typename wtype>
BigIntO<wtype> div(const BigInt<wtype>& A, const BigInt<wtype>& B, upL_t tds = 1){
    BigIntO<wtype> Q, R;
    divmod(Q, R, A, B, tds);
    return Q;
}
template <typename wtype>
BigIntO<wtype> mod(const BigInt<wtype>& A, const BigInt<wtype>& B, upL_t tds = 1){
    BigIntO<wtype> Q, R;
    divmod(Q, R, A, B, tds);
    return R;
}
template <typename wtype>
BigIntO<wtype> operator/(const BigInt<wtype>& A, const BigInt<wtype>& B){
    return div(A, B);
}
template <typename wtype>
BigIntO<wtype> operator%(const BigInt<wtype>& A, const BigInt<wtype>& B){
    return mod(A, B);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
