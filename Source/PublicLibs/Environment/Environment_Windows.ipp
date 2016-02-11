/* Environment_Windows.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/04/2015
 * Last Modified    : 01/04/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <Windows.h>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "Environment.h"
namespace ymp{
namespace Environment{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LowerProcessPriority(){
    SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
}
bool LockToCore(upL_t core){
    DWORD_PTR mask = 0;

    if (core == (upL_t)0 - 1){
        DWORD_PTR before_mask;
        GetProcessAffinityMask(GetCurrentProcess(), &before_mask, &mask);

        //  Select largest core #.
#if _WIN64
        mask = 0x8000000000000000;
#else
        mask = 0x80000000;
#endif
        while ((mask & before_mask) == 0)
            mask >>= 1;

    }else if (core >= 64){
        Console::Warning("Unable to set thread affinity.\n");
        return false;

    }else{
        mask = (DWORD_PTR)1 << core;
    }

    if (SetThreadAffinityMask(GetCurrentThread(), mask) == 0){
        Console::Warning("Unable to set thread affinity.\n");
        return false;
    }
    return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
upL_t GetLogicalProcessorsLegacy(){
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (upL_t)info.dwNumberOfProcessors;
}
#if _WIN32_WINNT >= 0x0601
upL_t GetLogicalProcessorsWin7(){
    upL_t out = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
    return out == 0 ? 1 : out;
}
#endif
upL_t GetLogicalProcessors(){
#if _WIN32_WINNT >= 0x0601
    return GetLogicalProcessorsWin7();
#else
    return GetLogicalProcessorsLegacy();
#endif
}
upL_t GetFreePhysicalMemory(){
    uiL_t bytes;

    MEMORYSTATUSEX data;
    data.dwLength = sizeof(data);
    if (GlobalMemoryStatusEx(&data))
        bytes = (uiL_t)data.ullAvailPhys;
    else
        bytes = 0;

    if (bytes > MAX_MEMORY)
        bytes = MAX_MEMORY;
    return static_cast<upL_t>(bytes);
}
uiL_t GetTotalPhysicalMemory(){
    MEMORYSTATUSEX data;
    data.dwLength = sizeof(data);
    if (GlobalMemoryStatusEx(&data))
        return (uiL_t)data.ullTotalPhys;
    else
        return 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
u64_t x86_rdtsc(){
    return __rdtsc();
}
void x86_cpuid(u32_t eabcdx[4], u32_t eax, u32_t ecx){
    int out[4];
    __cpuidex(out, eax, ecx);
    eabcdx[0] = out[0];
    eabcdx[1] = out[1];
    eabcdx[2] = out[2];
    eabcdx[3] = out[3];
}
uiL_t GetProcessorFrequency(){
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    DWORD_PTR before_mask;
    DWORD_PTR t;
    GetProcessAffinityMask(process, &before_mask, &t);

    t = 1;
    while ((t & before_mask) == 0)
        t <<= 1;

    if (SetThreadAffinityMask(thread, t) == 0){
        Console::Warning("Unable to set Affinity Mask.");
        Console::Quit(1);
    }

    LARGE_INTEGER frequency;
    if (!QueryPerformanceFrequency(&frequency)){
        Console::Warning("Unable to measure clock speed.");
        Console::Quit(1);
    }
    u64_t freq = frequency.QuadPart;
    freq >>= 4;


    u64_t start_cycles = __rdtsc();

    LARGE_INTEGER start_timer;
    if (!QueryPerformanceCounter(&start_timer)){
        Console::Warning("Unable to measure clock speed.");
        Console::Quit(1);
    }
    LARGE_INTEGER current_timer;
    do {
        if (!QueryPerformanceCounter(&current_timer)){
            Console::Warning("Unable to measure clock speed.");
            Console::Quit(1);
        }
    }while ((u64_t)current_timer.QuadPart - (u64_t)start_timer.QuadPart < freq);

    u64_t end_cycles = __rdtsc();

    if (SetThreadAffinityMask(thread, before_mask) == 0){
        Console::Warning("Unable to set Affinity Mask.");
        Console::Quit(1);
    }

    double cycle_dif = (double)(end_cycles - start_cycles);
    double timer_dif = (double)((u64_t)current_timer.QuadPart - (u64_t)start_timer.QuadPart);

    return (u64_t)(cycle_dif / timer_dif * frequency.QuadPart);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
