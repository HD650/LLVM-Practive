# use 3.15 for MSVC_RUNTIME_LIBRARY support
cmake_minimum_required(VERSION 3.15)
project(llvm-practice-sccp)

# target name and pass name
set(TARGET_NAME Sccp)
set(PASS_NAME TestingPass)

# include the basic build script
include(../etc/ConfigLLVM.cmake)

# specify the llvm ir quick test file
set(DEBUG_MODE_TEST_FILE 
  "/lib/llvm-15/llvm-test/MergeBB_conditional_branch_merged.ll" PATH 
  "The llvm ir input files for quick test this pass")

# compile the cpp to produce a dynamic link lib
set(SOURCES
  TestingPass.cpp
  Sccp.cpp)
set(HEADERS
  Sccp.h)
add_library(${TARGET_NAME} SHARED ${SOURCES} ${HEADERS})

# compile the cpp in the unittest folder to produce a test executable
set(TESTS ./unittest/main.cpp
  ./unittest/tSccp.cpp)

# generate a debug target
include(../etc/DebugTarget.cmake)

# build test and run
include(../etc/Unittest.cmake)