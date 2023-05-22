# use 3.15 for MSVC_RUNTIME_LIBRARY support
cmake_minimum_required(VERSION 3.15)
project(llvm-practice-dominance)

# target name and pass name
set(TARGET_NAME DominanceAnalysis)
set(PASS_NAME TestingPass)

# include the basic build script
include(../etc/ConfigLLVM.cmake)

# compile the cpp to produce a dynamic link lib
set(SOURCES
  TestingPass.cpp
  IterativeDominanceAnalysis.cpp)
set(HEADERS
  IDominanceAnalysis.h
  IterativeDominanceAnalysis.h)
add_library(${TARGET_NAME} SHARED ${SOURCES} ${HEADERS})

# generate a debug target
include(../etc/DebugTarget.cmake)