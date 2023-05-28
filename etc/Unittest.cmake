enable_testing()

# this should be defined in the make cmake file
file(GLOB TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

add_executable(tests ${TEST_SRCS})
target_link_libraries(tests gtest_main gtest)

add_test(NAME unit_tests COMMAND tests)