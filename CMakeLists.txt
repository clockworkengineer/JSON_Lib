cmake_minimum_required(VERSION 3.23.1)

project("JSONLib" VERSION 0.1.0 DESCRIPTION "JSON C++ Library" LANGUAGES CXX)

# C++ options

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

# Compiler options

if(MSVC)
  add_compile_options(/W4)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Werror -pedantic)
endif()

# Setup library version

configure_file(JSON_Config.h.in JSON_Config.hpp)

# Library name

set(JSON_LIBRARY_NAME ${PROJECT_NAME})

# JSON sources and includes

set (JSON_SOURCES
    ./classes/JSON.cpp
    ./classes/implementation/JSON_Impl.cpp
    ./classes/implementation/JSON_Impl_Parse.cpp
    ./classes/implementation/JSON_Impl_Stringify.cpp
    ./classes/implementation/JSON_Impl_Strip.cpp
    ./classes/implementation/JSON_JNode.cpp
    ./classes/implementation/JSON_Translator.cpp)

set (JSON_INCLUDES
    JSON_Config.hpp
    ./include/JSON.hpp
    ./include/implementation/JSON_Types.hpp
    ./include/implementation/JSON_Error.hpp
    ./include/implementation/JSON_JNode.hpp
    ./include/implementation/JSON_JNode_Numeric.hpp
    ./include/implementation/JSON_JNode_Variant.hpp
    ./include/implementation/JSON_JNode_Hole.hpp
    ./include/implementation/JSON_JNode_Object.hpp
    ./include/implementation/JSON_JNode_Array.hpp
    ./include/implementation/JSON_JNode_Number.hpp
    ./include/implementation/JSON_JNode_String.hpp
    ./include/implementation/JSON_JNode_Boolean.hpp
    ./include/implementation/JSON_JNode_Null.hpp
    ./include/implementation/JSON_JNode_Reference.hpp
    ./include/implementation/JSON_Impl.hpp
    ./include/implementation/JSON_Sources.hpp
    ./include/implementation/JSON_Destinations.hpp
    ./include/implementation/JSON_Translator.hpp
    ./include/implementation/JSON_Converter.hpp
    ./include/interface/IAction.hpp
    ./include/interface/ISource.hpp
    ./include/interface/IDestination.hpp
    ./include/interface/ITranslator.hpp
    ./include/interface/IConverter.hpp)

# Platform specific

if(MSVC)
  list (APPEND JSON_SOURCES "./classes/implementation/platform/windows/Converter.cpp")
else()
  list (APPEND JSON_SOURCES "./classes/implementation/platform/linux/Converter.cpp")
endif()

# JSON library

add_library(${JSON_LIBRARY_NAME} ${JSON_SOURCES} ${JSON_INCLUDES})
target_include_directories(${JSON_LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/interface ${PROJECT_SOURCE_DIR}/include/implementation ${PROJECT_SOURCE_DIR}/include/external ${PROJECT_BINARY_DIR} )

# Add tests

add_subdirectory(tests)

# Add exmaples

add_subdirectory(examples)

# install

install(TARGETS ${JSON_LIBRARY_NAME} DESTINATION lib)
install(FILES  ${JSON_INCLUDES} DESTINATION include)


