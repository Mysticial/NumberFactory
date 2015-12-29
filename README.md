# Number Factory

**Work in Progress... No ETA yet.**

The stuff that's been uploaded here is basically complete. All that's missing are the `ymp.dll` binaries which I'm still testing. So you will be able to compile and link. But you won't be able to run yet.

If you're interested in testing this and I know you personally, shoot me an email or drop me a message in Lounge\<C++\>. I'll send you the `ymp.dll` binaries to play with.

To-Do List:
 1. ~~Clean up and upload `PublicLibs` directory.~~
 2. ~~Minimize dependencies on the internal `PrivateLibs` and `Modules` directories.~~
 3. ~~Finalize YMP v1.0 functionality.~~
 4. ~~Clean up and upload the `NumberFactory` app/directory.~~
 5. Pre-release testing of everything.
 6. ~~Run lots of benchmarks.~~
 7. ~~Clean up and upload the YMP headers.~~
 8. Upload YMP binaries.
 9. Finish and release YMP documentation.

-----

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

For most constants, the overall performance comes within a factor of 2 of y-cruncher. This is actually pretty good considering that most of Number Factory's implementations are relatively unoptimized.

That said, squeezing out the remaining factor of 2 will probably be difficult. y-cruncher does some pretty crazy things to get to where it is now. And it uses internal functionality that isn't exposed through the YMP interface.


-----

**Requirements:**


Compilation:
 - Visual Studio 2015

A Visual Studio project has been provided with various configurations that target different processors. It should be fairly straight-forward to compile and run the app through the IDE. (Though it will be faster to run from outside the IDE since it bypasses the debug heap.)

The DLL interface with the YMP library is "supposed to" be portable across different compilers. But this is untested.
Number Factory is the first program to *dynamically* link with YMP. (y-cruncher *statically* links.)


Run-Time:
 - 64-bit Windows Vista or later. (Windows 7 SP1 required for AVX)
 - A processor with SSE4.1. (Intel Penryn or later. Or AMD Bulldozer or later.)

Number Factory itself can be compiled for older processors. But for all practical purposes, the underlying YMP library needs a *minimum* of SSE4.1 to be competitive with existing libraries. That said, the *real* performance gains will require AVX2.

There is currently no support for Linux yet. The Number Factory/YMP project is still experimental and will be tested in Windows first. Only if it is massively successful will a Linux version be considered.

-----

**Configurations:**

The Visual Studio project currently supports the following build configurations:

|Configuration |Target Processor  |YMP Binary|Notes                      |
|--------------|------------------|----------|---------------------------|
|Debug         |                  |SSE4.1    |No Optimizations           |
|Release       |                  |SSE4.1    |Same as "08-Nehalem"       |
|08-Nehalem    |Intel Nehalem     |SSE4.1    |                           |
|11-SandyBridge|Intel Sandy Bridge|AVX       |                           |
|11-Bulldozer  |AMD Bulldozer     |FMA4/XOP  |                           |
|13-Haswell    |Intel Haswell     |FMA3/AVX2 |                           |

Upon building a configuration through the IDE, the appropriate DLLs are copied into the same directory as the output binary. They will be needed to run the binary.

All `ymp.dll` binaries of the same YMP release will have the same ABI. So any Number Factory binary (or any other client app) will be able to link with any of them. This may make it slightly easier to do CPU dispatching.

However, `ymp.dll` binaries from *different* releases are not guaranteed to be compatible. Backwards incompatible changes may require recompilation of the client app. Maintaining a static ABI is currently not a feasible commitment.

-----

**Source Directories:**

 - `NumberFactory/` - The main Number Factory directory.
 - `ymp/` - Headers for the YMP library.
 - `PublicLibs/` - Public shared libraries. Also used by the [Digit Viewer](https://github.com/Mysticial/DigitViewer).
 - `PrivateLibs/`, `Objects/`, and `Modules/` - Internal components. Don't use directly.

-----

**Licensing:**

 - The source code for Number Factory is opened-sourced and licensed under BSD-3.
 - The binary `cilkrts20.dll` is Intel's Cilk Run-Time library. It is licensed under BSD-3.
 - The binary `ymp.dll` is the YMP library. It is free for non-commercial use only.
