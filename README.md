# LLVM_Practice
 
## HelloWorld Pass
**The first pass that simply prints function names of a module. 
Mainly for verifying the build and debug system of future passes.**
HelloWorld builds a pass as `dll/so` shared library that does NOT link any LLVM library. Thus it's a standalone pass not within LLVM build system.
We could call `opt` and load that shared library to run the pass on any LLVM IR code.
Also a `gdb` target is generated support debugging the pass in the shared library.
This pass and its build system is the base of future passes.