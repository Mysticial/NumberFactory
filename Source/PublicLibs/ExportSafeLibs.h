/* ExportSafeLibs.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/31/2015
 * Last Modified    : 05/31/2015
 * 
 *      DLL Export safe versions of various STL classes.
 * 
 */

#pragma once
#ifndef _ymp_ExportSafeLibs_H
#define _ymp_ExportSafeLibs_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "CompilerSettings.h"
#include "Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Unique Pointer
template <typename type>
static void local_ptr_deletor(type* ptr){
    delete ptr;
}
////////////////////////////////////////////////////////////////////////////////
template <typename type>
class dll_uptr{
    type* ptr;
    void (*deletor)(type*);

public:
    //  Rule of 5
    ~dll_uptr(){
        reset();
    }

    dll_uptr(const dll_uptr&) = delete;
    void operator=(const dll_uptr&) = delete;

    dll_uptr(dll_uptr&& x)
        : ptr(x.ptr)
        , deletor(x.deletor)
    {
        x.ptr = nullptr;
    }
    void operator=(dll_uptr&& x){
        reset();
        ptr = x.ptr;
        deletor = x.deletor;
        x.ptr = nullptr;
    }
    void reset(){
        if (ptr != nullptr)
            deletor(ptr);
    }

public:
    dll_uptr() : ptr(nullptr) {}
    dll_uptr(type* ptr, void (*deletor)(type*) = &local_ptr_deletor)
        : ptr(ptr)
        , deletor(deletor)
    {}

    type* get() const{
        return ptr;
    }
    type* operator->() const{
        return ptr;
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Unique Array
template <typename type>
static void local_arr_deletor(type* ptr){
    delete[] ptr;
}
////////////////////////////////////////////////////////////////////////////////
template <typename type>
class dll_uarr{
    type* ptr;
    void (*deletor)(type*);

public:
    //  Rule of 5
    ~dll_uarr(){
        reset();
    }

    dll_uarr(const dll_uarr&) = delete;
    void operator=(const dll_uarr&) = delete;

    dll_uarr(dll_uarr&& x)
        : ptr(x.ptr)
        , deletor(x.deletor)
    {
        x.ptr = nullptr;
    }
    void operator=(dll_uarr&& x){
        reset();
        ptr = x.ptr;
        deletor = x.deletor;
        x.ptr = nullptr;
    }
    void reset(){
        if (ptr != nullptr)
            deletor(ptr);
    }

public:
    dll_uarr() : ptr(nullptr) {}
    dll_uarr(type* ptr, void (*deletor)(type*) = &local_arr_deletor)
        : ptr(ptr)
        , deletor(deletor)
    {}

    type* get() const{
        return ptr;
    }
    type& operator[](upL_t index){
        return ptr[index];
    }
    const type& operator[](upL_t index) const{
        return ptr[index];
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  String (Conversion Class)
template <typename type>
class dll_str{
    upL_t L;
    dll_uarr<type> ptr;

public:
    dll_str(dll_str&& x)
        : L(x.L)
        , ptr(std::move(x.ptr))
    {
        x.L = 0;
    }
    void operator=(dll_str&& x){
        L = x.L;
        ptr = std::move(x.ptr);
        x.L = 0;
    }

    dll_str() : L(0) {}
    dll_str(const type* str)
        : L(strlen(str))
        , ptr(new type[L + 1])
    {
        memcpy(ptr.get(), str, (L + 1) * sizeof(type));
    }
    dll_str(const std::basic_string<type>& str)
        : L(str.size())
        , ptr(new type[L + 1])
    {
        memcpy(ptr.get(), str.c_str(), (L + 1) * sizeof(type));
    }

    //operator const type*() const{
    //    return std::basic_string<type>(ptr.get(), L);
    //}
    operator std::basic_string<type>() const{
        return std::basic_string<type>(ptr.get(), L);
    }
    const type* c_str() const{
        return ptr.get();
    }
    const upL_t size() const{
        return L;
    }

    type& operator[](upL_t index){
        return ptr[index];
    }
    type operator[](upL_t index) const{
        return ptr[index];
    }

    friend std::basic_ostream<type>& operator<<(std::basic_ostream<type>& os, const dll_str& str){
        return os << str.c_str();
    }

private:
    static upL_t strlen(const type* str){
        upL_t L = 0;
        while (str[L] != '\0') L++;
        return L;
    }
};
////////////////////////////////////////////////////////////////////////////////
typedef dll_str<char>       dll_astr;
typedef dll_str<wchar_t>    dll_wstr;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Exceptions
#ifdef YMP_STANDALONE
#define YMP_COLLAPSE_EXCEPTION(expression)  {expression} return nullptr;
#define YMP_EXPAND_EXCEPTION(expression)    expression
#else
#define YMP_COLLAPSE_EXCEPTION(expression)  \
    try{    \
        expression  \
    }catch (ym_exception& _err){    \
        _err.print();   \
        return new ExceptionWrapper(ExceptionWrapper::STD, _err.what());    \
    }catch (std::exception& _err){  \
        return new ExceptionWrapper(ExceptionWrapper::STD, _err.what());    \
    }catch (...){   \
        return new ExceptionWrapper(ExceptionWrapper::UNKNOWN, "Unknown Exception");    \
    }   \
    return nullptr;
#define YMP_EXPAND_EXCEPTION(expression){   \
        ExceptionWrapper* _error = (expression);    \
        if (_error != nullptr) _error->Rethrow();   \
    }
#endif
////////////////////////////////////////////////////////////////////////////////
class ExceptionWrapper{
public:
    enum Type{
        YMP,
        STD,
        UNKNOWN,
    };

    ExceptionWrapper(Type type, const char* str, void (*deletor)(ExceptionWrapper*) = &local_ptr_deletor)
        : type(type)
        , deletor(deletor)
        , data(str)
    {}
    void Rethrow(){
        dll_uptr<ExceptionWrapper> uptr(this, deletor);
        throw std::string(data);
    }

private:
    Type type;
    void (*deletor)(ExceptionWrapper*);
    dll_astr data;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
