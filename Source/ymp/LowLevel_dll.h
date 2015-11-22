/* LowLevel_dll.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 06/06/2015
 * Last Modified    : 06/06/2015
 * 
 */

#pragma once
#ifndef _ymp_LowLevelMath_dll_H
#define _ymp_LowLevelMath_dll_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ExportSafeLibs.h"
namespace ymp{
    class LookupTable;
    struct BasicParameters;
extern "C" {
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  upL_t   GetAlignment();
YMP_EXPORT  void    CompilationOptions();
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  void    add1_ip_c32 (u32_t* T, u32_t carry_in);
YMP_EXPORT  void    add1_ip_c64 (u64_t* T, u64_t carry_in);
YMP_EXPORT  u32_t   add1_ir_c32 (u32_t* T, upL_t L, u32_t carry_in);
YMP_EXPORT  u64_t   add1_ir_c64 (u64_t* T, upL_t L, u64_t carry_in);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  void    add_ip_c32  (u32_t* T, const u32_t* A, upL_t L, u32_t carry_in);
YMP_EXPORT  void    add_ip_c64  (u64_t* T, const u64_t* A, upL_t L, u64_t carry_in);
YMP_EXPORT  void    sub_ip_c32  (u32_t* T, const u32_t* A, upL_t L, u32_t carry_in);
YMP_EXPORT  void    sub_ip_c64  (u64_t* T, const u64_t* A, upL_t L, u64_t carry_in);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  u32_t   add_ir_c32  (u32_t* T, const u32_t* A, upL_t L, u32_t carry_in);
YMP_EXPORT  u64_t   add_ir_c64  (u64_t* T, const u64_t* A, upL_t L, u64_t carry_in);
YMP_EXPORT  u32_t   sub_ir_c32  (u32_t* T, const u32_t* A, upL_t L, u32_t carry_in);
YMP_EXPORT  u64_t   sub_ir_c64  (u64_t* T, const u64_t* A, upL_t L, u64_t carry_in);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  u32_t   add_nr_c32  (u32_t* T, const u32_t* A, const u32_t* B, upL_t L, u32_t carry_in);
YMP_EXPORT  u64_t   add_nr_c64  (u64_t* T, const u64_t* A, const u64_t* B, upL_t L, u64_t carry_in);
YMP_EXPORT  u32_t   sub_nr_c32  (u32_t* T, const u32_t* A, const u32_t* B, upL_t L, u32_t carry_in);
YMP_EXPORT  u64_t   sub_nr_c64  (u64_t* T, const u64_t* A, const u64_t* B, upL_t L, u64_t carry_in);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  u32_t   mul_ir_c32  (u32_t* T, upL_t L, u32_t B, u32_t carry_in);
YMP_EXPORT  u64_t   mul_ir_c64  (u64_t* T, upL_t L, u64_t B, u64_t carry_in);
YMP_EXPORT  u32_t   mul_nr_c32  (u32_t* T, const u32_t* A, upL_t L, u32_t B, u32_t carry_in);
YMP_EXPORT  u64_t   mul_nr_c64  (u64_t* T, const u64_t* A, upL_t L, u64_t B, u64_t carry_in);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  ExceptionWrapper*   mul_iPsize_c32  (uiL_t& bytes, uiL_t cwordlen, upL_t tds);
YMP_EXPORT  ExceptionWrapper*   mul_iPsize_c64  (uiL_t& bytes, uiL_t cwordlen, upL_t tds);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  void    sqrB_c32    (u32_t* T, const u32_t* A, upL_t AL);
YMP_EXPORT  void    sqrB_c64    (u64_t* T, const u64_t* A, upL_t AL);
YMP_EXPORT  void    mulB_c32    (u32_t* T, const u32_t* A, upL_t AL, const u32_t* B, upL_t BL);
YMP_EXPORT  void    mulB_c64    (u64_t* T, const u64_t* A, upL_t AL, const u64_t* B, upL_t BL);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  ExceptionWrapper*   sqr_p_c32   (const BasicParameters& mp, u32_t* T, const u32_t* A, upL_t AL);
YMP_EXPORT  ExceptionWrapper*   sqr_p_c64   (const BasicParameters& mp, u64_t* T, const u64_t* A, upL_t AL);
YMP_EXPORT  ExceptionWrapper*   mul_p_c32   (const BasicParameters& mp, u32_t* T, const u32_t* A, upL_t AL, const u32_t* B, upL_t BL);
YMP_EXPORT  ExceptionWrapper*   mul_p_c64   (const BasicParameters& mp, u64_t* T, const u64_t* A, upL_t AL, const u64_t* B, upL_t BL);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  ExceptionWrapper*   mul_Psize32 (uiL_t& bytes, uiL_t cwordlen, upL_t tds);
YMP_EXPORT  ExceptionWrapper*   mul_Psize64 (uiL_t& bytes, uiL_t cwordlen, upL_t tds);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  ExceptionWrapper*   sqrp_p_c32  (const BasicParameters& mp, u32_t* T, upL_t s, upL_t e, const u32_t* A, upL_t AL);
YMP_EXPORT  ExceptionWrapper*   sqrp_p_c64  (const BasicParameters& mp, u64_t* T, upL_t s, upL_t e, const u64_t* A, upL_t AL);
YMP_EXPORT  ExceptionWrapper*   mulp_p_c32  (const BasicParameters& mp, u32_t* T, upL_t s, upL_t e, const u32_t* A, upL_t AL, const u32_t* B, upL_t BL);
YMP_EXPORT  ExceptionWrapper*   mulp_p_c64  (const BasicParameters& mp, u64_t* T, upL_t s, upL_t e, const u64_t* A, upL_t AL, const u64_t* B, upL_t BL);
////////////////////////////////////////////////////////////////////////////////
YMP_EXPORT  ExceptionWrapper*   mulp_Psize32(uiL_t& bytes, uiL_t s, uiL_t e, uiL_t AL, uiL_t BL, upL_t tds);
YMP_EXPORT  ExceptionWrapper*   mulp_Psize64(uiL_t& bytes, uiL_t s, uiL_t e, uiL_t AL, uiL_t BL, upL_t tds);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
