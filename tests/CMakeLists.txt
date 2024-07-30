cmake_minimum_required(VERSION 3.18.1)

project("JSON_Lib Unit Tests" VERSION 1.1.0 DESCRIPTION "JSON C++ Library Catch Unit Tests" LANGUAGES CXX)

# Get Catch2 from repository
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)

FetchContent_MakeAvailable(Catch2)

# Enable unit tests
enable_testing()

set(TEST_EXECUTABLE ${JSON_LIBRARY_NAME}_Unit_Tests)

set(TEST_SOURCES
        source/jnode/JSON_Lib_Tests_JNode_Assignment.cpp
        source/jnode/JSON_Lib_Tests_JNode_Construct.cpp
        source/jnode/JSON_Lib_Tests_JNode_Indexing.cpp
        source/jnode/JSON_Lib_Tests_JNode_Reference.cpp
        source/json/JSON_Lib_Tests_JSON_Create_Complex.cpp
        source/json/JSON_Lib_Tests_JSON_Create_Array.cpp
        source/json/JSON_Lib_Tests_JSON_Create_Simple.cpp
        source/io/JSON_Lib_Tests_ISource_Buffer.cpp
        source/io/JSON_Lib_Tests_ISource_File.cpp
        source/io/JSON_Lib_Tests_IDestination_Buffer.cpp
        source/io/JSON_Lib_Tests_IDestination_File.cpp
        source/io/JSON_Lib_Tests_File_GetFormat.cpp
        source/io/JSON_Lib_Tests_File_FromFile.cpp
        source/io/JSON_Lib_Tests_File_ToFile.cpp
        source/number/JSON_Lib_Tests_JNode_Number_Assignment.cpp
        source/number/JSON_Lib_Tests_JNode_Number_Conversion.cpp
        source/number/JSON_Lib_Tests_JNode_Number_Misc.cpp
        source/number/JSON_Lib_Tests_JNode_Number_Precision.cpp
        source/number/JSON_Lib_Tests_JNode_Number_Notation.cpp             
        source/parse/JSON_Lib_Tests_Parse_Escapes.cpp
        source/parse/JSON_Lib_Tests_Parse_Misc.cpp
        source/parse/JSON_Lib_Tests_Parse_Simple.cpp
        source/parse/JSON_Lib_Tests_Parse_Collection.cpp
        source/parse/JSON_Lib_Tests_Parse_Whitespace.cpp
        source/parse/JSON_Lib_Tests_Parse_Exceptions.cpp
        source/stringify/JSON_Lib_Tests_Stringify_Misc.cpp
        source/stringify/JSON_Lib_Tests_Stringify_Escapes.cpp
        source/stringify/JSON_Lib_Tests_Stringify_Simple.cpp
        source/stringify/JSON_Lib_Tests_Stringify_Collection.cpp
        source/stringify/JSON_Lib_Tests_Stringify_Exceptions.cpp
        source/stringify/JSON_Lib_Tests_Stringify_Bencode.cpp
        source/stringify/JSON_Lib_Tests_Stringify_XML.cpp
        source/stringify/JSON_Lib_Tests_Print.cpp
        source/misc/JSON_Lib_Tests_Misc.cpp
        source/misc/JSON_Lib_Tests_Helper.cpp
        source/traverse/JSON_Lib_Tests_Traverse.cpp)

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_include_directories(${TEST_EXECUTABLE} PUBLIC ${PROJECT_CURRENT_SOURCE_DIR}/include ./include)
target_link_libraries(${TEST_EXECUTABLE} ${JSON_LIBRARY_NAME} Catch2::Catch2WithMain)

add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})

add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files files)
add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/files)
