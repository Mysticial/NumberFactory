/* BasicParametersOwner.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/25/2014
 * Last Modified    : 06/14/2015
 * 
 */

#pragma once
#ifndef _ymp_BasicParametersOwner_H
#define _ymp_BasicParametersOwner_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/AlignedMalloc.h"
#include "BasicParameters.h"
#include "GlobalLookupTable.h"
#include "GlobalParallelFramework.h"
namespace ymp{
    class ParallelFramework;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicParametersO : public BasicParameters{
    //  Small Buffer optimization.
    static const upL_t SMALL_BUFFER_SIZE = 256;
    YM_ALIGN(64) char small_buffer[256];

    //  Smart pointer for larger buffers.
    SmartPointer<>::type uptr;

public:
    BasicParametersO(
        const LookupTable& tw,
        Parallelizer& parallelizer,
        upL_t tds,
        upL_t ML
    )
        : BasicParameters(tw, parallelizer, tds, nullptr, ML)
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
    BasicParametersO(
        const LookupTable& tw,
        ParallelFramework& framework,
        upL_t tds,
        upL_t ML
    )
        : BasicParametersO(tw, (Parallelizer&)framework, tds, ML)
    {}
    BasicParametersO(const LookupTable& tw, upL_t tds, upL_t ML)
        : BasicParametersO(tw, Parallelism::get_global_framework(), tds, ML)
    {}
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
