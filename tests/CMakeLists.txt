cmake_minimum_required(VERSION 3.23.1)

project("JSON_Lib Unit Tests" VERSION 0.1.0 DESCRIPTION "JSON C++ Library Catch Unit Tests" LANGUAGES CXX)

# Get Catch2 from repository

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

FetchContent_MakeAvailable(Catch2)

# Enable unit tests

enable_testing()

set(TEST_EXECUTABLE ${JSON_LIBRARY_NAME}_Unit_Tests)

set(TEST_SOURCES
    JSON_Lib_Tests_JSON.cpp
    JSON_Lib_Tests_JNode.cpp
    JSON_Lib_Tests_JNode_Number.cpp
    JSON_Lib_Tests_ISource.cpp
    JSON_Lib_Tests_IDestination.cpp
    JSON_Lib_Tests_Parse_Escapes.cpp
    JSON_Lib_Tests_Parse_Misc.cpp
    JSON_Lib_Tests_Parse_Simple.cpp
    JSON_Lib_Tests_Parse_Collection.cpp
    JSON_Lib_Tests_Parse_Whitespace.cpp
    JSON_Lib_Tests_Parse_Exceptions.cpp
    JSON_Lib_Tests_Stringify_Misc.cpp
    JSON_Lib_Tests_Stringify_Escapes.cpp
    JSON_Lib_Tests_Stringify_Simple.cpp
    JSON_Lib_Tests_Stringify_Collection.cpp
    JSON_Lib_Tests_Stringify_Exceptions.cpp
    JSON_Lib_Tests_Print.cpp
    JSON_Lib_Tests_Misc.cpp
    JSON_Lib_Tests_Helper.cpp)

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_include_directories(${TEST_EXECUTABLE} PUBLIC ${PROJECT_CURRENT_SOURCE_DIR}/include ./include)
target_link_libraries(${TEST_EXECUTABLE} ${JSON_LIBRARY_NAME} Catch2::Catch2WithMain)

add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})

add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files files)
add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/$<CONFIG>/files)
