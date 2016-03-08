/* String.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/31/2015
 * Last Modified    : 05/31/2015
 * 
 */

#pragma once
#ifndef _ymp_ExportSafeLibs_String_H
#define _ymp_ExportSafeLibs_String_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "Array.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename type>
class dll_str{
    size_t L;
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
    const size_t size() const{
        return L;
    }

    type& operator[](size_t index){
        return ptr[index];
    }
    type operator[](size_t index) const{
        return ptr[index];
    }

    friend std::basic_ostream<type>& operator<<(std::basic_ostream<type>& os, const dll_str& str){
        return os << str.c_str();
    }

private:
    static size_t strlen(const type* str){
        size_t L = 0;
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
}
#endif
