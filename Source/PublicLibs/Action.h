/* Action.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/27/2014
 * Last Modified    : 02/05/2016
 * 
 *      BasicAction is a simple action that takes no parameters.
 *      IndexAction is an action that takes a single index parameter.
 *      ResourceAction takes either a range of indices or a range of resources.
 * 
 * 
 *  BasicAction is the most basic action. It takes no parameters and is mostly
 *  used by binary divide-and-conquer recursions.
 * 
 *  IndexAction takes an index parameter and is most suitable for parallel
 *  for-each style loops.
 * 
 *  ResourceAction is similar to an IndexAction, but each action requires
 *  additional resources (such as scratch memory) to run.
 * 
 */

#pragma once
#ifndef ymp_Action_H
#define ymp_Action_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class IndexAction{
public:
    virtual void run(upL_t index) = 0;

    IndexAction() = default;
    IndexAction(const IndexAction&) = delete;
    void operator=(const IndexAction&) = delete;
};
////////////////////////////////////////////////////////////////////////////////
class BasicAction : public IndexAction{
public:
    virtual void run() = 0;
private:
    virtual void run(upL_t) override final{ run(); }
};
////////////////////////////////////////////////////////////////////////////////
class ResourceAction{
public:
    //  Run the action for all indices in the range, [work_s, work_e) using a
    //  single resource at index, "resource_index".
    virtual void run_multi_workunit(upL_t work_s, upL_t work_e, upL_t resource_index) = 0;

    //  Run the action for a single index at, "work_index" using multiple
    //  resources in the range, [resource_s, resource_e).
    virtual void run_multi_resource(upL_t work_index, upL_t resource_s, upL_t resource_e) = 0;

    ResourceAction() = default;
    ResourceAction(const ResourceAction&) = delete;
    void operator=(const ResourceAction&) = delete;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
