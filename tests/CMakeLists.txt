cmake_minimum_required(VERSION 3.18.1)

project("JSON_Lib Unit Tests" VERSION 1.0.0 DESCRIPTION "JSON C++ Library Catch Unit Tests" LANGUAGES CXX)

# Get Catch2 from repository
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

# Enable unit tests
enable_testing()

set(TEST_EXECUTABLE ${JSON_LIBRARY_NAME}_Unit_Tests)

set(TEST_SOURCES
  ./jnode/JSON_Lib_Tests_JNode.cpp
  ./jnode/JSON_Lib_Tests_JNode_Number.cpp
  ./json/JSON_Lib_Tests_JSON.cpp
  ./io/JSON_Lib_Tests_ISource.cpp
  ./io/JSON_Lib_Tests_IDestination.cpp
  ./io/JSON_Lib_Tests_File.cpp
  ./parse/JSON_Lib_Tests_Parse_Escapes.cpp
  ./parse/JSON_Lib_Tests_Parse_Misc.cpp
  ./parse/JSON_Lib_Tests_Parse_Simple.cpp
  ./parse/JSON_Lib_Tests_Parse_Collection.cpp
  ./parse/JSON_Lib_Tests_Parse_Whitespace.cpp
  ./parse/JSON_Lib_Tests_Parse_Exceptions.cpp
  ./stringify/JSON_Lib_Tests_Stringify_Misc.cpp
  ./stringify/JSON_Lib_Tests_Stringify_Escapes.cpp
  ./stringify/JSON_Lib_Tests_Stringify_Simple.cpp
  ./stringify/JSON_Lib_Tests_Stringify_Collection.cpp
  ./stringify/JSON_Lib_Tests_Stringify_Exceptions.cpp
  ./stringify/JSON_Lib_Tests_Stringify_Bencode.cpp
  ./stringify/JSON_Lib_Tests_Stringify_XML.cpp
  ./stringify/JSON_Lib_Tests_Print.cpp
  ./misc/JSON_Lib_Tests_Misc.cpp
  ./misc/JSON_Lib_Tests_Helper.cpp
  ./traverse/JSON_Lib_Tests_Traverse.cpp)

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_include_directories(${TEST_EXECUTABLE} PUBLIC ${PROJECT_CURRENT_SOURCE_DIR}/include ./include)
target_link_libraries(${TEST_EXECUTABLE} ${JSON_LIBRARY_NAME} Catch2::Catch2WithMain)

add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})

add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files files)
add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/files)
