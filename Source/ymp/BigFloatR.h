/* BigFloatR.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 06/02/2015
 * Last Modified    : 06/21/2015
 * 
 *      Functions Library for BigFloatR.
 * 
 */

#pragma once
#ifndef _ymp_BigFloatR_H
#define _ymp_BigFloatR_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Functions_dll.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Conversions
template <typename wtype> YM_FORCE_INLINE
uiL_t BigFloat_to_string_hex_sizes(upL_t digits);
template <> YM_FORCE_INLINE uiL_t BigFloat_to_string_hex_sizes<u32_t>(upL_t digits){
    uiL_t out;
    YMP_EXPAND_EXCEPTION(BigFloat_c32_to_string_hex_sizes(out, digits));
    return out;
}
template <> YM_FORCE_INLINE uiL_t BigFloat_to_string_hex_sizes<u64_t>(upL_t digits){
    uiL_t out;
    YMP_EXPAND_EXCEPTION(BigFloat_c64_to_string_hex_sizes(out, digits));
    return out;
}
YM_FORCE_INLINE const char* to_string_hex(char* buffer, const BigFloat<u32_t>& x, upL_t digits){
    const char* out;
    YMP_EXPAND_EXCEPTION(BigFloat_c32_to_string_hex(out, buffer, x, digits));
    return out;
}
YM_FORCE_INLINE const char* to_string_hex(char* buffer, const BigFloat<u64_t>& x, upL_t digits){
    const char* out;
    YMP_EXPAND_EXCEPTION(BigFloat_c64_to_string_hex(out, buffer, x, digits));
    return out;
}
////////////////////////////////////////////////////////////////////////////////
template <typename wtype> YM_FORCE_INLINE
uiL_t BigFloat_to_string_dec_sizes(uiL_t& Msize, upL_t digits, upL_t tds = 1);
template <> YM_FORCE_INLINE uiL_t BigFloat_to_string_dec_sizes<u32_t>(uiL_t& Msize, upL_t digits, upL_t tds){
    uiL_t out;
    YMP_EXPAND_EXCEPTION(BigFloat_c32_to_string_dec_sizes(out, Msize, digits, tds));
    return out;
}
template <> YM_FORCE_INLINE uiL_t BigFloat_to_string_dec_sizes<u64_t>(uiL_t& Msize, upL_t digits, upL_t tds){
    uiL_t out;
    YMP_EXPAND_EXCEPTION(BigFloat_c64_to_string_dec_sizes(out, Msize, digits, tds));
    return out;
}
YM_FORCE_INLINE const char* to_string_dec(const BasicParameters& mp, char* buffer, const BigFloat<u32_t>& x, upL_t digits){
    const char* out;
    YMP_EXPAND_EXCEPTION(BigFloat_c32_to_string_dec(mp, out, buffer, x, digits));
    return out;
}
YM_FORCE_INLINE const char* to_string_dec(const BasicParameters& mp, char* buffer, const BigFloat<u64_t>& x, upL_t digits){
    const char* out;
    YMP_EXPAND_EXCEPTION(BigFloat_c64_to_string_dec(mp, out, buffer, x, digits));
    return out;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Reciprocal
template <typename wtype> YM_FORCE_INLINE
void rcp_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
template <> YM_FORCE_INLINE void rcp_sizes<u32_t>(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_rcp_sizes(Tsize, Psize, Msize, p, tds));
}
template <> YM_FORCE_INLINE void rcp_sizes<u64_t>(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_rcp_sizes(Tsize, Psize, Msize, p, tds));
}
YM_FORCE_INLINE void rcp(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T,
    const BigFloat<u32_t>& A,
    upL_t p,
    u32_t* P, upL_t PL,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_rcp(mp, T, A, p, P, PL, spp));
}
YM_FORCE_INLINE void rcp(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T,
    const BigFloat<u64_t>& A,
    upL_t p,
    u64_t* P, upL_t PL,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_rcp(mp, T, A, p, P, PL, spp));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Division
template <typename wtype> YM_FORCE_INLINE
void div_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
template <> YM_FORCE_INLINE void div_sizes<u32_t>(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_div_sizes(Tsize, Psize, Msize, p, tds));
}
template <> YM_FORCE_INLINE void div_sizes<u64_t>(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_div_sizes(Tsize, Psize, Msize, p, tds));
}
YM_FORCE_INLINE void div(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T,
    const BigFloat<u32_t>& N,
    const BigFloat<u32_t>& D,
    upL_t p,
    u32_t* P, upL_t PL,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_div(mp, T, N, D, p, P, PL, spp));
}
YM_FORCE_INLINE void div(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T,
    const BigFloat<u64_t>& N,
    const BigFloat<u64_t>& D,
    upL_t p,
    u64_t* P, upL_t PL,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_div(mp, T, N, D, p, P, PL, spp));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Inverse Square Root (single word)
template <typename wtype> YM_FORCE_INLINE
void invsqrt_uW_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
template <> YM_FORCE_INLINE void invsqrt_uW_sizes<u32_t>(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_invsqrt_uW_sizes(Tsize, Psize, Msize, p, tds));
}
template <> YM_FORCE_INLINE void invsqrt_uW_sizes<u64_t>(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_invsqrt_uW_sizes(Tsize, Psize, Msize, p, tds));
}
YM_FORCE_INLINE void invsqrt_uW(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T, u32_t x,
    upL_t p,
    u32_t* P, upL_t PL,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_invsqrt_uW(mp, T, x, p, P, PL, spp));
}
YM_FORCE_INLINE void invsqrt_uW(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T, u64_t x,
    upL_t p,
    u64_t* P, upL_t PL,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_invsqrt_uW(mp, T, x, p, P, PL, spp));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Power (single word)
template <typename wtype> YM_FORCE_INLINE
void pow_uL_sizes(uiL_t& Tsize, uiL_t& Msize, uiL_t p, upL_t tds);
template <> YM_FORCE_INLINE void pow_uL_sizes<u32_t>(uiL_t& Tsize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_pow_uW_uL_sizes(Tsize, Msize, p, tds));
}
template <> YM_FORCE_INLINE void pow_uL_sizes<u64_t>(uiL_t& Tsize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_pow_uW_uL_sizes(Tsize, Msize, p, tds));
}
YM_FORCE_INLINE void pow_uL(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T,
    u32_t x, uiL_t pow,
    upL_t p,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_pow_uW_uL(mp, T, x, pow, p, spp));
}
YM_FORCE_INLINE void pow_uL(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T,
    u64_t x, uiL_t pow,
    upL_t p,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_pow_uW_uL(mp, T, x, pow, p, spp));
}
////////////////////////////////////////////////////////////////////////////////
template <typename wtype> YM_FORCE_INLINE
void pow_sL_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
template <> YM_FORCE_INLINE void pow_sL_sizes<u32_t>(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_pow_uW_sL_sizes(Tsize, Psize, Msize, p, tds));
}
template <> YM_FORCE_INLINE void pow_sL_sizes<u64_t>(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_pow_uW_sL_sizes(Tsize, Psize, Msize, p, tds));
}
YM_FORCE_INLINE void pow_sL(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T,
    u32_t x, siL_t pow,
    upL_t p,
    u32_t* P, upL_t PL,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c32_pow_uW_sL(mp, T, x, pow, p, P, PL, spp));
}
YM_FORCE_INLINE void pow_sL(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T,
    u64_t x, siL_t pow,
    upL_t p,
    u64_t* P, upL_t PL,
    StatusStack* spp
){
    YMP_EXPAND_EXCEPTION(BigFloatR_c64_pow_uW_sL(mp, T, x, pow, p, P, PL, spp));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
