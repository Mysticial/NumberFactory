/* Exceptions.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/31/2015
 * Last Modified    : 05/31/2015
 * 
 */

#pragma once
#ifndef ymp_ExportSafeLibs_Exceptions_H
#define ymp_ExportSafeLibs_Exceptions_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "PublicLibs/CompilerSettings.h"
#include "Pointer.h"
#include "String.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_STANDALONE
#define YMP_COLLAPSE_EXCEPTION(expression)  {expression;} return nullptr
#define YMP_EXPAND_EXCEPTION(expression)    expression
#else
#define YMP_COLLAPSE_EXCEPTION(expression)  \
    try{    \
        expression; \
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

    ExceptionWrapper(Type type, const char* str, void (*deleter)(ExceptionWrapper*) = &local_ptr_deleter)
        : m_type(type)
        , m_deleter(deleter)
        , m_data(str)
    {}
    void Rethrow(){
        dll_uptr<ExceptionWrapper> uptr(this, m_deleter);
        throw std::string(m_data);
    }

private:
    Type m_type;
    void (*m_deleter)(ExceptionWrapper*);
    dll_astr m_data;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
