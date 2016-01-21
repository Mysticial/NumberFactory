/* StopWatch.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/12/2015
 * Last Modified    : 02/12/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/Serialization/Serialization.h"
#include "PublicLibs/Environment/Environment.h"
#include "StopWatch.h"
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void StopWatch::Reset(){
    total_wall_time = 0;
    total_cpu_time = 0;
    is_running = false;
}
void StopWatch::Start(){
    if (is_running)
        return;
    wall_clock = WallClock::Now();
    cpu_clock = CPUClock();
    is_running = true;
}
void StopWatch::Stop(){
    if (!is_running)
        return;
    total_wall_time += wall_clock.SecondsElapsed();
    total_cpu_time  += CPUClock() - cpu_clock;
    is_running = false;
}
////////////////////////////////////////////////////////////////////////////////
double StopWatch::GetWallTime() const{
    double out_wall = total_wall_time;
    if (is_running){
        out_wall += wall_clock.SecondsElapsed();
    }
    return out_wall;
}
double StopWatch::GetCpuTime() const{
    double out_cpu = total_cpu_time;
    if (is_running){
        out_cpu += CPUClock() - cpu_clock;
    }
    return out_cpu;
}
double StopWatch::GetCpuUtilization() const{
    double wall, cpu;
    wall = GetTimes(cpu);
    return cpu / wall;
}
double StopWatch::GetTimes(double& cpu_time) const{
    double out_wall = total_wall_time;
    double out_cpu = total_cpu_time;
    if (is_running){
        out_wall += wall_clock.SecondsElapsed();
        out_cpu += CPUClock() - cpu_clock;
    }
    cpu_time = out_cpu;
    return out_wall;
}
////////////////////////////////////////////////////////////////////////////////
void StopWatch::PrintCpuUtilization() const{
    double utilization = 100. * GetCpuUtilization();
    Console::print("CPU Utilization:        ", 'w');
    if (utilization > 0){
        Console::print_float(utilization, 6, 'Y');
        Console::println(" %");
        Console::print("Multi-core Efficiency:  ", 'w');
        Console::print_float(utilization / Environment::GetLogicalProcessors(), 6, 'Y');
        Console::print(" %");
    }else{
        Console::println("Too Fast to Measure", 'Y');
        Console::SetColor('w');
        Console::print("Multi-core Efficiency:  ", 'w');
        Console::print("Too Fast to Measure", 'Y');
    }
    Console::println("\n", 'w');
}
////////////////////////////////////////////////////////////////////////////////
void StopWatch::Serialize(std::string& stream) const{
    double wall, cpu;
    wall = GetTimes(cpu);
    Serialization::write_float(stream, "total_wall_time   :", wall);
    Serialization::write_float(stream, "total_cpu_time    :", cpu);
}
void StopWatch::Deserialize(const char*& stream){
    total_wall_time = Serialization::parse_float(stream);
    total_cpu_time = Serialization::parse_float(stream);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
