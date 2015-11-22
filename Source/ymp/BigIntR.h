/* BigIntR.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/19/2015
 * Last Modified    : 07/19/2015
 * 
 *      Functions Library for BigIntR.
 * 
 */

#pragma once
#ifndef _ymp_BigIntR_H
#define _ymp_BigIntR_H
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
uiL_t BigInt_to_string_hex_sizes(upL_t words);
template <> YM_FORCE_INLINE uiL_t BigInt_to_string_hex_sizes<u32_t>(upL_t words){
    uiL_t out;
    YMP_EXPAND_EXCEPTION(BigInt_c32_to_string_hex_sizes(out, words));
    return out;
}
template <> YM_FORCE_INLINE uiL_t BigInt_to_string_hex_sizes<u64_t>(upL_t words){
    uiL_t out;
    YMP_EXPAND_EXCEPTION(BigInt_c64_to_string_hex_sizes(out, words));
    return out;
}
YM_FORCE_INLINE const char* to_string_hex(char* buffer, const BigInt<u32_t>& x){
    const char* out;
    YMP_EXPAND_EXCEPTION(BigInt_c32_to_string_hex(out, buffer, x));
    return out;
}
YM_FORCE_INLINE const char* to_string_hex(char* buffer, const BigInt<u64_t>& x){
    const char* out;
    YMP_EXPAND_EXCEPTION(BigInt_c64_to_string_hex(out, buffer, x));
    return out;
}
////////////////////////////////////////////////////////////////////////////////
template <typename wtype> YM_FORCE_INLINE
uiL_t BigInt_to_string_dec_sizes(uiL_t& Msize, upL_t words, upL_t tds = 1);
template <> YM_FORCE_INLINE uiL_t BigInt_to_string_dec_sizes<u32_t>(uiL_t& Msize, upL_t words, upL_t tds){
    uiL_t out;
    YMP_EXPAND_EXCEPTION(BigInt_c32_to_string_dec_sizes(out, Msize, words, tds));
    return out;
}
template <> YM_FORCE_INLINE uiL_t BigInt_to_string_dec_sizes<u64_t>(uiL_t& Msize, upL_t words, upL_t tds){
    uiL_t out;
    YMP_EXPAND_EXCEPTION(BigInt_c64_to_string_dec_sizes(out, Msize, words, tds));
    return out;
}
YM_FORCE_INLINE const char* to_string_dec(const BasicParameters& mp, char* buffer, const BigInt<u32_t>& x){
    const char* out;
    YMP_EXPAND_EXCEPTION(BigInt_c32_to_string_dec(mp, out, buffer, x));
    return out;
}
YM_FORCE_INLINE const char* to_string_dec(const BasicParameters& mp, char* buffer, const BigInt<u64_t>& x){
    const char* out;
    YMP_EXPAND_EXCEPTION(BigInt_c64_to_string_dec(mp, out, buffer, x));
    return out;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
