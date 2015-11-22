/* ymp.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/31/2015
 * Last Modified    : 06/20/2015
 * 
 *      The main header for the entire YMP library.
 * 
 *  This header pulls in everything that most apps will ever need from ymp.
 *  If you don't want to do that, feel free include these headers individually.
 * 
 */

#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exception.h"

#include "Parallelism.h"
#include "LowLevel.h"
#include "BigIntR.h"
#include "BigIntO.h"
#include "BigFloatR.h"
#include "BigFloatO.h"
