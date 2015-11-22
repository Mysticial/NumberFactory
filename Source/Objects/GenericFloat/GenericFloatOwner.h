/* GenericFloatOwner.h - Generic Floating-Point
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/10/2014
 * Last Modified    : 06/23/2015
 * 
 * 
 *      This is the RAII child class of "GenericFloat.h". It is easy to use, but
 *  has a lot of overhead.
 * 
 *  If maximum performance is needed, use "GenericFloatRaw.h" instead.
 * 
 */

#pragma once
#ifndef _ymp_GenericFloatOwner_H
#define _ymp_GenericFloatOwner_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Modules/Parameters/LookupTables.h"
#ifndef YMP_IMPORT
#include "Objects/ExactFloat/ExactFloat.h"
#endif
#include "GenericFloat.h"
#include "GenericFloatRaw.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Headers
template <typename wtype>
class BigFloatO : public BigFloat<wtype>{
    std::unique_ptr<wtype[]> uptr;

#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
    wtype local_buffer[OBJ_SMALL_NUM_BUFFER];
#endif

////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
public:
    BigFloatO(BigFloatO&& x){
        operator=(std::move(x));
    }
    void operator=(BigFloatO&& x){
        this->L = x.L;
        this->exp = x.exp;
        this->sign = x.sign;
        this->buffer_size = x.buffer_size;

#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
        if (x.uptr.get() != x.base_ptr){
            transfer_local_buffer(x);
            x.clear();
            return;
        }
#endif

        uptr = std::move(x.uptr);
        this->base_ptr = x.base_ptr;
        this->T = x.T;

        x.clear();
    }
    BigFloatO(const BigFloatO& x){
        operator=(x);
    }
    void operator=(const BigFloatO& x){
        this->L = x.L;
        this->exp = x.exp;
        this->sign = x.sign;

#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
        if (x.uptr.get() != x.base_ptr){
            this->buffer_size = OBJ_SMALL_NUM_BUFFER;
            transfer_local_buffer(x);
            return;
        }
#endif

        this->buffer_size = this->L + OBJ_ALLOCATE_EXTRA;
        uptr = std::unique_ptr<wtype[]>(new wtype[this->buffer_size]);
        this->base_ptr = uptr.get();
        this->T = this->base_ptr;

        memcpy(this->T, x.T, this->L * sizeof(wtype));
    }

///////////////////////////////////////////////////////////////////////////////
//  Constructors
public:
    BigFloatO()
        : BigFloat<wtype>(0)
    {
        this->base_ptr = nullptr;
        this->set_zero();
    }
    BigFloatO(u32_t x){
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        this->set_uW(x);
    }
    BigFloatO(s32_t x){
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        if (x >= 0){
            this->set_uW(x);
        }else{
            this->set_uW(-x);
            this->sign = false;
        }
    }
    BigFloatO(uiL_t x){
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        this->set_uL(x);
    }
    BigFloatO(siL_t x){
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        if (x >= 0){
            this->set_uL(x);
        }else{
            this->set_uL(-x);
            this->sign = false;
        }
    }
    BigFloatO(double x, siL_t exp = 0)
        : BigFloat<wtype>(0)
    {
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        this->set_double(x, exp);
    }

    //  Copy from big object.
#ifndef YMP_IMPORT
    BigFloatO(const BasicInt<wtype>& x){
        resize_and_zero(x.get_L());
        this->set_BasicInt(x);
    }
    BigFloatO(const ExactFloat<wtype>& x){
        resize_and_zero(x.get_L());
        this->set_ExactFloat(x);
    }
#endif
    BigFloatO(const BigInt<wtype>& x){
        resize_and_zero(x.get_L());
        this->set_BigInt(x);
    }
    BigFloatO(const BigFloat<wtype>& x){
        resize_and_zero(x.L);
        this->set_BigFloat(x, x.L);
    }

    void operator=(const BigFloat<wtype>& x){
        if (this->buffer_size < x.L){
            resize_and_zero(x.L);
        }
        this->set_BigFloat(x, x.L);
    }

public:
    //  Don't use these unless you know what you're doing. Improper use of these
    //  functions can leave the object in an inconsistent state.
    BigFloatO(const BigFloatR<wtype>& x, std::unique_ptr<wtype[]> uptr){
        if (x.base_ptr != uptr.get()){
            //  Assertion Failure
        }
        this->T = x.T;
        this->L = x.L;
        this->exp = x.exp;
        this->sign = x.sign;
        this->base_ptr = x.base_ptr;
        this->buffer_size = x.buffer_size;
        this->uptr = std::move(uptr);
    }
    using BigFloat<wtype>::get_baseptr;
    void consume_meta(const BigFloat<wtype>& x){
        if (this->base_ptr != x.base_ptr){
            //  Assertion Failure
        }
        this->T = x.T;
        this->L = x.L;
        this->exp = x.exp;
        this->sign = x.sign;
    }

////////////////////////////////////////////////////////////////////////////////
//  Misc.
public:
    using BigFloat<wtype>::strip_tzs_all;

    //  Sets the object to zero and releases whatever resources it holds.
    void clear(){
        uptr.reset();
        this->buffer_size = OBJ_SMALL_NUM_BUFFER;
#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
        this->base_ptr = local_buffer;
#else
        this->base_ptr = nullptr;
#endif
        this->T = this->base_ptr;
        this->L = 0;
        this->sign = true;
    }

