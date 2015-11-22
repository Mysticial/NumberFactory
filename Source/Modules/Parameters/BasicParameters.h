/* BasicParameters.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/25/2014
 * Last Modified    : 06/14/2015
 * 
 *      This holds a set of parameters that is used by almost every single
 *  large multiply function. Furthermore, these tend to stay the same. So they
 *  can be grouped together and passed as an aggregrate.
 * 
 */

#pragma once
#ifndef _ymp_BasicParameters_H
#define _ymp_BasicParameters_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/AlignedMalloc.h"
namespace ymp{
    class LookupTable;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct BasicParameters{
    const LookupTable* tw;  //  Twiddle-Factor Tables
    void* M;                //  Scratch Memory
    upL_t ML;               //  Scratch Memory size (in bytes)
    upL_t tds;              //  Task Decomposition

    BasicParameters(){}
    BasicParameters(const LookupTable* tw, upL_t tds = 1)
        : tw(tw)
        , M(nullptr)
        , ML((upL_t)0 - 1)
        , tds(tds)
    {}
    BasicParameters(const LookupTable* tw, void* M, upL_t ML, upL_t tds = 1)
        : tw(tw)
        , M(M)
        , ML(ML)
        , tds(tds)
    {}

    BasicParameters edit_tds(upL_t tds) const{
        return BasicParameters(tw, M, ML, tds);
    }

    //  Splits this set of parameters into two roughly equal parts.
    //  This is useful for recursive thread-splitting.
    void split(BasicParameters& A, BasicParameters& B) const;
};
class BasicParametersO : public BasicParameters{
    //  Small Buffer optimization.
    static const upL_t SMALL_BUFFER_SIZE = 256;
    YM_ALIGN(64) char small_buffer[256];

    //  Smart pointer for larger buffers.
    SmartPointer<>::type uptr;

public:
    BasicParametersO(const LookupTable* tw, upL_t ML, upL_t tds = 1)
        : BasicParameters(tw, nullptr, ML, tds)
    {
        if (ML <= SMALL_BUFFER_SIZE){
            M = small_buffer;
        }else{
            uptr = SmartPointer<>::malloc_uptr(ML, DEFAULT_ALIGNMENT);
            M = uptr.get();
            if (M == nullptr)
                throw std::bad_alloc();
        }
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  DLL Export Hell
extern "C" {
    YMP_EXPORT void BasicParameters_split(const BasicParameters& T, BasicParameters& A, BasicParameters& B);
}
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_IMPORT
inline void BasicParameters::split(BasicParameters& A, BasicParameters& B) const{
    BasicParameters_split(*this, A, B);
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
