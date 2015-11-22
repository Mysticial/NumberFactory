/* GenericInt.h - Generic Integer
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2015
 * Last Modified    : 07/13/2015
 * 
 * 
 *      This class represents a signed integer. This class has
 *  no restrictions and has support for zero and negative values.
 * 
 *  This class is also fully equipped with fast algorithms.
 * 
 *  This is the abstract base class. It cannot be used directly. All non-const
 *  methods are protected so that child classes have the option of being immutable.
 * 
 */

#pragma once
#ifndef _ymp_GenericInt_H
#define _ymp_GenericInt_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/TemplateTools.h"
#include "PublicLibs/ExportSafeLibs.h"
#include "Modules/Parameters/BasicParameters.h"
#include "Objects/ObjectGlobals.h"
namespace ymp{
    class hash_t;
    template <typename> class BasicInt;
    template <typename> class BigInt;
    template <typename> class BigIntR;
    template <typename> class BigIntO;
    template <typename> class BigFloat;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  DLL Exports
extern "C" {
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_uL(BigInt<u32_t>& T, uiL_t x);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_uL(BigInt<u64_t>& T, uiL_t x);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_uWs(BigInt<u32_t>& T, const u32_t* A, upL_t L);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_uWs(BigInt<u64_t>& T, const u64_t* A, upL_t L);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_BasicInt(BigInt<u32_t>& T, const BasicInt<u32_t>& x);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_BasicInt(BigInt<u64_t>& T, const BasicInt<u64_t>& x);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_BigInt(BigInt<u32_t>& T, const BigInt<u32_t>& x);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_BigInt(BigInt<u64_t>& T, const BigInt<u64_t>& x);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_BigFloat(BigInt<u32_t>& T, const BigFloat<u32_t>& x);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_BigFloat(BigInt<u64_t>& T, const BigFloat<u64_t>& x);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_print(const BigInt<u32_t>& T);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_print(const BigInt<u64_t>& T);
    YMP_EXPORT void BigInt_c32_get_range(const BigInt<u32_t>& T, u32_t* buffer, upL_t s, upL_t L);
    YMP_EXPORT void BigInt_c64_get_range(const BigInt<u64_t>& T, u64_t* buffer, upL_t s, upL_t L);
    YMP_EXPORT int BigInt_c32_cmp(const BigInt<u32_t>& T, const BigInt<u32_t>& B);
    YMP_EXPORT int BigInt_c64_cmp(const BigInt<u64_t>& T, const BigInt<u64_t>& B);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_shl(BigInt<u32_t>& T, upL_t bits);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_shl(BigInt<u64_t>& T, upL_t bits);
    YMP_EXPORT void BigInt_c32_shr(BigInt<u32_t>& T, upL_t bits);
    YMP_EXPORT void BigInt_c64_shr(BigInt<u64_t>& T, upL_t bits);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_mul_uW(BigInt<u32_t>& T, u32_t x);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_mul_uW(BigInt<u64_t>& T, u64_t x);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_mul_uW(BigInt<u32_t>& T, const BigInt<u32_t>& A, u32_t x);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_mul_uW(BigInt<u64_t>& T, const BigInt<u64_t>& A, u64_t x);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_add(BigInt<u32_t>& T, const BigInt<u32_t>& A, const BigInt<u32_t>& B);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_add(BigInt<u64_t>& T, const BigInt<u64_t>& A, const BigInt<u64_t>& B);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_sub(BigInt<u32_t>& T, const BigInt<u32_t>& A, const BigInt<u32_t>& B);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_sub(BigInt<u64_t>& T, const BigInt<u64_t>& A, const BigInt<u64_t>& B);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_mul_Psize(uiL_t& bytes, uiL_t AL, uiL_t BL, upL_t tds);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_mul_Psize(uiL_t& bytes, uiL_t AL, uiL_t BL, upL_t tds);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_sqr(BigInt<u32_t>& T, const BasicParameters& mp, const BigInt<u32_t>& A);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_sqr(BigInt<u64_t>& T, const BasicParameters& mp, const BigInt<u64_t>& A);
    YMP_EXPORT ExceptionWrapper* BigInt_c32_set_mul(BigInt<u32_t>& T, const BasicParameters& mp, const BigInt<u32_t>& A, const BigInt<u32_t>& B);
    YMP_EXPORT ExceptionWrapper* BigInt_c64_set_mul(BigInt<u64_t>& T, const BasicParameters& mp, const BigInt<u64_t>& A, const BigInt<u64_t>& B);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Headers
template <typename wtype>
class BigInt{
protected:
    static const ukL_t WORD_BITS = WordTraits<wtype>::BITS;

    wtype* T;   //  Logical Pointer
    upL_t L;    //  Length
    bool sign;  //  Sign: true = positive or zero, false = negative

    wtype* base_ptr;
    upL_t buffer_size;

    template <typename> friend class BigIntR;
    template <typename> friend class BigIntO;

////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
protected:
    //  Prevent this class from being directly used.
    BigInt(){}
    ~BigInt(){}
    YM_FORCE_INLINE BigInt(const BigInt& x) = default;
    YM_FORCE_INLINE BigInt& operator=(const BigInt& x) = default;

////////////////////////////////////////////////////////////////////////////////
//  Constructors
protected:
    BigInt(upL_t buffer_size) : buffer_size(buffer_size) {}
    YM_FORCE_INLINE BigInt(const BigInt& x, upL_t L){
        //  Create BigInt using the bottom part of an existing BigInt.
        base_ptr = x.T;
        T = base_ptr;
        buffer_size = L;
    }
    YM_FORCE_INLINE BigInt(const BigInt& x, upL_t s, upL_t L){
        //  Create BigInt using the range [s, s + L).
        base_ptr = x.T + s;
        T = base_ptr;
        buffer_size = L;
    }
    YM_FORCE_INLINE BigInt(upL_t L, const BigInt& x){
        //  Create BigInt using the upper part of an existing BigInt.
        base_ptr = x.T + L;
        T = base_ptr;
        buffer_size = x.buffer_size - L - (x.T - x.base_ptr);
    }

////////////////////////////////////////////////////////////////////////////////
//  Setters
protected:
    YM_FORCE_INLINE void set_zero(){
        L = 0;
        sign = true;
    }
    YM_FORCE_INLINE void set_uW(wtype x){
        T = base_ptr;
        L = 1;
        T[0] = x;
        sign = true;
        if (x == 0)
            L = 0;
    }
    void set_uL(uiL_t x);
    void set_uWs(const wtype* A, upL_t L);
    void set_BasicInt(const BasicInt<wtype>& x);
    void set_BigInt(const BigInt<wtype>& x);
    void set_BigFloat(const BigFloat<wtype>& x);

////////////////////////////////////////////////////////////////////////////////
//  Getters
public:
    YM_FORCE_INLINE const wtype* get_T() const{ return T; }
    YM_FORCE_INLINE upL_t get_L() const{ return L; }
    YM_FORCE_INLINE int get_sign() const{
        if (!sign)
            return -1;
        if (L == 0)
            return 0;
        return 1;
    }
    YM_FORCE_INLINE bool get_signbool() const{ return sign; }
    YM_FORCE_INLINE const wtype* get_baseptr() const{ return base_ptr; }
    YM_FORCE_INLINE upL_t get_buffersize() const{ return buffer_size; }
protected:
    YM_FORCE_INLINE wtype* get_baseptr(){ return base_ptr; }

////////////////////////////////////////////////////////////////////////////////
//  Debuggers
public:
    void print() const;
    void check_normalization() const;
    YM_FORCE_INLINE void assert_size(const char* location, upL_t size) const{
        BufferTooSmallException::check(location, buffer_size, size);
    }

////////////////////////////////////////////////////////////////////////////////
//  Misc.
public:
    YM_FORCE_INLINE bool is_zero() const{
        return L == 0;
    }
    wtype operator[](upL_t mag) const{
        //  Returns the word at the mag'th magnitude.
        if (mag >= L)
            return 0;
        return T[mag];
    }
    void get_range(wtype* buffer, upL_t s, upL_t L) const;
    hash_t hash_compute() const;
protected:

////////////////////////////////////////////////////////////////////////////////
//  Compare
public:
    int cmp(const BigInt& B) const;
    friend bool operator<(const BigInt& A, const BigInt& B){
        return A.cmp(B) < 0;
    }
    friend bool operator>(const BigInt& A, const BigInt& B){
        return A.cmp(B) > 0;
    }
    friend bool operator<=(const BigInt& A, const BigInt& B){
        return A.cmp(B) <= 0;
    }
    friend bool operator>=(const BigInt& A, const BigInt& B){
        return A.cmp(B) >= 0;
    }
    friend bool operator==(const BigInt& A, const BigInt& B){
        return A.cmp(B) == 0;
    }

private:
    int ucmp(const BigInt& B) const;

////////////////////////////////////////////////////////////////////////////////
//  Basic Arithmetic
protected:
    YM_FORCE_INLINE void negate(){
        if (L == 0)
            return;
        sign = !sign;
    }
    void operator<<=(upL_t bits);
    void operator>>=(upL_t bits);
    void operator*=(wtype x);
    void set_mul_uW(const BigInt& A, wtype x);
    void add_ip(wtype x, upL_t mag);

////////////////////////////////////////////////////////////////////////////////
//  Addition
protected:
    void set_add(const BigInt& A, const BigInt& B);
    void set_sub(const BigInt& A, const BigInt& B);
private:
    void set_uadd(const BigInt& A, const BigInt& B);
    void set_usub(const BigInt& A, const BigInt& B);

////////////////////////////////////////////////////////////////////////////////
//  Multiplication
public:
    static uiL_t mul_Psize  (uiL_t AL, uiL_t BL, upL_t tds);
    static uiL_t iPsize_clen(uiL_t clen, upL_t tds);
protected:
    void set_sqr      (const BasicParameters& mp, const BigInt& A);
    void set_mul      (const BasicParameters& mp, const BigInt& A, const BigInt& B);

////////////////////////////////////////////////////////////////////////////////
//  Transforms
public:

////////////////////////////////////////////////////////////////////////////////
//  Common Transform Interface Use Cases
protected:

////////////////////////////////////////////////////////////////////////////////
//  Internal
protected:
    void strip_lzs();

    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_uL(BigInt<u32_t>& T, uiL_t x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_uL(BigInt<u64_t>& T, uiL_t x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_uWs(BigInt<u32_t>& T, const u32_t* A, upL_t L);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_uWs(BigInt<u64_t>& T, const u64_t* A, upL_t L);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_BasicInt(BigInt<u32_t>& T, const BasicInt<u32_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_BasicInt(BigInt<u64_t>& T, const BasicInt<u64_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_BigInt(BigInt<u32_t>& T, const BigInt<u32_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_BigInt(BigInt<u64_t>& T, const BigInt<u64_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_BigFloat(BigInt<u32_t>& T, const BigFloat<u32_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_BigFloat(BigInt<u64_t>& T, const BigFloat<u64_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_shl(BigInt<u32_t>& T, upL_t bits);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_shl(BigInt<u64_t>& T, upL_t bits);
    friend YMP_EXPORT void BigInt_c32_shr(BigInt<u32_t>& T, upL_t bits);
    friend YMP_EXPORT void BigInt_c64_shr(BigInt<u64_t>& T, upL_t bits);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_mul_uW(BigInt<u32_t>& T, u32_t x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_mul_uW(BigInt<u64_t>& T, u64_t x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_mul_uW(BigInt<u32_t>& T, const BigInt<u32_t>& A, u32_t x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_mul_uW(BigInt<u64_t>& T, const BigInt<u64_t>& A, u64_t x);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_add(BigInt<u32_t>& T, const BigInt<u32_t>& A, const BigInt<u32_t>& B);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_add(BigInt<u64_t>& T, const BigInt<u64_t>& A, const BigInt<u64_t>& B);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_sub(BigInt<u32_t>& T, const BigInt<u32_t>& A, const BigInt<u32_t>& B);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_sub(BigInt<u64_t>& T, const BigInt<u64_t>& A, const BigInt<u64_t>& B);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_sqr(BigInt<u32_t>& T, const BasicParameters& mp, const BigInt<u32_t>& A);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_sqr(BigInt<u64_t>& T, const BasicParameters& mp, const BigInt<u64_t>& A);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c32_set_mul(BigInt<u32_t>& T, const BasicParameters& mp, const BigInt<u32_t>& A, const BigInt<u32_t>& B);
    friend YMP_EXPORT ExceptionWrapper* BigInt_c64_set_mul(BigInt<u64_t>& T, const BasicParameters& mp, const BigInt<u64_t>& A, const BigInt<u64_t>& B);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  DLL Export Hell
#ifdef YMP_IMPORT
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_uL(uiL_t x){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_uL(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_uL(uiL_t x){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_uL(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_uWs(const u32_t* A, upL_t L){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_uWs(*this, A, L));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_uWs(const u64_t* A, upL_t L){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_uWs(*this, A, L));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_BasicInt(const BasicInt<u32_t>& x){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_BasicInt(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_BasicInt(const BasicInt<u64_t>& x){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_BasicInt(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_BigInt(const BigInt<u32_t>& x){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_BigInt(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_BigInt(const BigInt<u64_t>& x){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_BigInt(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_BigFloat(const BigFloat<u32_t>& x){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_BigFloat(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_BigFloat(const BigFloat<u64_t>& x){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_BigFloat(*this, x));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigInt<u32_t>::print() const{
    YMP_EXPAND_EXCEPTION(BigInt_c32_print(*this));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::print() const{
    YMP_EXPAND_EXCEPTION(BigInt_c64_print(*this));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::get_range(u32_t* buffer, upL_t s, upL_t L) const{
    BigInt_c32_get_range(*this, buffer, s, L);
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::get_range(u64_t* buffer, upL_t s, upL_t L) const{
    BigInt_c64_get_range(*this, buffer, s, L);
}
template <> YM_FORCE_INLINE int BigInt<u32_t>::cmp(const BigInt<u32_t>& B) const{
    return BigInt_c32_cmp(*this, B);
}
template <> YM_FORCE_INLINE int BigInt<u64_t>::cmp(const BigInt<u64_t>& B) const{
    return BigInt_c64_cmp(*this, B);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigInt<u32_t>::operator<<=(upL_t bits){
    YMP_EXPAND_EXCEPTION(BigInt_c32_shl(*this, bits));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::operator<<=(upL_t bits){
    YMP_EXPAND_EXCEPTION(BigInt_c64_shl(*this, bits));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::operator>>=(upL_t bits){
    BigInt_c32_shr(*this, bits);
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::operator>>=(upL_t bits){
    BigInt_c64_shr(*this, bits);
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::operator*=(u32_t x){
    YMP_EXPAND_EXCEPTION(BigInt_c32_mul_uW(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::operator*=(u64_t x){
    YMP_EXPAND_EXCEPTION(BigInt_c64_mul_uW(*this, x));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_mul_uW(const BigInt& A, u32_t x){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_mul_uW(*this, A, x));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_mul_uW(const BigInt& A, u64_t x){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_mul_uW(*this, A, x));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_add(const BigInt<u32_t>& A, const BigInt<u32_t>& B){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_add(*this, A, B));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_add(const BigInt<u64_t>& A, const BigInt<u64_t>& B){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_add(*this, A, B));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_sub(const BigInt<u32_t>& A, const BigInt<u32_t>& B){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_sub(*this, A, B));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_sub(const BigInt<u64_t>& A, const BigInt<u64_t>& B){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_sub(*this, A, B));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE uiL_t BigInt<u32_t>:: mul_Psize(uiL_t AL, uiL_t BL, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(BigInt_c32_mul_Psize(bytes, AL, BL, tds));
    return bytes;
}
template <> YM_FORCE_INLINE uiL_t BigInt<u64_t>:: mul_Psize(uiL_t AL, uiL_t BL, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(BigInt_c64_mul_Psize(bytes, AL, BL, tds));
    return bytes;
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_sqr(const BasicParameters& mp, const BigInt<u32_t>& A){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_sqr(*this, mp, A));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_sqr(const BasicParameters& mp, const BigInt<u64_t>& A){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_sqr(*this, mp, A));
}
template <> YM_FORCE_INLINE void BigInt<u32_t>::set_mul(const BasicParameters& mp, const BigInt<u32_t>& A, const BigInt<u32_t>& B){
    YMP_EXPAND_EXCEPTION(BigInt_c32_set_mul(*this, mp, A, B));
}
template <> YM_FORCE_INLINE void BigInt<u64_t>::set_mul(const BasicParameters& mp, const BigInt<u64_t>& A, const BigInt<u64_t>& B){
    YMP_EXPAND_EXCEPTION(BigInt_c64_set_mul(*this, mp, A, B));
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