    //  Set the object to zero and resize the buffer.
    void resize_and_zero(upL_t buffer_size){
#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
        if (buffer_size <= OBJ_SMALL_NUM_BUFFER){
            uptr.reset();
            this->buffer_size = OBJ_SMALL_NUM_BUFFER;
            this->base_ptr = local_buffer;
        }else
#endif
        {
            buffer_size += OBJ_ALLOCATE_EXTRA;
            uptr = std::unique_ptr<wtype[]>(new wtype[buffer_size]);
            this->buffer_size = buffer_size;
            this->base_ptr = uptr.get();
        }

        this->T = this->base_ptr;
        this->L = 0;
    }

////////////////////////////////////////////////////////////////////////////////
//  Basic Arithmetic
public:
    using BigFloat<wtype>::negate;

    void operator<<=(siL_t pow){
        if (this->buffer_size <= this->T - this->base_ptr + this->L){
            //  Buffer is not large enough. Resize it.
            BigFloatO<wtype> tmp;
            tmp.resize_and_zero(this->L + 1);
            tmp.set_BigFloat(*this, this->L + 1);
            operator=(std::move(tmp));
        }
        BigFloat<wtype>::operator<<=(pow);
    }
    void operator*=(wtype x){
        if (this->buffer_size <= this->T - this->base_ptr + this->L){
            //  Buffer is not large enough. Do out-of-place multiply.
            BigFloatO<wtype> tmp;
            tmp.resize_and_zero(this->L + 1);
            tmp.set_mul_uW(*this, x);
            operator=(std::move(tmp));
        }else{
            //  Buffer is large enough. Do in-place multiply.
            BigFloat<wtype>::operator*=(x);
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  Setter Arithmetic
public:
    void set_add(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        upL_t AL = get_add_size(A, B, p);
        if (this->buffer_size < AL){
            resize_and_zero(AL);
        }
        BigFloat<wtype>::set_add(A, B, p);
    }
    void set_sub(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        upL_t AL = get_add_size(A, B, p);
        if (this->buffer_size < AL){
            resize_and_zero(AL);
        }
        BigFloat<wtype>::set_sub(A, B, p);
    }
    void set_sqr(const BasicParameters& mp, const BigFloat<wtype>& A, upL_t p){
        upL_t AL = get_mul_size(A, A, p);
        if (this->buffer_size < AL){
            resize_and_zero(AL);
        }
        BigFloat<wtype>::set_sqr(mp, A, p);
    }
    void set_mul(const BasicParameters& mp, const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        upL_t AL = get_mul_size(A, B, p);
        if (this->buffer_size < AL){
            resize_and_zero(AL);
        }
        BigFloat<wtype>::set_mul(mp, A, B, p);
    }

    void set_sqr(const BigFloat<wtype>& A, upL_t p, upL_t tds = 0){
        upL_t AL = get_mul_size(A, A, p);
        if (this->buffer_size < AL){
            resize_and_zero(AL);
        }
        upL_t ML = (upL_t)BigFloat<wtype>::mul_Psize(A.L, A.L, p, tds);
        BasicParametersO mp(get_global_table(), ML, tds);
        BigFloat<wtype>::set_sqr(mp, A, p);
    }
    void set_mul(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p, upL_t tds = 0){
        upL_t AL = get_mul_size(A, B, p);
        if (this->buffer_size < AL){
            resize_and_zero(AL);
        }
        upL_t ML = (upL_t)BigFloat<wtype>::mul_Psize(A.L, B.L, p, tds);
        BasicParametersO mp(get_global_table(), ML, tds);
        BigFloat<wtype>::set_mul(mp, A, B, p);
    }

////////////////////////////////////////////////////////////////////////////////
//  Return Arithmetic
public:
    friend BigFloatO<wtype> add(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        BigFloatO<wtype> out;
        out.set_add(A, B, p);
        return out;
    }
    friend BigFloatO<wtype> sub(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        BigFloatO<wtype> out;
        out.set_sub(A, B, p);
        return out;
    }
    friend BigFloatO<wtype> sqr(const BasicParameters& mp, const BigFloat<wtype>& A, upL_t p){
        BigFloatO<wtype> out;
        out.set_sqr(mp, A, p);
        return out;
    }
    friend BigFloatO<wtype> mul(const BasicParameters& mp, const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        BigFloatO<wtype> out;
        out.set_mul(mp, A, B, p);
        return out;
    }

    friend BigFloatO<wtype> sqr(const BigFloat<wtype>& A, upL_t p, upL_t tds = 1){
        BigFloatO<wtype> out;
        out.set_sqr(A, p, tds);
        return out;
    }
    friend BigFloatO<wtype> mul(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p, upL_t tds = 1){
        BigFloatO<wtype> out;
        out.set_mul(A, B, p, tds);
        return out;
    }

////////////////////////////////////////////////////////////////////////////////
//  Internals
private:

#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
    YM_FORCE_INLINE void transfer_local_buffer(const BigFloatO& x){
        uptr.reset();
        this->base_ptr = local_buffer;
        this->T = local_buffer + (x.T - x.base_ptr);
        memcpy(local_buffer, x.local_buffer, sizeof(local_buffer));
    }
#endif

    static upL_t get_add_size(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        //  Get the buffer size needed to do addition/subtraction.
        if (A.L == 0) return (p > B.L ? B.L : p) + OBJ_EXTRA_PRECISION;
        if (B.L == 0) return (p > A.L ? A.L : p) + OBJ_EXTRA_PRECISION;

        siL_t magA = A.get_mag();
        siL_t magB = B.get_mag();
        siL_t mag = magA > magB ? magA : magB;
        siL_t exp = A.exp < B.exp ? A.exp : B.exp;
        siL_t diff = mag - exp;
        return (upL_t)((siL_t)p > diff ? diff : (siL_t)p) + OBJ_EXTRA_PRECISION;
    }
    static upL_t get_mul_size(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        //  Get the buffer size needed to do multiplication.
        upL_t TL = A.L + B.L;
        return (p > TL ? TL : p) + OBJ_EXTRA_PRECISION;
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
