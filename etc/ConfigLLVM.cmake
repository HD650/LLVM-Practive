# specify the llvm installation dir
set(LLVM_INSTALL_DIR 
  "/lib/llvm-15" CACHE PATH 
  "LLVM installation directory")

# specify the llvm ir quick test file
set(DEBUG_MODE_TEST_FILE 
  "/lib/llvm-15/llvm-test/test.ll" PATH 
  "The llvm ir input files for quick test this pass")

# location of LLVMConfig.cmake
list(APPEND CMAKE_PREFIX_PATH "${LLVM_INSTALL_DIR}/lib/cmake/llvm/")

# find package of llvm so include dir, lib dir are all defined
find_package(LLVM 15 REQUIRED CONFIG)
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

# don't build with rtti flag to match LLVM, otherwise some symbols can not be resolved
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()