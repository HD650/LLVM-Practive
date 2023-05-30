project(llvm-practice-unittest)
enable_testing()

# load gtest
# specify the gtest installation dir
set(GTest_INSTALL_DIR 
    "/usr/src/gtest" CACHE PATH 
  "GTest installation directory")

# location of LLVMConfig.cmake
list(APPEND CMAKE_PREFIX_PATH "${GTest_INSTALL_DIR}")

find_package(GTest REQUIRED)

message("GTest STATUS:
  Definitions ${GTEST_DEFINITIONS}
  Includes    ${GTEST_INCLUDE_DIRS}
  Libraries   ${GTEST_LIBRARY_DIRS}
  Targets     ${GTEST_TARGETS_TO_BUILD}"
)

# include gtest headers
include_directories(${GTEST_INCLUDE_DIRS})


# compile the test to an exeutable
add_executable(UnitTest ${TESTS})

# link the pass we implemented and google tests framework and the llvm
llvm_map_components_to_libnames(LLVM_LIBS support core irreader)
target_link_libraries(UnitTest ${GTEST_LIBRARIES} ${TARGET_NAME} pthread ${LLVM_LIBS})
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# define a target in the ctest
add_test(NAME unit_tests COMMAND UnitTest)

# let make invoke ctest automatically so we have a test right after build
add_custom_target(run_tests ALL
                  COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS UnitTest)