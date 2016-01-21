/* Environment.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/04/2015
 * Last Modified    : 01/01/2016
 * 
 */

#ifdef _WIN32
#include "Environment_Windows.ipp"
#endif
#ifdef __linux
#include "Environment_Linux.ipp"
#endif
