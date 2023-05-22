# use 3.15 for MSVC_RUNTIME_LIBRARY support
cmake_minimum_required(VERSION 3.15)
project(llvm-practice-hello-world)

# target name and pass name
set(TARGET_NAME HelloWorld)
set(PASS_NAME HelloWorld)

# include the basic build script
include(../etc/ConfigLLVM.cmake)

# compile the cpp to priduce a dynamic link lib
add_library(HelloWorld SHARED HelloWorldPass.cpp)

# generate a debug target
include(../etc/DebugTarget.cmake)
