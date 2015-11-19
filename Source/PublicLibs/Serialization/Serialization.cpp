/* Serialization.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 10/28/2012
 * Last Modified    : 09/26/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "ParseException.h"
#include "Serialization.h"
namespace ymp{
namespace Serialization{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const ParseException UNEXPECTED_END_OF_STRING("Unexpected end of string.");
const ParseException UNEXPECTED_END_OF_LINE("Unexpected end of string.");
const ParseException INVALID_DIGIT("Unexpected end of string.");
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void parse_to_tab(const wchar_t*& str){
    wchar_t ch;
    do{
        ch = *str++;
        if (ch == '\0') UNEXPECTED_END_OF_STRING.fire();
        if (ch == '\n') UNEXPECTED_END_OF_LINE.fire();
    }while (ch != '\t');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Line
YM_NO_INLINE void write_line(std::wstring& stream){
    stream += L"\r\n";
}
YM_NO_INLINE void parse_line(const wchar_t*& str){
    wchar_t ch;
    do{
        ch = *str++;
        if (ch == '\0') UNEXPECTED_END_OF_STRING.fire();
    }while (ch != '\n');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
YM_NO_INLINE void write_siL(std::wstring& stream, const std::wstring& label, siL_t x){
    stream += label;
    stream += L"\t";
    stream += std::to_wstring(x);
    write_line(stream);
}
YM_NO_INLINE siL_t parse_siL(const wchar_t*& str){
    //  Skip until after the first tab.
    parse_to_tab(str);

    //  Parse sign
    siL_t x = 0;
    int sign = 0;
    if (str[0] == '-'){
        str++;
        sign = 1;
    }

    //  Parse number
    while (1){
        wchar_t ch = *str++;
        if (ch == '\0') UNEXPECTED_END_OF_STRING.fire();
        if (ch == '\r') continue;
        if (ch == '\n') break;
        if (ch < '0' || ch > '9') INVALID_DIGIT.fire();
        x *= 10;
        x += ch - '0';
    }

    //  Adjust sign
    if (sign)
        x = -x;

    return x;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Floating-Point
YM_NO_INLINE void write_float(std::wstring& stream, const std::wstring& label, double x){
    union{
        double f;
        u64_t i;
    };
    f = x;

    write_siL(stream, label, i);
}
YM_NO_INLINE double parse_float(const wchar_t*& str){
    union{
        double f;
        u64_t i;
    };
    i = (u64_t)parse_siL(str);
    return f;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  String
YM_NO_INLINE void write_str(std::wstring& stream, const std::wstring& label, const std::wstring& x){
    stream += label;
    stream += L"\t";
    stream += x;
    write_line(stream);
}
YM_NO_INLINE std::wstring parse_str(const wchar_t*& str){
    //  Skip until after the first tab.
    parse_to_tab(str);

    std::wstring x;
    while (1){
        wchar_t ch = *str++;
        if (ch == '*') return x;
        if (ch == '\r') continue;
        if (ch == '\n') break;
        x += ch;
    }
    return x;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
