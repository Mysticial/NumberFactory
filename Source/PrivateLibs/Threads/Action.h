/* Action.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/27/2014
 * Last Modified    : 07/08/2015
 * 
 *      BasicAction is a simple action that takes no parameters.
 *      IndexAction is an action that takes a single index parameter.
 * 
 *  However, both of these actions have vtables which isn't safe to export
 *  across the DLL boundary. Therefore, there are wrapper classes for this
 *  purpose.
 * 
 *  The wrapper classes, "dll_BasicAction" and "dll_IndexAction" are POD types
 *  which are safe to pass across the DLL boundary.
 * 
 *  Users of this class will never have to care about these wrapper classes.
 *      -   The wrapping is abstracted away in TaskParallelizer.h.
 *      -   And the unwrapping is done in TaskParallelizer.cpp.
 * 
 *  Therefore, users of this class will have the same interface on both sides
 *  of the DLL boundary.
 * 
 */

#pragma once
#ifndef _ymp_Action_H
#define _ymp_Action_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
namespace Threads{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class IndexAction{
public:
    virtual void run(upL_t index) = 0;

    IndexAction(){}
    IndexAction(const IndexAction&) = delete;
    IndexAction(IndexAction&&) = delete;
    void operator=(const IndexAction&) = delete;
    void operator=(IndexAction&&) = delete;
};
////////////////////////////////////////////////////////////////////////////////
class BasicAction : public IndexAction{
public:
    virtual void run() = 0;

private:
    virtual void run(upL_t) override final{ run(); }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  DLL Export Hell
struct dll_BasicAction{
    typedef void (*fp)(BasicAction* action);
    void operator=(dll_BasicAction&) = delete;

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
    void operator=(dll_IndexAction&) = delete;

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
}
}
#endif
