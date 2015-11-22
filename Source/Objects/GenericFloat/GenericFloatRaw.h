/* GenericFloatRaw.h - Generic Floating-Point
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/26/2011
 * Last Modified    : 06/23/2015
 * 
 * 
 *      This is the non-ownership child class of "GenericFloat.h". It is hard to
 *  use, but has minimal overhead.
 * 
 */

#pragma once
#ifndef _ymp_GenericFloatRaw_H
#define _ymp_GenericFloatRaw_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#ifndef YMP_IMPORT
#include "Objects/BasicInt/BasicIntRaw.h"
#include "Objects/ExactFloat/ExactFloatRaw.h"
#endif
#include "GenericFloat.h"
namespace ymp{
    template <typename> class BasicIntR;
    template <typename> class ExactFloatR;
    template <typename> class SwapFloat;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Headers
template <typename wtype>
class BigFloatR : public BigFloat<wtype>{
////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
private:
    BigFloatR(const BigFloatR&) = delete;
    void operator=(const BigFloatR&) = delete;
public:
    YM_FORCE_INLINE BigFloatR(BigFloatR&& x)
        : BigFloat<wtype>(x)
    {
        x.base_ptr = nullptr;
        x.T = nullptr;
    }
    YM_FORCE_INLINE void operator=(BigFloatR&& x){
        BigFloat<wtype>::operator=(x);
        x.base_ptr = nullptr;
        x.T = nullptr;
    }

////////////////////////////////////////////////////////////////////////////////
//  Constructors
public:
    BigFloatR(){}
    YM_FORCE_INLINE BigFloatR(const BigFloatR<wtype>& x, upL_t L) : BigFloat<wtype>(x, L) {}
    YM_FORCE_INLINE BigFloatR(const BigFloatR<wtype>& x, upL_t s, upL_t L) : BigFloat<wtype>(x, s, L) {}
    YM_FORCE_INLINE BigFloatR(upL_t L, const BigFloatR<wtype>& x) : BigFloat<wtype>(L, x) {}
    YM_FORCE_INLINE BigFloatR(wtype* ptr, upL_t size) : BigFloat<wtype>(size) {
        this->base_ptr = ptr;
        this->T = ptr;
    }
    //YM_FORCE_INLINE BigFloatR(wtype* ptr, upL_t L, siL_t exp) : BigFloat<wtype>(L) {
    //    this->base_ptr = ptr;
    //    this->T = ptr;
    //    this->L = L;
    //    this->exp = exp;
    //    this->sign = true;
    //}

#ifndef YMP_IMPORT
    YM_FORCE_INLINE BigFloatR(BasicIntR<wtype>& A){
        this->replace(A);
    }
    YM_FORCE_INLINE BigFloatR(ExactFloatR<wtype>& A){
        this->replace(A);
    }
#endif

////////////////////////////////////////////////////////////////////////////////
//  Replacers
public:
    YM_FORCE_INLINE void replace(wtype* ptr, upL_t size){
        this->base_ptr = ptr;
        this->buffer_size = size;
        this->T = ptr;
    }

#ifndef YMP_IMPORT
    void replace(BasicIntR<wtype>& A){
        this->base_ptr = A.get_T();
        this->buffer_size = A.get_buffersize();
        this->T = this->base_ptr;
        this->L = A.get_L();
        this->exp = 0;
        this->sign = true;
    }
    void replace(ExactFloatR<wtype>& A){
        this->base_ptr = A.get_baseptr();
        this->buffer_size = A.get_buffersize();
        this->T = A.get_T();
        this->L = A.get_L();
        this->exp = A.get_exp();
        this->sign = true;
    }
#endif

////////////////////////////////////////////////////////////////////////////////
//  Setters
public:
    using BigFloat<wtype>::set_zero;
    using BigFloat<wtype>::set_uW;
    using BigFloat<wtype>::set_uL;
    using BigFloat<wtype>::set_double;
    using BigFloat<wtype>::set_BigFloat;
    using BigFloat<wtype>::set_SwapFloat;

////////////////////////////////////////////////////////////////////////////////
//  Getters
public:
    using BigFloat<wtype>::get_baseptr;

////////////////////////////////////////////////////////////////////////////////
//  Misc.
public:
    using BigFloat<wtype>::strip_tzs_align;
    using BigFloat<wtype>::strip_tzs_all;
    using BigFloat<wtype>::strip_l1s;
    using BigFloat<wtype>::chop_to_precision;
    using BigFloat<wtype>::chop_to_magnitude;
    using BigFloat<wtype>::binary_negate;
    using BigFloat<wtype>::purturb_up;

////////////////////////////////////////////////////////////////////////////////
//  Basic Arithmetic
public:
    using BigFloat<wtype>::negate;
    using BigFloat<wtype>::operator<<=;
    using BigFloat<wtype>::operator*=;
    using BigFloat<wtype>::set_mul_uW;
    using BigFloat<wtype>::add_ip;

////////////////////////////////////////////////////////////////////////////////
//  Addition
public:
    using BigFloat<wtype>::set_add;
    using BigFloat<wtype>::set_sub;
    void set_rsub(const BigFloat<wtype>& A, const BigFloat<wtype>& B, siL_t mag, upL_t p, bool sign);

////////////////////////////////////////////////////////////////////////////////
//  Multiplication
public:
    using BigFloat<wtype>::set_sqr;
    using BigFloat<wtype>::set_mul;
    using BigFloat<wtype>::set_mul_small;

////////////////////////////////////////////////////////////////////////////////
//  Transforms
public:
    using BigFloat<wtype>::set_inverse_fsqr;
    using BigFloat<wtype>::set_inverse_fmul;

////////////////////////////////////////////////////////////////////////////////
//  Common Transform Interface Use Cases
public:
    using BigFloat<wtype>::set_sqr_ip;
    using BigFloat<wtype>::set_mul_ip;
    using BigFloat<wtype>::set_mul_AB_BC;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
