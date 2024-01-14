cmake_minimum_required(VERSION 3.18.1)

project("JSON_Lib" VERSION 1.0.0 DESCRIPTION "JSON C++ Library" LANGUAGES CXX)

# C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler options
if(MSVC)
  add_compile_options(/W4)
  set(PLATFORM "windows")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Werror -pedantic)
  set(PLATFORM "linux")
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
  ./classes/JSON.cpp
  ./classes/implementation/JSON_Impl.cpp
  ./classes/implementation/file/JSON_Impl_File.cpp
  ./classes/implementation/translator/JSON_Translator.cpp
  ./classes/implementation/parser/JSON_Parser.cpp
  ./classes/implementation/stringify/JSON_Stringify.cpp
  ./classes/implementation/stringify/Bencode_Stringify.cpp)

set(JSON_INCLUDES
  JSON_Config.hpp
  ./include/JSON.hpp
  ./include/implementation/JSON_Core.hpp
  ./include/implementation/JSON_Impl.hpp
  ./include/implementation/variants/JSON_Variant.hpp
  ./include/implementation/variants/JSON_Hole.hpp
  ./include/implementation/variants/JSON_Object.hpp
  ./include/implementation/variants/JSON_Array.hpp
  ./include/implementation/variants/JSON_Number.hpp
  ./include/implementation/variants/JSON_String.hpp
  ./include/implementation/variants/JSON_Boolean.hpp
  ./include/implementation/variants/JSON_Null.hpp
  ./include/implementation/jnode/JSON_JNode.hpp
  ./include/implementation/jnode/JSON_JNode_Index.hpp
  ./include/implementation/jnode/JSON_JNode_Reference.hpp
  ./include/implementation/jnode/JSON_JNode_Creation.hpp
  ./include/implementation/io/JSON_Sources.hpp
  ./include/implementation/io/JSON_BufferSource.hpp
  ./include/implementation/io/JSON_FileSource.hpp
  ./include/implementation/io/JSON_Destinations.hpp
  ./include/implementation/io/JSON_BufferDestination.hpp
  ./include/implementation/io/JSON_FileDestination.hpp
  ./include/implementation/translation/JSON_Translator.hpp
  ./include/implementation/translation/JSON_Converter.hpp
  ./include/implementation/parser/JSON_Parser.hpp
  ./include/implementation/stringify/JSON_Stringify.hpp
  ./include/implementation/stringify/Bencode_Stringify.hpp
  ./include/interface/JSON_Interfaces.hpp
  ./include/interface/IAction.hpp
  ./include/interface/ISource.hpp
  ./include/interface/IDestination.hpp
  ./include/interface/ITranslator.hpp
  ./include/interface/IConverter.hpp
  ./include/interface/IParser.hpp
  ./include/interface/IStringify.hpp)

# Platform specific
list(APPEND JSON_SOURCES "./classes/implementation/translator/${PLATFORM}/JSON_Converter.cpp")

# JSON library
add_library(${JSON_LIBRARY_NAME} ${JSON_SOURCES} ${JSON_INCLUDES})
target_include_directories(${JSON_LIBRARY_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/interface
  ${PROJECT_SOURCE_DIR}/include/implementation
  ${PROJECT_SOURCE_DIR}/include/implementation/jnode
  ${PROJECT_SOURCE_DIR}/include/implementation/io
  ${PROJECT_SOURCE_DIR}/include/implementation/translation
  ${PROJECT_SOURCE_DIR}/include/implementation/variants
  ${PROJECT_SOURCE_DIR}/include/implementation/parser
  ${PROJECT_SOURCE_DIR}/include/implementation/stringify
  ${PROJECT_SOURCE_DIR}/include/external
  ${PROJECT_BINARY_DIR})

# Add tests
add_subdirectory(tests)

# Add exmaples
add_subdirectory(examples)

# install
install(TARGETS ${JSON_LIBRARY_NAME} DESTINATION lib)
install(FILES ${JSON_INCLUDES} DESTINATION include)
