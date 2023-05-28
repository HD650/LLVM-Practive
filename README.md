# LLVM_Practice

## Environment set up
Need to install:
1. LLVM 15 (build from source or prebuilt distribution)
2. gdb
3. LLVM Lit
4. cmake


## HelloWorld Pass
**The first pass that simply prints function names of a module. 
Mainly for verifying the build and debug system of future passes.**

HelloWorld builds a pass as `dll/so` shared library that does NOT link any LLVM library. Thus it's a standalone pass not within LLVM build system.
We could call `opt` and load that shared library to run the pass on any LLVM IR code.
Also a `gdb` target is generated support debugging the pass in the shared library.
This pass and its build system is the base of future passes.