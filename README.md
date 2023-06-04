# LLVM_Practice

## Environment set up
Need to install:
1. LLVM 15 (build from source or prebuilt distribution)
2. GDB
3. LLVM Lit
4. Cmake
5. Google test


## HelloWorld Pass
**The first pass that simply prints function names of a module. 
Mainly for verifying the build and debug system of future passes.**

HelloWorld builds a pass as `dll/so` shared library that does NOT link any LLVM library. Thus it's a standalone pass not within LLVM build system.
We could call `opt` and load that shared library to run the pass on any LLVM IR code.
Also a `gdb` target is generated support debugging the pass in the shared library.
This pass and its build system is the base of future passes.

## Dominance Analysis (WIP)
** A function analysis that compute the dominator information. Dominator info is used extensively in compiler optimization. The unittest system is implemented in this analysis to check the correctness of the analysis.**

DominanceAnalysis builds a pass as shared library that invokes an analysis. The analysis library contains:
1. implementation of the Simple Fast Dominator Algorithm and the Lengauer-Tarjan Algorithm (WIP).
2. implementation of a dominator tree that can retrieve dominance information efficiently.
3. a gtest suit that checks the correctness of the implementation of the analysis.