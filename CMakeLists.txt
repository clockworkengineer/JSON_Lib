cmake_minimum_required(VERSION 3.18.1)

project("JSON_Lib" VERSION 1.1.0 DESCRIPTION "JSON C++ Library" LANGUAGES CXX)

# C++ options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler options
if(MSVC)
  add_compile_options(/W4)
  set(PLATFORM "windows")
else()
  add_compile_options(-Wall -Werror -pedantic)
  set(PLATFORM "linux")
endif()

# Setup library version
configure_file(JSON_Config.h.in JSON_Config.hpp)

# Library name
set(JSON_LIBRARY_NAME ${PROJECT_NAME})

# JSON sources and includes
set(JSON_SOURCES
  ./classes/source/JSON.cpp
  ./classes/source/implementation/JSON_Impl.cpp
  ./classes/source/implementation/file/JSON_File.cpp
  ./classes/source/implementation/translator/Default_Translator.cpp
  ./classes/source/implementation/parser/Default_Parser.cpp)

set(JSON_INCLUDES
  JSON_Config.hpp
  ./classes/include/JSON.hpp
  ./classes/include/implementation/JSON_Core.hpp
  ./classes/include/implementation/JSON_Impl.hpp
  ./classes/include/implementation/common/JSON_Error.hpp
  ./classes/include/implementation/variants/JSON_Variant.hpp
  ./classes/include/implementation/variants/JSON_Hole.hpp
  ./classes/include/implementation/variants/JSON_Object.hpp
  ./classes/include/implementation/variants/JSON_Array.hpp
  ./classes/include/implementation/variants/JSON_Number.hpp
  ./classes/include/implementation/variants/JSON_String.hpp
  ./classes/include/implementation/variants/JSON_Boolean.hpp
  ./classes/include/implementation/variants/JSON_Null.hpp
        classes/include/implementation/node/JSON_Node.hpp
        classes/include/implementation/node/JSON_Node_Index.hpp
        classes/include/implementation/node/JSON_Node_Reference.hpp
        classes/include/implementation/node/JSON_Node_Creation.hpp
  ./classes/include/implementation/io/JSON_Sources.hpp
  ./classes/include/implementation/io/JSON_BufferSource.hpp
  ./classes/include/implementation/io/JSON_FileSource.hpp
  ./classes/include/implementation/io/JSON_Destinations.hpp
  ./classes/include/implementation/io/JSON_BufferDestination.hpp
  ./classes/include/implementation/io/JSON_FileDestination.hpp
  ./classes/include/implementation/converter/JSON_Converter.hpp
  ./classes/include/implementation/translator/Default_Translator.hpp
  ./classes/include/implementation/translator/XML_Translator.hpp
  ./classes/include/implementation/parser/Default_Parser.hpp
  ./classes/include/implementation/stringify/Default_Stringify.hpp
  ./classes/include/implementation/stringify/Bencode_Stringify.hpp
  ./classes/include/implementation/stringify/XML_Stringify.hpp
  ./classes/include/implementation/stringify/YAML_Stringify.hpp
  ./classes/include/interface/JSON_Interfaces.hpp
  ./classes/include/interface/IAction.hpp
  ./classes/include/interface/ISource.hpp
  ./classes/include/interface/IDestination.hpp
  ./classes/include/interface/ITranslator.hpp
  ./classes/include/interface/IParser.hpp
  ./classes/include/interface/IStringify.hpp
)

# Platform specific
list(APPEND JSON_SOURCES "./classes/source/implementation/converter/${PLATFORM}/JSON_Converter.cpp")

# JSON library
add_library(${JSON_LIBRARY_NAME} ${JSON_SOURCES} ${JSON_INCLUDES})
target_include_directories(${JSON_LIBRARY_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/classes/include
  ${PROJECT_SOURCE_DIR}/classes/include/interface
  ${PROJECT_SOURCE_DIR}/classes/include/implementation
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/common
        ${PROJECT_SOURCE_DIR}/classes/include/implementation/node
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/io
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/converter
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/translator
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/variants
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/parser
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/stringify
  ${PROJECT_BINARY_DIR})

# Add tests
add_subdirectory(tests)

# Add exmaples
add_subdirectory(examples)

# install
install(TARGETS ${JSON_LIBRARY_NAME} DESTINATION lib)
install(FILES ${JSON_INCLUDES} DESTINATION include)
