/* Action.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/27/2014
 * Last Modified    : 07/08/2015
 * 
 *  The BasicAction and IndexAction classes have vtables which aren't safe to
  * export across the DLL boundary. Here are wrapper classes to make it safe.
 * 
 *  The wrapper classes, "dll_BasicAction" and "dll_IndexAction" are POD types
 *  which are safe to pass across the DLL boundary.
 * 
 *  The "run_in_parallel()" functions will automatically handle the wrapping
 *  and unwrapping. So the user doesn't need to deal with that. The interface
 *  is the same on both sides of the DLL barrier.
 * 
 */

#pragma once
#ifndef ymp_ExportSafeLibs_Action_H
#define ymp_ExportSafeLibs_Action_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Action.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Wrap them on the client side.
struct dll_BasicAction{
    using fp = void (*)(BasicAction* action);
    void operator=(const dll_BasicAction&) = delete;

    dll_BasicAction(BasicAction& action)
        : m_fp_run(internal_run)
        , m_action(&action)
    {}

    void run(){
        m_fp_run(m_action);
    }

private:
    fp m_fp_run;
    BasicAction* m_action;

    static void internal_run(BasicAction* action){
        action->run();
    }
};
struct dll_IndexAction{
    using fp = void (*)(IndexAction* action, upL_t index);
    void operator=(const dll_IndexAction&) = delete;

    dll_IndexAction(IndexAction& action)
        : m_fp_run(internal_run)
        , m_action(&action)
    {}

    void run(upL_t index){
        m_fp_run(m_action, index);
    }

private:
    fp m_fp_run;
    IndexAction* m_action;

    static void internal_run(IndexAction* action, upL_t index){
        action->run(index);
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Unwrap them on the DLL side.
class dll_BasicActionWrapper : public BasicAction{
    dll_BasicAction& m_action;

public:
    dll_BasicActionWrapper(dll_BasicAction& action)
        : m_action(action)
    {}

    virtual void run() override{
        m_action.run();
    }
};
class dll_IndexActionWrapper : public IndexAction{
    dll_IndexAction& m_action;

public:
    dll_IndexActionWrapper(dll_IndexAction& action)
        : m_action(action)
    {}

    virtual void run(upL_t index) override{
        m_action.run(index);
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
