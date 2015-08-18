# Number Factory

**Work in Progress... No ETA yet.**

Number Factory is a collection of mini-programs that can compute various numbers to extremely large sizes.
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
y-cruncher is still faster since is better optimized and has access to functionality that is
not exposed throughput the YMP interface.


-----

**Requirements:**


Compilation:
 - Visual Studio 2015

A Visual Studio project has been provided. It is completely setup with options to compile and run for both Sandy Bridge
and Haswell through the IDE.

The DLL interface with the YMP library is "supposed to" be portable across different compilers. But this is untested.
Number Factory is the first program to *dynamically* link with YMP. (y-cruncher *statically* links.)


Run-Time:
 - 64-bit Windows 7 or later
 - A Sandy Bridge processor or later with AVX. (Users with AVX2 will get a significant performance boost.)

Number Factory itself can be compiled for a broader range of targets.
But the YMP library is currently only available as a 64-bit Windows binary with AVX or AVX2.


-----

**Licensing:**

 - The source code for Number Factory is opened-sourced and licensed under BSD-3.
 - The binary `cilkrts20.dll` is Intel's Cilk Run-Time library. It is licensed under BSD-3.
 - The binary `ymp.dll` is the YMP library. It is free for non-commercial use only.
