/* LowLevelMath.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/31/2015
 * Last Modified    : 05/31/2015
 * 
 *      Low level subroutines. Analogous to GMP's "mpn" layer.
 * 
 */

#pragma once
#ifndef _ymp_LowLevelMath_H
#define _ymp_LowLevelMath_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "Modules/Parameters/LookupTables.h"
#include "Modules/Parameters/BasicParameters.h"
#include "LowLevel_dll.h"
namespace ymp{
    class LookupTable;
namespace LowLevel{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Alignment
#ifdef YMP_IMPORT
const upL_t ALIGNMENT = GetAlignment();
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Propagate Carry
YM_FORCE_INLINE void add_ip(u32_t* T, u32_t carry_in){
    add1_ip_c32(T, carry_in);
}
YM_FORCE_INLINE void add_ip(u64_t* T, u64_t carry_in){
    add1_ip_c64(T, carry_in);
}
YM_FORCE_INLINE u32_t add_ir(u32_t* T, upL_t L, u32_t carry_in){
    return add1_ir_c32(T, L, carry_in);
}
YM_FORCE_INLINE u64_t add_ir(u64_t* T, upL_t L, u64_t carry_in){
    return add1_ir_c64(T, L, carry_in);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Add/Sub and Propagate Carry
YM_FORCE_INLINE void add_ip(u32_t* T, const u32_t* A, upL_t L, u32_t carry_in = 0){
    add_ip_c32(T, A, L, carry_in);
}
YM_FORCE_INLINE void add_ip(u64_t* T, const u64_t* A, upL_t L, u64_t carry_in = 0){
    add_ip_c64(T, A, L, carry_in);
}
YM_FORCE_INLINE void sub_ip(u32_t* T, const u32_t* A, upL_t L, u32_t carry_in = 0){
    sub_ip_c32(T, A, L, carry_in);
}
YM_FORCE_INLINE void sub_ip(u64_t* T, const u64_t* A, upL_t L, u64_t carry_in = 0){
    sub_ip_c64(T, A, L, carry_in);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Add/Sub and Return Carry (in-place)
YM_FORCE_INLINE u32_t add_ir(u32_t* T, const u32_t* A, upL_t L, u32_t carry_in = 0){
    return add_ir_c32(T, A, L, carry_in);
}
YM_FORCE_INLINE u64_t add_ir(u64_t* T, const u64_t* A, upL_t L, u64_t carry_in = 0){
    return add_ir_c64(T, A, L, carry_in);
}
YM_FORCE_INLINE u32_t sub_ir(u32_t* T, const u32_t* A, upL_t L, u32_t carry_in = 0){
    return sub_ir_c32(T, A, L, carry_in);
}
YM_FORCE_INLINE u64_t sub_ir(u64_t* T, const u64_t* A, upL_t L, u64_t carry_in = 0){
    return sub_ir_c64(T, A, L, carry_in);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Add/Sub and Return Carry (out-of-place)
YM_FORCE_INLINE u32_t add_nr(u32_t* T, const u32_t* A, const u32_t* B, upL_t L, u32_t carry_in = 0){
    return add_nr_c32(T, A, B, L, carry_in);
}
YM_FORCE_INLINE u64_t add_nr(u64_t* T, const u64_t* A, const u64_t* B, upL_t L, u64_t carry_in = 0){
    return add_nr_c64(T, A, B, L, carry_in);
}
YM_FORCE_INLINE u32_t sub_nr(u32_t* T, const u32_t* A, const u32_t* B, upL_t L, u32_t carry_in = 0){
    return sub_nr_c32(T, A, B, L, carry_in);
}
YM_FORCE_INLINE u64_t sub_nr(u64_t* T, const u64_t* A, const u64_t* B, upL_t L, u64_t carry_in = 0){
    return sub_nr_c64(T, A, B, L, carry_in);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Single-Word Multiply
YM_FORCE_INLINE u32_t mul_ir(u32_t* T, upL_t L, u32_t B, u32_t carry_in = 0){
    return mul_ir_c32(T, L, B, carry_in);
}
YM_FORCE_INLINE u64_t mul_ir(u64_t* T, upL_t L, u64_t B, u64_t carry_in = 0){
    return mul_ir_c64(T, L, B, carry_in);
}
YM_FORCE_INLINE u32_t mul_nr(u32_t* T, const u32_t* A, upL_t L, u32_t B, u32_t carry_in = 0){
    return mul_nr_c32(T, A, L, B, carry_in);
}
YM_FORCE_INLINE u64_t mul_nr(u64_t* T, const u64_t* A, upL_t L, u64_t B, u64_t carry_in = 0){
    return mul_nr_c64(T, A, L, B, carry_in);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Incremental Pre-allocated Scratch Memory Size
template <typename wtype> YM_FORCE_INLINE
uiL_t mul_iPsize(uiL_t cwordlen, upL_t tds = 1);
template <> YM_FORCE_INLINE uiL_t mul_iPsize<u32_t>(uiL_t cwordlen, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(mul_iPsize_c32(bytes, cwordlen, tds));
    return bytes;
}
template <> YM_FORCE_INLINE uiL_t mul_iPsize<u64_t>(uiL_t cwordlen, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(mul_iPsize_c64(bytes, cwordlen, tds));
    return bytes;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Basecase Multiply
YM_FORCE_INLINE void sqrB(u32_t* T, const u32_t* A, upL_t AL){
    sqrB_c32(T, A, AL);
}
YM_FORCE_INLINE void sqrB(u64_t* T, const u64_t* A, upL_t AL){
    sqrB_c64(T, A, AL);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void mulB(u32_t* T, const u32_t* A, upL_t AL, const u32_t* B, upL_t BL){
    mulB_c32(T, A, AL, B, BL);
}
YM_FORCE_INLINE void mulB(u64_t* T, const u64_t* A, upL_t AL, const u64_t* B, upL_t BL){
    mulB_c64(T, A, AL, B, BL);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Full Product Multiply
YM_FORCE_INLINE void sqr(const BasicParameters& mp, u32_t* T, const u32_t* A, upL_t AL){
    YMP_EXPAND_EXCEPTION(sqr_p_c32(mp, T, A, AL));
}
YM_FORCE_INLINE void sqr(const BasicParameters& mp, u64_t* T, const u64_t* A, upL_t AL){
    YMP_EXPAND_EXCEPTION(sqr_p_c64(mp, T, A, AL));
}
YM_FORCE_INLINE void mul(const BasicParameters& mp, u32_t* T, const u32_t* A, upL_t AL, const u32_t* B, upL_t BL){
    YMP_EXPAND_EXCEPTION(mul_p_c32(mp, T, A, AL, B, BL));
}
YM_FORCE_INLINE void mul(const BasicParameters& mp, u64_t* T, const u64_t* A, upL_t AL, const u64_t* B, upL_t BL){
    YMP_EXPAND_EXCEPTION(mul_p_c64(mp, T, A, AL, B, BL));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype> YM_FORCE_INLINE
uiL_t mul_Psize(uiL_t cwordlen, upL_t tds = 1);
template <> YM_FORCE_INLINE uiL_t mul_Psize<u32_t>(uiL_t cwordlen, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(mul_Psize32(bytes, cwordlen, tds));
    return bytes;
}
template <> YM_FORCE_INLINE uiL_t mul_Psize<u64_t>(uiL_t cwordlen, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(mul_Psize64(bytes, cwordlen, tds));
    return bytes;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Partial Product Multiply
YM_FORCE_INLINE void sqrp(const BasicParameters& mp, u32_t* T, upL_t s, upL_t e, const u32_t* A, upL_t AL){
    YMP_EXPAND_EXCEPTION(sqrp_p_c32(mp, T, s, e, A, AL));
}
YM_FORCE_INLINE void sqrp(const BasicParameters& mp, u64_t* T, upL_t s, upL_t e, const u64_t* A, upL_t AL){
    YMP_EXPAND_EXCEPTION(sqrp_p_c64(mp, T, s, e, A, AL));
}
YM_FORCE_INLINE void mulp(const BasicParameters& mp, u32_t* T, upL_t s, upL_t e, const u32_t* A, upL_t AL, const u32_t* B, upL_t BL){
    YMP_EXPAND_EXCEPTION(mulp_p_c32(mp, T, s, e, A, AL, B, BL));
}
YM_FORCE_INLINE void mulp(const BasicParameters& mp, u64_t* T, upL_t s, upL_t e, const u64_t* A, upL_t AL, const u64_t* B, upL_t BL){
    YMP_EXPAND_EXCEPTION(mulp_p_c64(mp, T, s, e, A, AL, B, BL));
}
////////////////////////////////////////////////////////////////////////////////
template <typename wtype> YM_FORCE_INLINE
uiL_t mulp_Psize(uiL_t s, uiL_t e, uiL_t AL, uiL_t BL, upL_t tds = 1);
template <> YM_FORCE_INLINE uiL_t mulp_Psize<u32_t>(uiL_t s, uiL_t e, uiL_t AL, uiL_t BL, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(mulp_Psize32(bytes, s, e, AL, BL, tds));
    return bytes;
}
template <> YM_FORCE_INLINE uiL_t mulp_Psize<u64_t>(uiL_t s, uiL_t e, uiL_t AL, uiL_t BL, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(mulp_Psize64(bytes, s, e, AL, BL, tds));
    return bytes;
}
////////////////////////////////////////////////////////////////////////////////
#if 0
template <typename wtype> YM_FORCE_INLINE
wtype ipart_bin_to_radix_iPsize(wtype radix, upL_t BL, upL_t tds);
template <typename wtype> YM_FORCE_INLINE
wtype ipart_bin_to_radix(const BasicParameters& mp, wtype radix, wtype* R, upL_t RL, const wtype* B, upL_t BL, StatusStack* spp = nullptr);
template <typename wtype> YM_FORCE_INLINE
wtype fpart_bin_to_radix_iPsize(wtype radix, upL_t RL, upL_t tds);
template <typename wtype> YM_FORCE_INLINE
wtype fpart_bin_to_radix(const BasicParameters& mp, wtype radix, wtype* R, upL_t RL, const wtype* B, upL_t BL, StatusStack* spp = nullptr);
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
