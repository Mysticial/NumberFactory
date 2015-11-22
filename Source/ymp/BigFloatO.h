/* BigFloatO.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 06/02/2015
 * Last Modified    : 06/21/2015
 * 
 *      Functions Library for BigFloatO.
 * 
 */

#pragma once
#ifndef _ymp_BigFloatO_H
#define _ymp_BigFloatO_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <ostream>
#include "PublicLibs/Exception.h"
#include "PublicLibs/AlignedMalloc.h"
#include "Objects/GenericFloat/GenericFloatOwner.h"
#include "Functions_dll.h"
#include "BigFloatR.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Conversions
template <typename wtype>
inline std::string to_string_hex(const BigFloat<wtype>& x, upL_t digits){
    upL_t Bsize = (upL_t)BigFloat_to_string_hex_sizes<wtype>(digits);
    auto buffer = SmartPointer<char>::malloc_uptr(Bsize, DEFAULT_ALIGNMENT);
    return to_string_hex(buffer.get(), x, digits);
}
template <typename wtype>
inline std::string to_string_dec(const BigFloat<wtype>& x, upL_t digits, upL_t tds = 1){
    uiL_t Msize;
    upL_t Bsize = (upL_t)BigFloat_to_string_dec_sizes<wtype>(Msize, digits, tds);
    auto buffer = SmartPointer<char>::malloc_uptr(Bsize, DEFAULT_ALIGNMENT);
    const char* str;
    {
        BasicParametersO mp(get_global_table(), (upL_t)Msize, tds);
        str = to_string_dec(mp, buffer.get(), x, digits);
    }
    return str;
}
template <typename wtype>
std::ostream& operator<<(std::ostream& os, const BigFloat<wtype>& x){
    os << to_string_dec(x, 20);
    return os;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Reciprocal
template <typename wtype>
void rcp(
    const BasicParameters& mp,
    BigFloatO<wtype>& T,
    const BigFloat<wtype>& A,
    upL_t p,
    StatusStack* spp = nullptr
){
    uiL_t Tsize, Psize, Msize;
    rcp_sizes<wtype>(Tsize, Psize, Msize, p, mp.tds);
    if (T.get_buffersize() < Tsize){
        T.resize_and_zero((upL_t)Tsize);
    }

    auto P = SmartPointer<wtype>::malloc_uptr((upL_t)Psize, sizeof(wtype));

    BigFloatR<wtype> tmp(T.get_baseptr(), (upL_t)Tsize);
    rcp(mp, tmp, A, p, P.get(), (upL_t)Psize, spp);
    T.consume_meta(tmp);
}
template <typename wtype>
BigFloatO<wtype> rcp(
    const BigFloat<wtype>& A,
    upL_t p,
    upL_t tds = 1,
    StatusStack* spp = nullptr
){
    uiL_t Tsize, Psize, Msize;
    rcp_sizes<wtype>(Tsize, Psize, Msize, p, tds);

    auto T = std::unique_ptr<wtype[]>(new wtype[(upL_t)Tsize]);
    auto P = SmartPointer<wtype>::malloc_uptr((upL_t)Psize, sizeof(wtype));
    BasicParametersO mp(get_global_table(), (upL_t)Msize, tds);

    BigFloatR<wtype> tmp(T.get(), (upL_t)Tsize);
    rcp(mp, tmp, A, p, P.get(), (upL_t)Psize, spp);
    return BigFloatO<wtype>(tmp, std::move(T));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Division
template <typename wtype>
void div(
    const BasicParameters& mp,
    BigFloatO<wtype>& T,
    const BigFloat<wtype>& N,
    const BigFloat<wtype>& D,
    upL_t p,
    StatusStack* spp = nullptr
){
    uiL_t Tsize, Psize, Msize;
    div_sizes<wtype>(Tsize, Psize, Msize, p, mp.tds);
    if (T.get_buffersize() < Tsize){
        T.resize_and_zero((upL_t)Tsize);
    }

    auto P = SmartPointer<wtype>::malloc_uptr((upL_t)Psize, sizeof(wtype));

    BigFloatR<wtype> tmp(T.get_baseptr(), (upL_t)Tsize);
    div(mp, tmp, N, D, p, P.get(), (upL_t)Psize, spp
    );T.consume_meta(tmp);
}
template <typename wtype>
BigFloatO<wtype> div(
    const BigFloat<wtype>& N,
    const BigFloat<wtype>& D,
    upL_t p,
    upL_t tds = 1,
    StatusStack* spp = nullptr
){
    uiL_t Tsize, Psize, Msize;
    div_sizes<wtype>(Tsize, Psize, Msize, p, tds);

    auto T = std::unique_ptr<wtype[]>(new wtype[(upL_t)Tsize]);
    auto P = SmartPointer<wtype>::malloc_uptr((upL_t)Psize, sizeof(wtype));
    BasicParametersO mp(get_global_table(), (upL_t)Msize, tds);

    BigFloatR<wtype> tmp(T.get(), (upL_t)Tsize);
    div(mp, tmp, N, D, p, P.get(), (upL_t)Psize, spp);
    return BigFloatO<wtype>(tmp, std::move(T));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Inverse Square Root (single word)
template <typename wtype>
void invsqrt_uW(
    const BasicParameters& mp,
    BigFloatO<wtype>& T, wtype x,
    upL_t p,
    StatusStack* spp = nullptr
){
    uiL_t Tsize, Psize, Msize;
    invsqrt_uW_sizes<wtype>(Tsize, Psize, Msize, p, mp.tds);
    if (T.get_buffersize() < Tsize){
        T.resize_and_zero((upL_t)Tsize);
    }

    auto P = SmartPointer<wtype>::malloc_uptr((upL_t)Psize, sizeof(wtype));

    BigFloatR<wtype> tmp(T.get_baseptr(), (upL_t)Tsize);
    invsqrt_uW(mp, tmp, x, p, P.get(), (upL_t)Psize, spp);
    T.consume_meta(tmp);
}
template <typename wtype>
BigFloatO<wtype> invsqrt_uW(
    wtype x,
    upL_t p,
    upL_t tds = 1,
    StatusStack* spp = nullptr
){
    uiL_t Tsize, Psize, Msize;
    invsqrt_uW_sizes<wtype>(Tsize, Psize, Msize, p, tds);

    auto T = std::unique_ptr<wtype[]>(new wtype[(upL_t)Tsize]);
    auto P = SmartPointer<wtype>::malloc_uptr((upL_t)Psize, sizeof(wtype));
    BasicParametersO mp(get_global_table(), (upL_t)Msize, tds);

    BigFloatR<wtype> tmp(T.get(), (upL_t)Tsize);
    invsqrt_uW(mp, tmp, x, p, P.get(), (upL_t)Psize, spp);
    return BigFloatO<wtype>(tmp, std::move(T));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Square Root
template <typename wtype>
BigFloatO<wtype> invsqrt(const BigFloat<wtype>& x, upL_t p, upL_t tds = 1){
    //  y-cruncher does not have an implementation of the generic square root.
    //  So use this quick-and-dirty implementation instead.

    //  TODO: Move this into y-cruncher and reimplement it properly using BigFloatR.

    if (p == 0){
        if (x.get_sign() < 0){
            throw ym_exception("invsqrt(-1)");
        }

        siL_t exp;
        double xd = x.to_double(exp);
        if (exp & 1){
            exp--;
            xd *= (double)((wtype)0 - 1) + 1.0;
        }
        exp = -exp/2;
        xd = 1 / std::sqrt(xd);

        return BigFloatO<wtype>(xd, exp);
    }

    upL_t s = p / 2 + 1;
    if (p == 1) s = 0;
    if (p == 2) s = 1;

    //  Recurse
    BigFloatO<wtype> r = invsqrt(x, s, tds);

    BigFloatO<wtype> t = sqr(r, p, tds);
    t = mul(t, x, p, tds);
    t = sub(t, BigFloatO<wtype>(1), p);
    t <<= -1;
    t = mul(t, r, s, tds);
    return sub(r, t, p);
}
template <typename wtype>
BigFloatO<wtype> sqrt(const BigFloat<wtype>& x, upL_t p, upL_t tds = 1){
    if (x.is_zero())
        return BigFloatO<wtype>();
    
    //  TODO: After invsqrt() is properly reimplemented in y-cruncher, investigate
    //  if this multiply can be merged into the final iteration for a speedup
    //  in a similar manner to division.
    return mul(invsqrt(x, p, tds), x, p, tds);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Power (single word)
template <typename wtype>
void pow_sL(
    const BasicParameters& mp,
    BigFloatO<wtype>& T,
    wtype x, siL_t pow,
    upL_t p,
    StatusStack* spp = nullptr
){
    if (pow == 0){
        T = BigFloatO<wtype>(1);
        return;

    }else if (pow > 0){
        uiL_t Tsize, Msize;
        pow_uL_sizes<wtype>(Tsize, Msize, p, mp.tds);
        if (T.get_buffersize() < Tsize){
            T.resize_and_zero((upL_t)Tsize);
        }

        BigFloatR<wtype> tmp(T.get_baseptr(), (upL_t)Tsize);
        pow_uL(mp, tmp, x, pow, p, spp);
        T.consume_meta(tmp);

    }else{
        uiL_t Tsize, Psize, Msize;
        pow_sL_sizes<wtype>(Tsize, Psize, Msize, p, mp.tds);
        if (T.get_buffersize() < Tsize){
            T.resize_and_zero((upL_t)Tsize);
        }

        auto P = SmartPointer<wtype>::malloc_uptr((upL_t)Psize, sizeof(wtype));

        BigFloatR<wtype> tmp(T.get_baseptr(), (upL_t)Tsize);
        pow_sL(mp, tmp, x, pow, p, P.get(), (upL_t)Psize, spp);
        T.consume_meta(tmp);

    }
}
template <typename wtype>
BigFloatO<wtype> pow_sL(
    wtype x, siL_t pow,
    upL_t p,
    upL_t tds = 1,
    StatusStack* spp = nullptr
){
    if (pow == 0){
        return BigFloatO<wtype>(1);

    }else if (pow > 0){
        uiL_t Tsize, Msize;
        pow_uL_sizes<wtype>(Tsize, Msize, p, tds);

        auto T = std::unique_ptr<wtype[]>(new wtype[(upL_t)Tsize]);
        BasicParametersO mp(get_global_table(), (upL_t)Msize, tds);

        BigFloatR<wtype> tmp(T.get(), (upL_t)Tsize);
        pow_uL(mp, tmp, x, pow, p, spp);
        return BigFloatO<wtype>(tmp, std::move(T));

    }else{
        uiL_t Tsize, Psize, Msize;
        pow_sL_sizes<wtype>(Tsize, Psize, Msize, p, tds);

        auto T = std::unique_ptr<wtype[]>(new wtype[(upL_t)Tsize]);
        auto P = SmartPointer<wtype>::malloc_uptr((upL_t)Psize, sizeof(wtype));
        BasicParametersO mp(get_global_table(), (upL_t)Msize, tds);

        BigFloatR<wtype> tmp(T.get(), (upL_t)Tsize);
        pow_sL(mp, tmp, x, pow, p, P.get(), (upL_t)Psize, spp);
        return BigFloatO<wtype>(tmp, std::move(T));

    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
