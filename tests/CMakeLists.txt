cmake_minimum_required(VERSION 3.23.1)

project("JSONLib Unit Tests" VERSION 0.1.0 DESCRIPTION "JSON C++ Library Catch Unit Tests" LANGUAGES CXX)

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
    JSONLib_Tests_JSON.cpp
    JSONLib_Tests_JNode.cpp
    JSONLib_Tests_JNodeNumber.cpp
    JSONLib_Tests_ISource.cpp
    JSONLib_Tests_IDestination.cpp
    JSONLib_Tests_Parse_Escapes.cpp
    JSONLib_Tests_Parse_Misc.cpp
    JSONLib_Tests_Parse_Simple.cpp
    JSONLib_Tests_Parse_Collection.cpp
    JSONLib_Tests_Parse_Whitespace.cpp
    JSONLib_Tests_Parse_Exceptions.cpp
    JSONLib_Tests_Stringify_Misc.cpp
    JSONLib_Tests_Stringify_Escapes.cpp
    JSONLib_Tests_Stringify_Simple.cpp
    JSONLib_Tests_Stringify_Collection.cpp
    JSONLib_Tests_Stringify_Exceptions.cpp
    JSONLib_Tests_Misc.cpp
    JSONLib_Tests_Helper.cpp)

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_include_directories(${TEST_EXECUTABLE} PUBLIC ${PROJECT_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${TEST_EXECUTABLE} ${JSON_LIBRARY_NAME} Catch2::Catch2WithMain)

add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})

add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files files)
add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/$<CONFIG>/files)
