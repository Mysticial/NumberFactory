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
#ifndef ymp_GenericFloatOwner_H
#define ymp_GenericFloatOwner_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Modules/Parameters/GlobalLookupTable.h"
#include "Modules/Parameters/BasicParametersOwner.h"
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
    std::unique_ptr<wtype[]> m_uptr;

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
        this->m_len = x.m_len;
        this->m_exp = x.m_exp;
        this->m_sign = x.m_sign;
        this->m_buffer_size = x.m_buffer_size;

#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
        if (x.m_uptr.get() != x.m_base_ptr){
            transfer_local_buffer(x);
            x.clear();
            return;
        }
#endif

        m_uptr = std::move(x.m_uptr);
        this->m_base_ptr = x.m_base_ptr;
        this->m_ptr = x.m_ptr;

        x.clear();
    }
    BigFloatO(const BigFloatO& x){
        operator=(x);
    }
    void operator=(const BigFloatO& x){
        this->m_len = x.m_len;
        this->m_exp = x.m_exp;
        this->m_sign = x.m_sign;

#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
        if (x.m_uptr.get() != x.m_base_ptr){
            this->m_buffer_size = OBJ_SMALL_NUM_BUFFER;
            transfer_local_buffer(x);
            return;
        }
#endif

        this->m_buffer_size = this->m_len + OBJ_ALLOCATE_EXTRA;
        m_uptr = std::unique_ptr<wtype[]>(new wtype[this->m_buffer_size]);
        this->m_base_ptr = m_uptr.get();
        this->m_ptr = this->m_base_ptr;

        memcpy(this->m_ptr, x.m_ptr, this->m_len * sizeof(wtype));
    }

///////////////////////////////////////////////////////////////////////////////
//  Constructors
public:
    BigFloatO()
        : BigFloat<wtype>(0)
    {
        this->m_base_ptr = nullptr;
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
            this->m_sign = false;
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
            this->m_sign = false;
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
        resize_and_zero(x.m_len);
        this->set_BigFloat(x, x.m_len);
    }

    void operator=(const BigFloat<wtype>& x){
        if (this->m_buffer_size < x.m_len){
            resize_and_zero(x.m_len);
        }
        this->set_BigFloat(x, x.m_len);
    }

public:
    //  Don't use these unless you know what you're doing. Improper use of these
    //  functions can leave the object in an inconsistent state.
    BigFloatO(const BigFloatR<wtype>& x, std::unique_ptr<wtype[]> uptr){
        if (x.m_base_ptr != uptr.get()){
            //  Assertion Failure
        }
        this->m_ptr = x.m_ptr;
        this->m_len = x.m_len;
        this->m_exp = x.m_exp;
        this->m_sign = x.m_sign;
        this->m_base_ptr = x.m_base_ptr;
        this->m_buffer_size = x.m_buffer_size;
        this->m_uptr = std::move(uptr);
    }
    using BigFloat<wtype>::get_baseptr;
    void consume_meta(const BigFloat<wtype>& x){
        if (this->m_base_ptr != x.m_base_ptr){
            //  Assertion Failure
        }
        this->m_ptr = x.m_ptr;
        this->m_len = x.m_len;
        this->m_exp = x.m_exp;
        this->m_sign = x.m_sign;
    }

////////////////////////////////////////////////////////////////////////////////
//  Misc.
public:
    using BigFloat<wtype>::strip_tzs_all;

    //  Sets the object to zero and releases whatever resources it holds.
    void clear(){
        m_uptr.reset();
        this->m_buffer_size = OBJ_SMALL_NUM_BUFFER;
#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
        this->m_base_ptr = local_buffer;
#else
        this->m_base_ptr = nullptr;
#endif
        this->m_ptr = this->m_base_ptr;
        this->m_len = 0;
        this->m_sign = true;
    }

    //  Set the object to zero and resize the buffer.
    void resize_and_zero(upL_t buffer_size){
#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
        if (buffer_size <= OBJ_SMALL_NUM_BUFFER){
            m_uptr.reset();
            this->m_buffer_size = OBJ_SMALL_NUM_BUFFER;
            this->m_base_ptr = local_buffer;
        }else
#endif
        {
            buffer_size += OBJ_ALLOCATE_EXTRA;
            m_uptr = std::unique_ptr<wtype[]>(new wtype[buffer_size]);
            this->m_buffer_size = buffer_size;
            this->m_base_ptr = m_uptr.get();
        }

        this->m_ptr = this->m_base_ptr;
        this->m_len = 0;
    }

