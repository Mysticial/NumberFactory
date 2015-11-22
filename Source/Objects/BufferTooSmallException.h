/* BufferTooSmallException.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/10/2014
 * Last Modified    : 08/10/2014
 * 
 */

#pragma once
#ifndef _ymp_BufferTooSmallException_H
#define _ymp_BufferTooSmallException_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Exception.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YMC_EXCEPTION_BUFFER_TOO_SMALL  3
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BufferTooSmallException : public ym_exception{
    virtual void fire_this() const{
        throw *this;
    }

    uiL_t buffer_size;
    uiL_t required_size;
    std::string description;

public:
    YM_NO_INLINE BufferTooSmallException(const char* location, uiL_t buffer_size, uiL_t required_size)
        : buffer_size(buffer_size)
        , required_size(required_size)
    {
        this->location = location;
        level = YMC_EXCEPTION_BUFFER_TOO_SMALL;
        code = 1;
        message = "Buffer is too small.";

        description += "    Buffer Size  : ";
        description += std::to_string(buffer_size);
        description += "\n";

        description += "    Required Size: ";
        description += std::to_string(required_size);
        description += "\n";

        str = description.c_str();
    }

    static YM_FORCE_INLINE void check(const char* location, u32_t buffer_size, u32_t required_size){
#if YMC_EXCEPTION_BUFFER_TOO_SMALL > 0
        if (buffer_size >= required_size)
            return;
        fire(location, buffer_size, required_size);
#endif
    }
    static YM_FORCE_INLINE void check(const char* location, u64_t buffer_size, u64_t required_size){
#if YMC_EXCEPTION_BUFFER_TOO_SMALL > 0
        if (buffer_size >= required_size)
            return;
        fire(location, buffer_size, required_size);
#endif
    }

private:
    static YM_NO_INLINE void fire(const char* location, uiL_t buffer_size, uiL_t required_size){
        BufferTooSmallException(location, buffer_size, required_size).ym_exception::fire();
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
