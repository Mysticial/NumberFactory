/* GenericFloat.h - Generic Floating-Point
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/09/2014
 * Last Modified    : 06/23/2015
 * 
 * 
 *      This class represents a generic floating-point number. This class has
 *  no restrictions and has support for zero and negative values.
 * 
 *  This class is also fully equipped with fast algorithms.
 * 
 *  This is the abstract base class. It cannot be used directly. All non-const
 *  methods are protected so that child classes have the option of being immutable.
 * 
 */

#pragma once
#ifndef _ymp_GenericFloat_H
#define _ymp_GenericFloat_H
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
    namespace LMI{ class AcyclicTransformB; }
    template <typename> class BasicInt;
    template <typename> class ExactFloat;
    template <typename> class BigInt;
    template <typename> class BigFloatTransform;
    template <typename> class BigFloat;
    template <typename> class BigFloatR;
    template <typename> class BigFloatO;
    template <typename> class SwapFloat;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  DLL Exports
extern "C" {
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_uL(BigFloat<u32_t>& T, uiL_t x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_uL(BigFloat<u64_t>& T, uiL_t x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_BasicInt(BigFloat<u32_t>& T, const BasicInt<u32_t>& x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_BasicInt(BigFloat<u64_t>& T, const BasicInt<u64_t>& x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_ExactFloat(BigFloat<u32_t>& T, const ExactFloat<u32_t>& x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_ExactFloat(BigFloat<u64_t>& T, const ExactFloat<u64_t>& x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_BigInt(BigFloat<u32_t>& T, const BigInt<u32_t>& x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_BigInt(BigFloat<u64_t>& T, const BigInt<u64_t>& x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_BigFloat(BigFloat<u32_t>& T, const BigFloat<u32_t>& x, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_BigFloat(BigFloat<u64_t>& T, const BigFloat<u64_t>& x, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_print(const BigFloat<u32_t>& T);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_print(const BigFloat<u64_t>& T);
    YMP_EXPORT double BigFloat_c32_to_double(const BigFloat<u32_t>& T, siL_t& exp);
    YMP_EXPORT double BigFloat_c64_to_double(const BigFloat<u64_t>& T, siL_t& exp);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_double(BigFloat<u32_t>& T, double x, siL_t exp);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_double(BigFloat<u64_t>& T, double x, siL_t exp);
    YMP_EXPORT void BigFloat_c32_get_range(const BigFloat<u32_t>& T, u32_t* buffer, siL_t s, upL_t L);
    YMP_EXPORT void BigFloat_c64_get_range(const BigFloat<u64_t>& T, u64_t* buffer, siL_t s, upL_t L);
    YMP_EXPORT void BigFloat_c32_strip_tzs_all(BigFloat<u32_t>& T);
    YMP_EXPORT void BigFloat_c64_strip_tzs_all(BigFloat<u64_t>& T);
    YMP_EXPORT int BigFloat_c32_cmp(const BigFloat<u32_t>& T, const BigFloat<u32_t>& B);
    YMP_EXPORT int BigFloat_c64_cmp(const BigFloat<u64_t>& T, const BigFloat<u64_t>& B);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_mp2(BigFloat<u32_t>& T, siL_t pow);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_mp2(BigFloat<u64_t>& T, siL_t pow);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_mul_uW(BigFloat<u32_t>& T, u32_t x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_mul_uW(BigFloat<u64_t>& T, u64_t x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_mul_uW(BigFloat<u32_t>& T, const BigFloat<u32_t>& A, u32_t x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_mul_uW(BigFloat<u64_t>& T, const BigFloat<u64_t>& A, u64_t x);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_add(BigFloat<u32_t>& T, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_add(BigFloat<u64_t>& T, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_sub(BigFloat<u32_t>& T, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_sub(BigFloat<u64_t>& T, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_mul_Psize(uiL_t& bytes, uiL_t AL, uiL_t BL, uiL_t p, upL_t tds);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_mul_Psize(uiL_t& bytes, uiL_t AL, uiL_t BL, uiL_t p, upL_t tds);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_sqr(BigFloat<u32_t>& T, const BasicParameters& mp, const BigFloat<u32_t>& A, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_sqr(BigFloat<u64_t>& T, const BasicParameters& mp, const BigFloat<u64_t>& A, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_mul(BigFloat<u32_t>& T, const BasicParameters& mp, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_mul(BigFloat<u64_t>& T, const BasicParameters& mp, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_sqr_ip(BigFloat<u32_t>& T, const BasicParameters& mp, const BigFloat<u32_t>& A, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_sqr_ip(BigFloat<u64_t>& T, const BasicParameters& mp, const BigFloat<u64_t>& A, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_mul_ip(BigFloat<u32_t>& T, const BasicParameters& mp, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_mul_ip(BigFloat<u64_t>& T, const BasicParameters& mp, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p);
    YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_mul_AB_BC(
        const BasicParameters& mp,
        BigFloat<u32_t>& AB, upL_t pAB,
        BigFloat<u32_t>& BC, upL_t pBC,
        const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, const BigFloat<u32_t>& C
    );
    YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_mul_AB_BC(
        const BasicParameters& mp,
        BigFloat<u64_t>& AB, upL_t pAB,
        BigFloat<u64_t>& BC, upL_t pBC,
        const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, const BigFloat<u64_t>& C
    );
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Headers
template <typename wtype>
class BigFloat{
protected:
    static const ukL_t WORD_BITS = WordTraits<wtype>::BITS;

    wtype* T;   //  Logical Pointer
    upL_t L;    //  Length
    siL_t exp;  //  Exponent
    bool sign;  //  Sign: true = positive or zero, false = negative

    wtype* base_ptr;
    upL_t buffer_size;

    template <typename> friend class BigFloatR;
    template <typename> friend class BigFloatO;

////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
protected:
    //  Prevent this class from being directly used.
    BigFloat(){}
    ~BigFloat(){}
    YM_FORCE_INLINE BigFloat(const BigFloat& x) = default;
    YM_FORCE_INLINE BigFloat& operator=(const BigFloat& x) = default;

////////////////////////////////////////////////////////////////////////////////
//  Constructors
protected:
    BigFloat(upL_t buffer_size) : buffer_size(buffer_size) {}
    YM_FORCE_INLINE BigFloat(const BigFloat& x, upL_t L){
        //  Create BigFloat using the bottom part of an existing BigFloat.
        base_ptr = x.T;
        T = base_ptr;
        buffer_size = L;
    }
    YM_FORCE_INLINE BigFloat(const BigFloat& x, upL_t s, upL_t L){
        //  Create BigFloat using the range [s, s + L).
        base_ptr = x.T + s;
        T = base_ptr;
        buffer_size = L;
    }
    YM_FORCE_INLINE BigFloat(upL_t L, const BigFloat& x){
        //  Create BigFloat using the upper part of an existing BigFloat.
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
        exp = 0;
        L = 1;
        T[0] = x;
        sign = true;
        if (x == 0)
            L = 0;
    }
    void set_uL         (uiL_t x);
    void set_BasicInt   (const BasicInt<wtype>& x);
    void set_ExactFloat (const ExactFloat<wtype>& x);
    void set_BigInt     (const BigInt<wtype>& x);
    void set_BigFloat   (const BigFloat<wtype>& x, upL_t p);
    void set_SwapFloat  (const SwapFloat<wtype>& x, upL_t p);

////////////////////////////////////////////////////////////////////////////////
//  Getters
public:
    YM_FORCE_INLINE const wtype* get_T() const{ return T; }
    YM_FORCE_INLINE upL_t get_L() const{ return L; }
    YM_FORCE_INLINE siL_t get_exp() const{ return exp; }
    YM_FORCE_INLINE siL_t get_mag() const{ return exp + (siL_t)L; }
    YM_FORCE_INLINE int get_sign() const{
        if (!sign)
            return -1;
        if (L == 0)
            return 0;
        return 1;
    }
    YM_FORCE_INLINE bool get_signbool() const{ return sign; }
    YM_FORCE_INLINE upL_t get_buffersize() const{ return buffer_size; }
    YM_FORCE_INLINE const wtype* get_baseptr() const{ return base_ptr; }
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
//  Double-Precision Conversions
public:
    double to_double(siL_t& exp) const;
protected:
    void set_double(double x, siL_t exp = 0);

////////////////////////////////////////////////////////////////////////////////
//  Misc.
public:
    YM_FORCE_INLINE bool is_zero() const{
        return L == 0;
    }
    wtype operator[](siL_t mag) const{
        //  Returns the word at the mag'th magnitude.
        if (mag < exp)
            return 0;
        if (mag >= (siL_t)(exp + L))
            return 0;
        return T[mag - exp];
    }
    void get_range(wtype* buffer, siL_t s, upL_t L) const;
    hash_t hash_compute() const;
protected:
    void strip_tzs_all();
    void strip_tzs_align();
    void strip_l1s();
    void chop_to_precision(upL_t p);
    void chop_to_magnitude(siL_t mag);
    void binary_negate();
    void purturb_up();

////////////////////////////////////////////////////////////////////////////////
//  Compare
public:
    int cmp(const BigFloat& B) const;
    friend bool operator<(const BigFloat& A, const BigFloat& B){
        return A.cmp(B) < 0;
    }
    friend bool operator>(const BigFloat& A, const BigFloat& B){
        return A.cmp(B) > 0;
    }
    friend bool operator<=(const BigFloat& A, const BigFloat& B){
        return A.cmp(B) <= 0;
    }
    friend bool operator>=(const BigFloat& A, const BigFloat& B){
        return A.cmp(B) >= 0;
    }
    friend bool operator==(const BigFloat& A, const BigFloat& B){
        return A.cmp(B) == 0;
    }

private:
    int ucmp(const BigFloat& B) const;

////////////////////////////////////////////////////////////////////////////////
//  Basic Arithmetic
protected:
    YM_FORCE_INLINE void negate(){
        if (L == 0)
            return;
        sign = !sign;
    }
    void operator<<=(siL_t pow);
    void operator*=(wtype x);
    void set_mul_uW(const BigFloat& A, wtype x);
    void add_ip(wtype x, siL_t mag);

////////////////////////////////////////////////////////////////////////////////
//  Addition
protected:
    void set_add(const BigFloat& A, const BigFloat& B, upL_t p);
    void set_sub(const BigFloat& A, const BigFloat& B, upL_t p);
private:
    void set_uadd(const BigFloat& A, const BigFloat& B, upL_t p);
    void set_usub(const BigFloat& A, const BigFloat& B, upL_t p);

////////////////////////////////////////////////////////////////////////////////
//  Multiplication
public:
    static uiL_t mul_Psize  (uiL_t AL, uiL_t BL, uiL_t p, upL_t tds);
    static uiL_t iPsize     (uiL_t p, upL_t tds){
        return iPsize_clen(2*p, tds);
    }
    static uiL_t iPsize_clen(uiL_t clen, upL_t tds);
protected:
    void set_sqr      (const BasicParameters& mp, const BigFloat& A, upL_t p);
    void set_mul      (const BasicParameters& mp, const BigFloat& A, const BigFloat& B, upL_t p);
    void set_mul_small(const BasicParameters& mp, const BigFloat& A, const BigFloat& B, upL_t p);

////////////////////////////////////////////////////////////////////////////////
//  Transforms
public:
    BigFloatTransform<wtype> forward(void* P, const LMI::AcyclicTransformB* tp, void* T, upL_t p) const;
protected:
    void set_inverse_fsqr(void* P, BigFloatTransform<wtype>& A, upL_t p);
    void set_inverse_fsqr(void* P, BigFloatTransform<wtype>& A, siL_t mag, upL_t p);
    void set_inverse_fmul(void* P, BigFloatTransform<wtype>& A, const BigFloatTransform<wtype>& B, upL_t p);
    void set_inverse_fmul(void* P, BigFloatTransform<wtype>& A, const BigFloatTransform<wtype>& B, siL_t mag, upL_t p);

////////////////////////////////////////////////////////////////////////////////
//  Common Transform Interface Use Cases
protected:
    void set_sqr_ip(const BasicParameters& mp, const BigFloat& A, upL_t p);
    void set_mul_ip(const BasicParameters& mp, const BigFloat& A, const BigFloat& B, upL_t p);
    static void set_mul_AB_BC(
        const BasicParameters& mp,
        BigFloat& AB, upL_t pAB,
        BigFloat& BC, upL_t pBC,
        const BigFloat& A, const BigFloat& B, const BigFloat& C
    );

////////////////////////////////////////////////////////////////////////////////
//  Internal
protected:
    void strip_lzs();

private:
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_uL(BigFloat<u32_t>& T, uiL_t x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_uL(BigFloat<u64_t>& T, uiL_t x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_BasicInt(BigFloat<u32_t>& T, const BasicInt<u32_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_BasicInt(BigFloat<u64_t>& T, const BasicInt<u64_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_ExactFloat(BigFloat<u32_t>& T, const ExactFloat<u32_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_ExactFloat(BigFloat<u64_t>& T, const ExactFloat<u64_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_BigInt(BigFloat<u32_t>& T, const BigInt<u32_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_BigInt(BigFloat<u64_t>& T, const BigInt<u64_t>& x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_BigFloat(BigFloat<u32_t>& T, const BigFloat<u32_t>& x, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_BigFloat(BigFloat<u64_t>& T, const BigFloat<u64_t>& x, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_double(BigFloat<u32_t>& T, double x, siL_t exp);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_double(BigFloat<u64_t>& T, double x, siL_t exp);
    friend YMP_EXPORT void BigFloat_c32_strip_tzs_all(BigFloat<u32_t>& T);
    friend YMP_EXPORT void BigFloat_c64_strip_tzs_all(BigFloat<u64_t>& T);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_mp2(BigFloat<u32_t>& T, siL_t pow);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_mp2(BigFloat<u64_t>& T, siL_t pow);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_mul_uW(BigFloat<u32_t>& T, u32_t x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_mul_uW(BigFloat<u64_t>& T, u64_t x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_mul_uW(BigFloat<u32_t>& T, const BigFloat<u32_t>& A, u32_t x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_mul_uW(BigFloat<u64_t>& T, const BigFloat<u64_t>& A, u64_t x);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_add(BigFloat<u32_t>& T, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_add(BigFloat<u64_t>& T, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_sub(BigFloat<u32_t>& T, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_sub(BigFloat<u64_t>& T, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_sqr(BigFloat<u32_t>& T, const BasicParameters& mp, const BigFloat<u32_t>& A, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_sqr(BigFloat<u64_t>& T, const BasicParameters& mp, const BigFloat<u64_t>& A, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_mul(BigFloat<u32_t>& T, const BasicParameters& mp, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_mul(BigFloat<u64_t>& T, const BasicParameters& mp, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_sqr_ip(BigFloat<u32_t>& T, const BasicParameters& mp, const BigFloat<u32_t>& A, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_sqr_ip(BigFloat<u64_t>& T, const BasicParameters& mp, const BigFloat<u64_t>& A, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_mul_ip(BigFloat<u32_t>& T, const BasicParameters& mp, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_mul_ip(BigFloat<u64_t>& T, const BasicParameters& mp, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p);
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c32_set_mul_AB_BC(
        const BasicParameters& mp,
        BigFloat<u32_t>& AB, upL_t pAB,
        BigFloat<u32_t>& BC, upL_t pBC,
        const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, const BigFloat<u32_t>& C
    );
    friend YMP_EXPORT ExceptionWrapper* BigFloat_c64_set_mul_AB_BC(
        const BasicParameters& mp,
        BigFloat<u64_t>& AB, upL_t pAB,
        BigFloat<u64_t>& BC, upL_t pBC,
        const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, const BigFloat<u64_t>& C
    );
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Aliasing Object
template <typename wtype>
class BigFloatA : public BigFloat<wtype>{
public:
    YM_FORCE_INLINE BigFloatA(const BigFloat<wtype>& x)
        : BigFloat<wtype>(x)
    {}
    YM_FORCE_INLINE BigFloatA(const BigInt<wtype>& x){
        this->T = const_cast<wtype*>(x.get_T());
        this->L = x.get_L();
        this->exp = 0;
        this->sign = x.get_signbool();
        this->base_ptr = nullptr;
        this->buffer_size = 0;
    }

public:
    using BigFloat<wtype>::is_zero;
    using BigFloat<wtype>::operator[];
    using BigFloat<wtype>::get_range;
    using BigFloat<wtype>::negate;
    using BigFloat<wtype>::strip_tzs_align;
    using BigFloat<wtype>::strip_l1s;
    using BigFloat<wtype>::chop_to_precision;
    using BigFloat<wtype>::chop_to_magnitude;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  DLL Export Hell
#ifdef YMP_IMPORT
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_uL(uiL_t x){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_uL(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_uL(uiL_t x){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_uL(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_BasicInt(const BasicInt<u32_t>& x){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_BasicInt(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_BasicInt(const BasicInt<u64_t>& x){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_BasicInt(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_ExactFloat(const ExactFloat<u32_t>& x){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_ExactFloat(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_ExactFloat(const ExactFloat<u64_t>& x){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_ExactFloat(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_BigInt(const BigInt<u32_t>& x){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_BigInt(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_BigInt(const BigInt<u64_t>& x){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_BigInt(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_BigFloat(const BigFloat<u32_t>& x, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_BigFloat(*this, x, p));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_BigFloat(const BigFloat<u64_t>& x, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_BigFloat(*this, x, p));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigFloat<u32_t>::print() const{
    YMP_EXPAND_EXCEPTION(BigFloat_c32_print(*this));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::print() const{
    YMP_EXPAND_EXCEPTION(BigFloat_c64_print(*this));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE double BigFloat<u32_t>::to_double(siL_t& exp) const{
    return BigFloat_c32_to_double(*this, exp);
}
template <> YM_FORCE_INLINE double BigFloat<u64_t>::to_double(siL_t& exp) const{
    return BigFloat_c64_to_double(*this, exp);
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_double(double x, siL_t exp){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_double(*this, x, exp));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_double(double x, siL_t exp){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_double(*this, x, exp));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigFloat<u32_t>::get_range(u32_t* buffer, siL_t s, upL_t L) const{
    BigFloat_c32_get_range(*this, buffer, s, L);
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::get_range(u64_t* buffer, siL_t s, upL_t L) const{
    BigFloat_c64_get_range(*this, buffer, s, L);
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::strip_tzs_all(){
    BigFloat_c32_strip_tzs_all(*this);
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::strip_tzs_all(){
    BigFloat_c64_strip_tzs_all(*this);
}
template <> YM_FORCE_INLINE int BigFloat<u32_t>::cmp(const BigFloat<u32_t>& B) const{
    return BigFloat_c32_cmp(*this, B);
}
template <> YM_FORCE_INLINE int BigFloat<u64_t>::cmp(const BigFloat<u64_t>& B) const{
    return BigFloat_c64_cmp(*this, B);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigFloat<u32_t>::operator<<=(siL_t pow){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_mp2(*this, pow));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::operator<<=(siL_t pow){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_mp2(*this, pow));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::operator*=(u32_t x){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_mul_uW(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::operator*=(u64_t x){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_mul_uW(*this, x));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_mul_uW(const BigFloat& A, u32_t x){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_mul_uW(*this, A, x));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_mul_uW(const BigFloat& A, u64_t x){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_mul_uW(*this, A, x));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_add(const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_add(*this, A, B, p));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_add(const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_add(*this, A, B, p));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_sub(const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_sub(*this, A, B, p));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_sub(const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_sub(*this, A, B, p));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE uiL_t BigFloat<u32_t>:: mul_Psize(uiL_t AL, uiL_t BL, uiL_t p, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(BigFloat_c32_mul_Psize(bytes, AL, BL, p, tds));
    return bytes;
}
template <> YM_FORCE_INLINE uiL_t BigFloat<u64_t>:: mul_Psize(uiL_t AL, uiL_t BL, uiL_t p, upL_t tds){
    uiL_t bytes;
    YMP_EXPAND_EXCEPTION(BigFloat_c64_mul_Psize(bytes, AL, BL, p, tds));
    return bytes;
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_sqr(const BasicParameters& mp, const BigFloat<u32_t>& A, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_sqr(*this, mp, A, p));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_sqr(const BasicParameters& mp, const BigFloat<u64_t>& A, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_sqr(*this, mp, A, p));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_mul(const BasicParameters& mp, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_mul(*this, mp, A, B, p));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_mul(const BasicParameters& mp, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_mul(*this, mp, A, B, p));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_mul_ip(const BasicParameters& mp, const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_mul_ip(*this, mp, A, B, p));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_mul_ip(const BasicParameters& mp, const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, upL_t p){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_mul_ip(*this, mp, A, B, p));
}
template <> YM_FORCE_INLINE void BigFloat<u32_t>::set_mul_AB_BC(
    const BasicParameters& mp,
    BigFloat<u32_t>& AB, upL_t pAB,
    BigFloat<u32_t>& BC, upL_t pBC,
    const BigFloat<u32_t>& A, const BigFloat<u32_t>& B, const BigFloat<u32_t>& C
){
    YMP_EXPAND_EXCEPTION(BigFloat_c32_set_mul_AB_BC(mp, AB, pAB, BC, pBC, A, B, C));
}
template <> YM_FORCE_INLINE void BigFloat<u64_t>::set_mul_AB_BC(
    const BasicParameters& mp,
    BigFloat<u64_t>& AB, upL_t pAB,
    BigFloat<u64_t>& BC, upL_t pBC,
    const BigFloat<u64_t>& A, const BigFloat<u64_t>& B, const BigFloat<u64_t>& C
){
    YMP_EXPAND_EXCEPTION(BigFloat_c64_set_mul_AB_BC(mp, AB, pAB, BC, pBC, A, B, C));
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
