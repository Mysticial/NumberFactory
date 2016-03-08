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
#ifndef _ymp_ExportSafeLibs_Action_H
#define _ymp_ExportSafeLibs_Action_H
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
    typedef void (*fp)(BasicAction* action);
    void operator=(const dll_BasicAction&) = delete;

    dll_BasicAction(BasicAction& action)
        : fp_run(internal_run)
        , action(&action)
    {}

    void run(){
        fp_run(action);
    }

private:
    fp fp_run;
    BasicAction* action;

    static void internal_run(BasicAction* action){
        action->run();
    }
};
struct dll_IndexAction{
    typedef void (*fp)(IndexAction* action, upL_t index);
    void operator=(const dll_IndexAction&) = delete;

    dll_IndexAction(IndexAction& action)
        : fp_run(internal_run)
        , action(&action)
    {}

    void run(upL_t index){
        fp_run(action, index);
    }

private:
    fp fp_run;
    IndexAction* action;

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
    dll_BasicAction& action;

public:
    dll_BasicActionWrapper(dll_BasicAction& action)
        : action(action)
    {}

    virtual void run() override{
        action.run();
    }
};
class dll_IndexActionWrapper : public IndexAction{
    dll_IndexAction& action;

public:
    dll_IndexActionWrapper(dll_IndexAction& action)
        : action(action)
    {}

    virtual void run(upL_t index) override{
        action.run(index);
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
