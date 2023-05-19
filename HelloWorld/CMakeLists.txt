# use 3.15 for MSVC_RUNTIME_LIBRARY support
cmake_minimum_required(VERSION 3.15)
project(llvm-tutor-hello-world)

# specify the llvm installation dir
set(LT_LLVM_INSTALL_DIR 
  "/lib/llvm-15" CACHE PATH 
  "LLVM installation directory")

# specify the llvm ir quick test file
set(DEBUG_MODE_TEST_FILE 
  "/lib/llvm-15/llvm-test/test.ll" CACHE PATH 
  "The llvm ir input files for quick test this pass")

# location of LLVMConfig.cmake
list(APPEND CMAKE_PREFIX_PATH "${LT_LLVM_INSTALL_DIR}/lib/cmake/llvm/")

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

# compile the cpp to priduce a dynamic link lib
add_library(HelloWorld SHARED HelloWorldPass.cpp)

# in debug mode, generate a debug target that run opt under gdb, load out pass
# and run a simply llir input
if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_custom_target(debug
                  DEPENDS HelloWorld
		  COMMAND gdb "${LT_LLVM_INSTALL_DIR}/bin/opt" --ex "set args -load-pass-plugin=${CMAKE_BINARY_DIR}/$<TARGET_FILE_NAME:HelloWorld> -passes=\"HelloWorld\" ${DEBUG_MODE_TEST_FILE}"
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
endif()

# everything below is for Window build, while it's hard to build a standalone dll
# pass in Window and run it since vc++ does not allow unresolved symbols in dll.
# but if we link llvm lib into our dll pass, many symbols will be defined twice.
# there are workaround to let vc++ ignore unresolved symbols but pretty trival.

# do not links any libs of LLVM otherwise when loading our built pass
# many symbols will be defined twice (one in llvm-project one in our project)
#target_link_libraries(HelloWorld LLVMCore LLVMSupport LLVMAnalysis)

# link libcmt.lib instead of msvcrt.lib, so we could build our pass in debug mode
if(MSVC)
    # possible value:
    # MultiThreadedDebugDLL: if LLVM links a debug and dll version of c runtime
    # MultiThreaded/MultiThreadedDLL: if LLVM links a release version of c runtime
    set_property(TARGET HelloWorld PROPERTY
      MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    # set uo debugging command
      # set_target_properties(target_name PROPERTIES
      # VS_DEBUGGER_COMMAND "Path_to_your_executable"
      # VS_DEBUGGER_COMMAND_ARGUMENTS "arg1 arg2 arg3")
endif()
