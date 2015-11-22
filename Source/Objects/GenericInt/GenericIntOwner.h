/* GenericIntOwner.h - Generic Integer
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/14/2015
 * Last Modified    : 07/14/2015
 * 
 * 
 *      This is the RAII child class of "GenericInt.h". It is easy to use, but
 *  has a lot of overhead.
 * 
 *  If maximum performance is needed, use "GenericFloatRaw.h" instead.
 * 
 */

#pragma once
#ifndef _ymp_GenericIntOwner_H
#define _ymp_GenericIntOwner_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Modules/Parameters/LookupTables.h"
#include "GenericInt.h"
//#include "GenericIntRaw.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Headers
template <typename wtype>
class BigIntO : public BigInt<wtype>{
    std::unique_ptr<wtype[]> uptr;

#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
    wtype local_buffer[OBJ_SMALL_NUM_BUFFER];
#endif

////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
public:
    BigIntO(BigIntO&& x){
        operator=(std::move(x));
    }
    void operator=(BigIntO&& x){
        this->L = x.L;
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
    BigIntO(const BigIntO& x){
        operator=(x);
    }
    void operator=(const BigIntO& x){
        this->L = x.L;
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
    BigIntO()
        : BigInt<wtype>(0)
    {
        this->base_ptr = nullptr;
        this->set_zero();
    }
    BigIntO(u32_t x){
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        this->set_uW(x);
    }
    BigIntO(s32_t x){
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        if (x >= 0){
            this->set_uW(x);
        }else{
            this->set_uW(-x);
            this->sign = false;
        }
    }
    BigIntO(uiL_t x){
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        this->set_uL(x);
    }
    BigIntO(siL_t x){
        resize_and_zero(OBJ_MIN_BUFFER_SIZE);
        if (x >= 0){
            this->set_uL(x);
        }else{
            this->set_uL(-x);
            this->sign = false;
        }
    }
    explicit BigIntO(const BigFloat<wtype>& x){
        set_BigFloat(x);
    }

    void operator=(const BigInt<wtype>& x){
        if (this->buffer_size < x.L){
            resize_and_zero(x.L);
        }
        this->set_BigInt(x);
    }

////////////////////////////////////////////////////////////////////////////////
//  Setters
public:
    using BigInt<wtype>::set_zero;
    void set_uWs(const wtype* A, upL_t L){
        if (this->buffer_size < L){
            resize_and_zero(L);
        }
        BigInt<wtype>::set_uWs(A, L);
    }
    void set_BigFloat(const BigFloat<wtype>& x){
        siL_t mag = x.get_mag();
        if (mag <= 0){
            set_zero();
            return;
        }
        if (this->buffer_size < (upL_t)mag){
            resize_and_zero((upL_t)mag);
        }
        BigInt<wtype>::set_BigFloat(x);
    }

////////////////////////////////////////////////////////////////////////////////
//  Misc.
public:
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
    using BigInt<wtype>::negate;
    using BigInt<wtype>::operator>>=;

    void operator<<=(upL_t bits){
        if (this->L == 0 || bits == 0)
            return;

        const ukL_t BIT_SHIFT = WordTraits<wtype>::MAG;
        upL_t lp = bits >> BIT_SHIFT;

        if (this->buffer_size <= this->T - this->base_ptr + this->L + lp + 1){
            //  Buffer is not large enough. Resize it.
            BigIntO<wtype> tmp;
            tmp.resize_and_zero(this->L + lp + 1);
            tmp.set_BigInt(*this);
            operator=(std::move(tmp));
        }
        BigInt<wtype>::operator<<=(bits);
    }
    void operator*=(wtype x){
        if (this->buffer_size <= this->T - this->base_ptr + this->L){
            //  Buffer is not large enough. Do out-of-place multiply.
            BigIntO<wtype> tmp;
            tmp.resize_and_zero(this->L + 1);
            tmp.set_mul_uW(*this, x);
            operator=(std::move(tmp));
        }else{
            //  Buffer is large enough. Do in-place multiply.
            BigInt<wtype>::operator*=(x);
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  Setter Arithmetic
public:
    void set_add(const BigInt<wtype>& A, const BigInt<wtype>& B){
        upL_t TL = A.L > B.L ? A.L : B.L;
        if (this->buffer_size <= TL){
            resize_and_zero(TL + 1);
        }
        BigInt<wtype>::set_add(A, B);
    }
    void set_sub(const BigInt<wtype>& A, const BigInt<wtype>& B){
        upL_t TL = A.L > B.L ? A.L : B.L;
        if (this->buffer_size <= TL){
            resize_and_zero(TL + 1);
        }
        BigInt<wtype>::set_sub(A, B);
    }
    void set_sqr(const BasicParameters& mp, const BigInt<wtype>& A){
        upL_t TL = A.L + A.L;
        if (this->buffer_size < TL){
            resize_and_zero(TL);
        }
        BigInt<wtype>::set_sqr(mp, A);
    }
    void set_mul(const BasicParameters& mp, const BigInt<wtype>& A, const BigInt<wtype>& B){
        upL_t TL = A.L + B.L;
        if (this->buffer_size < TL){
            resize_and_zero(TL);
        }
        BigInt<wtype>::set_mul(mp, A, B);
    }

    void set_sqr(const BigInt<wtype>& A, upL_t tds = 1){
        upL_t TL = A.L + A.L;
        if (this->buffer_size < TL){
            resize_and_zero(TL);
        }
        upL_t ML = (upL_t)BigInt<wtype>::mul_Psize(A.L, A.L, tds);
        BasicParametersO mp(get_global_table(), ML, tds);
        BigInt<wtype>::set_sqr(mp, A);
    }
    void set_mul(const BigInt<wtype>& A, const BigInt<wtype>& B, upL_t tds = 1){
        upL_t TL = A.L + B.L;
        if (this->buffer_size < TL){
            resize_and_zero(TL);
        }
        upL_t ML = (upL_t)BigInt<wtype>::mul_Psize(A.L, B.L, tds);
        BasicParametersO mp(get_global_table(), ML, tds);
        BigInt<wtype>::set_mul(mp, A, B);
    }

////////////////////////////////////////////////////////////////////////////////
//  Return Arithmetic
public:
    friend BigIntO<wtype> sqr(const BigInt<wtype>& A, upL_t tds = 1){
        BigIntO<wtype> out;
        out.set_sqr(A, tds);
        return out;
    }
    friend BigIntO<wtype> mul(const BigInt<wtype>& A, const BigInt<wtype>& B, upL_t tds = 1){
        BigIntO<wtype> out;
        out.set_mul(A, B, tds);
        return out;
    }

    friend BigIntO<wtype> operator+(const BigInt<wtype>& A, const BigInt<wtype>& B){
        BigIntO<wtype> out;
        out.set_add(A, B);
        return out;
    }
    friend BigIntO<wtype> operator-(const BigInt<wtype>& A, const BigInt<wtype>& B){
        BigIntO<wtype> out;
        out.set_sub(A, B);
        return out;
    }
    friend BigIntO<wtype> operator*(const BigInt<wtype>& A, const BigInt<wtype>& B){
        BigIntO<wtype> out;
        out.set_mul(A, B);
        return out;
    }

////////////////////////////////////////////////////////////////////////////////
//  In-Place Arithmetic
public:
    void operator+=(const BigInt<wtype>& B){
        *this = *this + B;
    }
    void operator-=(const BigInt<wtype>& B){
        *this = *this - B;
    }
    void operator*=(const BigInt<wtype>& B){
        *this = *this * B;
    }

////////////////////////////////////////////////////////////////////////////////
//  Internals
private:
#ifdef YMP_ENABLE_SMALL_NUM_OPTIMIZATION
    YM_FORCE_INLINE void transfer_local_buffer(const BigIntO& x){
        uptr.reset();
        this->base_ptr = local_buffer;
        this->T = local_buffer + (x.T - x.base_ptr);
        memcpy(local_buffer, x.local_buffer, sizeof(local_buffer));
    }
#endif
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
