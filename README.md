# Number Factory

**Work in Progress... No ETA yet.**

Number Factory is a collection of mini-programs that can compute various numbers to extremely large sizes.<br>
It's essentially a souped up version of [Mini-Pi](https://github.com/Mysticial/Mini-Pi) that is powered by the same backend as [y-cruncher](http://www.numberworld.org/y-cruncher/).

Examples include:
 - Large integer power
 - Factorial and Binomial Coefficient
 - Square Root
 - Logarithms
 - e - Binary Splitting of exp(1).
 - Pi - Chudnovsky, Ramanujan, AGM, Machin-like formulas.
 - Catalan's Constant - Lupas, Huvent formulas
 - Zeta(3), Zeta(5) - various formulas
 - Lemniscate, Gamma(1/4)


Number Factory's current goals are:
 - Test the feasibility of maintaining a public interface to the y-cruncher's internals.
 - Serve as a research infrastructure for the y-cruncher project.
 - Show-and-tell some basic, but efficient and parallel implementations of various constants and functions.


Number Factory uses the YMP library which is an experimental public interface to y-cruncher's internals.


-----

**Performance:**

Number Factory has access to parallel large arithmetic whereas most other programs do not.
For this reason alone, it is often faster than other programs by sheer force of parallelization.

As far as sequential performance goes, Number Factory sits somewhere between Mini-Pi and y-cruncher.
y-cruncher is still faster since it is better optimized and has access to internal functionality that is
not exposed through the YMP interface.


-----

**Requirements:**


Compilation:
 - Visual Studio 2015

A Visual Studio project has been provided with various configurations that target different processors. It should be fairly straight-forward to compile and run the app through the IDE. (Though it will be faster to run from outside the IDE since it bypasses the debug heap.)

The DLL interface with the YMP library is "supposed to" be portable across different compilers. But this is untested.
Number Factory is the first program to *dynamically* link with YMP. (y-cruncher *statically* links.)


Run-Time:
 - 64-bit Windows 7 SP1 or later
 - A processor with AVX.

Number Factory itself can be compiled for older processors. But for all practical purposes, the underlying YMP library needs a *minimum* of AVX to avoid embarrassing itself. For that matter, you *really* want to use XOP or AVX2.

There is currently no support for Linux yet. The Number Factory/YMP project is still experimental and will be tested in Windows first. Only if it is massively successful will a Linux version be considered.

-----

**Configurations:**

Number Factory currently supports the following build configurations:

|Configuration |Target Processor  |YMP Binary|Cilk Plus Support|Notes                      |
|--------------|------------------|----------|-----------------|---------------------------|
|Debug         |                  |AVX       |Yes              |No Optimizations           |
|Release       |                  |AVX       |Yes              |Same as "11-SandyBridge"   |
|11-SandyBridge|Intel Sandy Bridge|AVX       |Yes              |                           |
|11-Bulldozer  |AMD Bulldozer     |FMA4/XOP  |No*              |                           |
|13-Haswell    |Intel Haswell     |FMA3/AVX2 |Yes              |                           |

Upon building a configuration through the IDE, the appropriate DLLs are copied into the same directory as the output binary. They will be needed to run the binary.

All `ymp.dll` binaries of the same YMP release will have the same ABI. So any Number Factory binary (or any other client app) will be able to link with any of them. This may make it slightly easier to do CPU dispatching.

However, `ymp.dll` binaries from *different* releases are not guaranteed to be compatible. Backwards incompatible changes may require recompilation of the client app. Maintaining a static ABI is currently not a feasible commitment.

*Different versions of YMP are compiled with different compilers. Not all of them support Cilk Plus. Those without Cilk Plus will use the Windows Thread Pool instead which is still very efficient.

-----

**Licensing:**

 - The source code for Number Factory is opened-sourced and licensed under BSD-3.
 - The binary `cilkrts20.dll` is Intel's Cilk Run-Time library. It is licensed under BSD-3.
 - The binary `ymp.dll` is the YMP library. It is free for non-commercial use only.
