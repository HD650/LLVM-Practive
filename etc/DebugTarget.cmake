# in debug mode, generate a debug target that run opt under gdb, load out pass
# and run a simply llir input
if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_custom_target(debug
                  DEPENDS ${TARGET_NAME}
		  COMMAND gdb "${LLVM_INSTALL_DIR}/bin/opt" --ex "set args -load-pass-plugin=${CMAKE_BINARY_DIR}/$<TARGET_FILE_NAME:${TARGET_NAME}> -passes=\"${PASS_NAME}\" ${DEBUG_MODE_TEST_FILE}"
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
    set_property(TARGET ${TARGET_NAME} PROPERTY
      MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    # set uo debugging command
      # set_target_properties(target_name PROPERTIES
      # VS_DEBUGGER_COMMAND "Path_to_your_executable"
      # VS_DEBUGGER_COMMAND_ARGUMENTS "arg1 arg2 arg3")
endif()