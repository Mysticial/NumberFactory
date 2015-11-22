/* Functions_dll.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 06/13/2015
 * Last Modified    : 06/13/2015
 * 
 *      Dynamic Library Exports
 * 
 */

#pragma once
#ifndef _ymp_Functions_dll_H
#define _ymp_Functions_dll_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ExportSafeLibs.h"
#include "Objects/GenericInt/GenericIntRaw.h"
#include "Objects/GenericFloat/GenericFloatRaw.h"
namespace ymp{
    class LookupTable;
    class StatusStack;
extern "C" {
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT void PrintVersionInfo();
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Conversions
YMP_EXPORT ExceptionWrapper* BigInt_c32_to_string_hex_sizes(uiL_t& Bsize, upL_t words);
YMP_EXPORT ExceptionWrapper* BigInt_c64_to_string_hex_sizes(uiL_t& Bsize, upL_t words);
YMP_EXPORT ExceptionWrapper* BigInt_c32_to_string_hex(const char*& str, char* buffer, const BigInt<u32_t>& x);
YMP_EXPORT ExceptionWrapper* BigInt_c64_to_string_hex(const char*& str, char* buffer, const BigInt<u64_t>& x);
YMP_EXPORT ExceptionWrapper* BigInt_c32_to_string_dec_sizes(uiL_t& Bsize, uiL_t& Msize, upL_t words, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigInt_c64_to_string_dec_sizes(uiL_t& Bsize, uiL_t& Msize, upL_t words, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigInt_c32_to_string_dec(const BasicParameters& mp, const char*& str, char* buffer, const BigInt<u32_t>& x);
YMP_EXPORT ExceptionWrapper* BigInt_c64_to_string_dec(const BasicParameters& mp, const char*& str, char* buffer, const BigInt<u64_t>& x);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT ExceptionWrapper* BigFloat_c32_to_string_hex_sizes(uiL_t& Bsize, upL_t digits);
YMP_EXPORT ExceptionWrapper* BigFloat_c64_to_string_hex_sizes(uiL_t& Bsize, upL_t digits);
YMP_EXPORT ExceptionWrapper* BigFloat_c32_to_string_hex(const char*& str, char* buffer, const BigFloat<u32_t>& x, upL_t digits);
YMP_EXPORT ExceptionWrapper* BigFloat_c64_to_string_hex(const char*& str, char* buffer, const BigFloat<u64_t>& x, upL_t digits);
YMP_EXPORT ExceptionWrapper* BigFloat_c32_to_string_dec_sizes(uiL_t& Bsize, uiL_t& Msize, upL_t digits, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloat_c64_to_string_dec_sizes(uiL_t& Bsize, uiL_t& Msize, upL_t digits, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloat_c32_to_string_dec(const BasicParameters& mp, const char*& str, char* buffer, const BigFloat<u32_t>& x, upL_t digits);
YMP_EXPORT ExceptionWrapper* BigFloat_c64_to_string_dec(const BasicParameters& mp, const char*& str, char* buffer, const BigFloat<u64_t>& x, upL_t digits);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Reciprocal
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_rcp_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_rcp_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_rcp(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T,
    const BigFloat<u32_t>& A,
    upL_t p,
    u32_t* P, upL_t PL,
    StatusStack* spp
);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_rcp(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T,
    const BigFloat<u64_t>& A,
    upL_t p,
    u64_t* P, upL_t PL,
    StatusStack* spp
);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Division
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_div_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_div_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_div(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T,
    const BigFloat<u32_t>& N,
    const BigFloat<u32_t>& D,
    upL_t p,
    u32_t* P, upL_t PL,
    StatusStack* spp
);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_div(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T,
    const BigFloat<u64_t>& N,
    const BigFloat<u64_t>& D,
    upL_t p,
    u64_t* P, upL_t PL,
    StatusStack* spp
);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Inverse Square Root (single word)
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_invsqrt_uW_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_invsqrt_uW_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_invsqrt_uW(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T, u32_t x,
    upL_t p,
    u32_t* P, upL_t PL,
    StatusStack* spp
);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_invsqrt_uW(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T, u64_t x,
    upL_t p,
    u64_t* P, upL_t PL,
    StatusStack* spp
);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Power (single word)
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_pow_uW_uL_sizes(uiL_t& Tsize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_pow_uW_uL_sizes(uiL_t& Tsize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_pow_uW_uL(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T,
    u32_t x, siL_t pow,
    upL_t p,
    StatusStack* spp
);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_pow_uW_uL(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T,
    u64_t x, siL_t pow,
    upL_t p,
    StatusStack* spp
);
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_pow_uW_sL_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_pow_uW_sL_sizes(uiL_t& Tsize, uiL_t& Psize, uiL_t& Msize, uiL_t p, upL_t tds);
YMP_EXPORT ExceptionWrapper* BigFloatR_c32_pow_uW_sL(
    const BasicParameters& mp,
    BigFloatR<u32_t>& T,
    u32_t x, siL_t pow,
    upL_t p,
    u32_t* P, upL_t PL,
    StatusStack* spp
);
YMP_EXPORT ExceptionWrapper* BigFloatR_c64_pow_uW_sL(
    const BasicParameters& mp,
    BigFloatR<u64_t>& T,
    u64_t x, siL_t pow,
    upL_t p,
    u64_t* P, upL_t PL,
    StatusStack* spp
);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
