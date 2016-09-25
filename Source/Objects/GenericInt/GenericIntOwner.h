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
#ifndef ymp_GenericIntOwner_H
#define ymp_GenericIntOwner_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Modules/Parameters/GlobalLookupTable.h"
#include "Modules/Parameters/BasicParametersOwner.h"
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
    std::unique_ptr<wtype[]> m_uptr;

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
        this->m_len = x.m_len;
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
    BigIntO(const BigIntO& x){
        operator=(x);
    }
    void operator=(const BigIntO& x){
        this->m_len = x.m_len;
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
    BigIntO()
        : BigInt<wtype>(0)
    {
        this->m_base_ptr = nullptr;
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
            this->m_sign = false;
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
            this->m_sign = false;
        }
    }
    explicit BigIntO(const BigFloat<wtype>& x){
        set_BigFloat(x);
    }

    void operator=(const BigInt<wtype>& x){
        if (this->m_buffer_size < x.m_len){
            resize_and_zero(x.m_len);
        }
        this->set_BigInt(x);
    }

////////////////////////////////////////////////////////////////////////////////
//  Setters
public:
    using BigInt<wtype>::set_zero;
    void set_uWs(const wtype* A, upL_t L){
        if (this->m_buffer_size < L){
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
        if (this->m_buffer_size < (upL_t)mag){
            resize_and_zero((upL_t)mag);
        }
        BigInt<wtype>::set_BigFloat(x);
    }

////////////////////////////////////////////////////////////////////////////////
//  Misc.
public:
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
    using BigInt<wtype>::negate;
    using BigInt<wtype>::operator>>=;

    void operator<<=(upL_t bits){
        if (this->m_len == 0 || bits == 0){
            return;
        }

        const ukL_t BIT_SHIFT = WordTraits<wtype>::MAG;
        upL_t lp = bits >> BIT_SHIFT;

        if (this->m_buffer_size <= this->m_ptr - this->m_base_ptr + this->m_len + lp + 1){
            //  Buffer is not large enough. Resize it.
            BigIntO<wtype> tmp;
            tmp.resize_and_zero(this->m_len + lp + 1);
            tmp.set_BigInt(*this);
            operator=(std::move(tmp));
        }
        BigInt<wtype>::operator<<=(bits);
    }
    void operator*=(wtype x){
        if (this->m_buffer_size <= this->m_ptr - this->m_base_ptr + this->m_len){
            //  Buffer is not large enough. Do out-of-place multiply.
            BigIntO<wtype> tmp;
            tmp.resize_and_zero(this->m_len + 1);
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
        upL_t TL = A.m_len > B.m_len ? A.m_len : B.m_len;
        if (this->m_buffer_size <= TL){
            resize_and_zero(TL + 1);
        }
        BigInt<wtype>::set_add(A, B);
    }
    void set_sub(const BigInt<wtype>& A, const BigInt<wtype>& B){
        upL_t TL = A.m_len > B.m_len ? A.m_len : B.m_len;
        if (this->m_buffer_size <= TL){
            resize_and_zero(TL + 1);
        }
        BigInt<wtype>::set_sub(A, B);
    }
    void set_sqr(const BasicParameters& mp, const BigInt<wtype>& A){
        upL_t TL = A.m_len + A.m_len;
        if (this->m_buffer_size < TL){
            resize_and_zero(TL);
        }
        BigInt<wtype>::set_sqr(mp, A);
    }
    void set_mul(const BasicParameters& mp, const BigInt<wtype>& A, const BigInt<wtype>& B){
        upL_t TL = A.m_len + B.m_len;
        if (this->m_buffer_size < TL){
            resize_and_zero(TL);
        }
        BigInt<wtype>::set_mul(mp, A, B);
    }

    void set_sqr(const BigInt<wtype>& A, upL_t tds = 1){
        upL_t TL = A.m_len + A.m_len;
        if (this->m_buffer_size < TL){
            resize_and_zero(TL);
        }
        upL_t ML = (upL_t)BigInt<wtype>::mul_Psize(A.m_len, A.m_len, tds);
        const LookupTable& tw = LookupTables::get_global_table<wtype>(A.m_len + A.m_len);
        BasicParametersO mp(tw, tds, ML);
        BigInt<wtype>::set_sqr(mp, A);
    }
    void set_mul(const BigInt<wtype>& A, const BigInt<wtype>& B, upL_t tds = 1){
        upL_t TL = A.m_len + B.m_len;
        if (this->m_buffer_size < TL){
            resize_and_zero(TL);
        }
        upL_t ML = (upL_t)BigInt<wtype>::mul_Psize(A.m_len, B.m_len, tds);
        const LookupTable& tw = LookupTables::get_global_table<wtype>(A.m_len + B.m_len);
        BasicParametersO mp(tw, tds, ML);
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
        m_uptr.reset();
        this->m_base_ptr = local_buffer;
        this->m_ptr = local_buffer + (x.m_ptr - x.m_base_ptr);
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