////////////////////////////////////////////////////////////////////////////////
//  Basic Arithmetic
public:
    using BigFloat<wtype>::negate;

    void operator<<=(siL_t pow){
        if (this->m_buffer_size <= this->m_ptr - this->m_base_ptr + this->m_len){
            //  Buffer is not large enough. Resize it.
            BigFloatO<wtype> tmp;
            tmp.resize_and_zero(this->m_len + 1);
            tmp.set_BigFloat(*this, this->m_len + 1);
            operator=(std::move(tmp));
        }
        BigFloat<wtype>::operator<<=(pow);
    }
    void operator*=(wtype x){
        if (this->m_buffer_size <= this->m_ptr - this->m_base_ptr + this->m_len){
            //  Buffer is not large enough. Do out-of-place multiply.
            BigFloatO<wtype> tmp;
            tmp.resize_and_zero(this->m_len + 1);
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
        if (this->m_buffer_size < AL){
            resize_and_zero(AL);
        }
        BigFloat<wtype>::set_add(A, B, p);
    }
    void set_sub(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        upL_t AL = get_add_size(A, B, p);
        if (this->m_buffer_size < AL){
            resize_and_zero(AL);
        }
        BigFloat<wtype>::set_sub(A, B, p);
    }
    void set_sqr(const BasicParameters& mp, const BigFloat<wtype>& A, upL_t p){
        upL_t AL = get_mul_size(A, A, p);
        if (this->m_buffer_size < AL){
            resize_and_zero(AL);
        }
        BigFloat<wtype>::set_sqr(mp, A, p);
    }
    void set_mul(const BasicParameters& mp, const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        upL_t AL = get_mul_size(A, B, p);
        if (this->m_buffer_size < AL){
            resize_and_zero(AL);
        }
        BigFloat<wtype>::set_mul(mp, A, B, p);
    }

    void set_sqr(const BigFloat<wtype>& A, upL_t p, upL_t tds = 0){
        upL_t AL = get_mul_size(A, A, p);
        if (this->m_buffer_size < AL){
            resize_and_zero(AL);
        }
        upL_t ML = (upL_t)BigFloat<wtype>::mul_Psize(A.m_len, A.m_len, p, tds);
        BasicParametersO mp(LookupTables::get_global_table<wtype>(A.m_len + A.m_len), tds, ML);
        BigFloat<wtype>::set_sqr(mp, A, p);
    }
    void set_mul(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p, upL_t tds = 0){
        upL_t AL = get_mul_size(A, B, p);
        if (this->m_buffer_size < AL){
            resize_and_zero(AL);
        }
        upL_t ML = (upL_t)BigFloat<wtype>::mul_Psize(A.m_len, B.m_len, p, tds);
        BasicParametersO mp(LookupTables::get_global_table<wtype>(A.m_len + B.m_len), tds, ML);
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
        m_uptr.reset();
        this->m_base_ptr = local_buffer;
        this->m_ptr = local_buffer + (x.m_ptr - x.m_base_ptr);
        memcpy(local_buffer, x.local_buffer, sizeof(local_buffer));
    }
#endif

    static upL_t get_add_size(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        //  Get the buffer size needed to do addition/subtraction.
        if (A.m_len == 0) return (p > B.m_len ? B.m_len : p) + OBJ_EXTRA_PRECISION;
        if (B.m_len == 0) return (p > A.m_len ? A.m_len : p) + OBJ_EXTRA_PRECISION;

        siL_t magA = A.get_mag();
        siL_t magB = B.get_mag();
        siL_t mag = magA > magB ? magA : magB;
        siL_t exp = A.m_exp < B.m_exp ? A.m_exp : B.m_exp;
        siL_t diff = mag - exp;
        return (upL_t)((siL_t)p > diff ? diff : (siL_t)p) + OBJ_EXTRA_PRECISION;
    }
    static upL_t get_mul_size(const BigFloat<wtype>& A, const BigFloat<wtype>& B, upL_t p){
        //  Get the buffer size needed to do multiplication.
        upL_t TL = A.m_len + B.m_len;
        return (p > TL ? TL : p) + OBJ_EXTRA_PRECISION;
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
