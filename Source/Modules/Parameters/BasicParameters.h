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
#ifndef ymp_BasicParameters_H
#define ymp_BasicParameters_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
    class LookupTable;
    class Parallelizer;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct BasicParameters{
    //  Statics
    const LookupTable& m_tw;
    Parallelizer& m_parallelizer;

    upL_t m_tds = 1;        //  Task Decomposition
    void* m_M = nullptr;    //  Scratch Memory
    upL_t m_ML = 0;         //  Scratch Memory (Size in bytes)

    BasicParameters(
        const LookupTable& tw,
        Parallelizer& parallelizer,
        upL_t tds
    )
        : m_tw(tw)
        , m_parallelizer(parallelizer)
        , m_tds(tds)
        , m_M(nullptr)
        , m_ML((upL_t)0 - 1)
    {}
    BasicParameters(
        const LookupTable& tw,
        Parallelizer& parallelizer,
        upL_t tds,
        void* M, upL_t ML
    )
        : m_tw(tw)
        , m_parallelizer(parallelizer)
        , m_tds(tds)
        , m_M(M)
        , m_ML(ML)
    {}

    BasicParameters edit_tds(upL_t tds) const{
        BasicParameters out = *this;
        out.m_tds = tds;
        return out;
    }
    BasicParameters edit_mem(void* M, upL_t ML) const{
        BasicParameters out = *this;
        out.m_M = M;
        out.m_ML = ML;
        return out;
    }

    //  Splits this set of parameters into two roughly equal parts.
    //  This is useful for recursive thread-splitting.
    void split(BasicParameters& A, BasicParameters& B) const;

    void operator=(const BasicParameters&) = delete;
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
