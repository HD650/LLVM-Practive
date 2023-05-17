cmake_minimum_required(VERSION 3.13.4)
project(llvm-tutor-hello-world)

# specify the llvm installation dir
set(LT_LLVM_INSTALL_DIR 
  "C:/Coding/LLVM_INSTALL/llvm_dynamic_release" CACHE PATH 
  "LLVM installation directory")

# location of LLVMConfig.cmake
list(APPEND CMAKE_PREFIX_PATH "${LT_LLVM_INSTALL_DIR}/lib/cmake/llvm/")

# find package of llvm so include dir, lib dir are all defined
find_package(LLVM 16 REQUIRED CONFIG)
message("LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}
  Targets     ${LLVM_TARGETS_TO_BUILD}"
)

# add -D -I -L
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# use c++ 17
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

# compile the cpp to priduce a dynamic link lib
add_library(HelloWorld SHARED HelloWorldPass.cpp)

# links nessesary libs of LLVM
target_link_libraries(HelloWorld
LLVMCore LLVMSupport LLVMAnalysis)