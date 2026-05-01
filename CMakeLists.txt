
cmake_minimum_required(VERSION 3.21)

project("JSON_Lib" VERSION 1.2.0 DESCRIPTION "JSON C++ Library" LANGUAGES CXX)

# Optimization level for JSON_Lib target
set(JSON_LIB_OPTIMIZATION_LEVEL "O2" CACHE STRING "Optimization level for JSON_Lib target")
set_property(CACHE JSON_LIB_OPTIMIZATION_LEVEL PROPERTY STRINGS O0 O1 O2 O3 Os Ofast)

# Set C++ standard and options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings and platform
if(MSVC)
  add_compile_options(/W4 /WX /MP)

  if(JSON_LIB_OPTIMIZATION_LEVEL STREQUAL "O0")
    add_compile_options(/Od)
  elseif(JSON_LIB_OPTIMIZATION_LEVEL STREQUAL "O2" OR JSON_LIB_OPTIMIZATION_LEVEL STREQUAL "O3")
    add_compile_options($<$<NOT:$<CONFIG:Debug>>:/O2>)
  endif()

  set(PLATFORM "windows")
else()
  add_compile_options(-Wall -Werror -pedantic)
  add_compile_options(-${JSON_LIB_OPTIMIZATION_LEVEL})
  set(PLATFORM "linux")
endif()

# Export compile_commands.json for IDE tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to build unit tests
option(BUILD_TESTING "Build unit tests" ON)

# Option to build examples
option(BUILD_EXAMPLES "Build examples" OFF)

# Option to enable link-time optimization on the JSON library target
option(JSON_LIB_ENABLE_LTO "Enable link-time optimization" ON)

# Embedded build options
option(JSON_LIB_EMBEDDED "Enable embedded-friendly build configuration" OFF)
option(JSON_LIB_NO_EXCEPTIONS "Disable exception support" OFF)
option(JSON_LIB_NO_HEAP "Disable heap allocation support" OFF)
option(JSON_LIB_NO_DYNAMIC_MEMORY "Disable dynamic memory usage" OFF)
option(JSON_LIB_ALLOCATOR_INTERFACE "Enable custom allocator/pool interface" OFF)
option(JSON_LIB_NO_STDIO "Disable stdio/file I/O support" OFF)

# Compile-time resource limits (0 = use library default)
set(JSON_LIB_MAX_PARSER_DEPTH "0" CACHE STRING "Override default maximum JSON parse depth (0 = use default of 10)")
set(JSON_LIB_MAX_STRING_LENGTH "0" CACHE STRING "Override default maximum JSON string length in bytes (0 = use default of 16384)")

# Embedded build enforces smaller runtime and disables test/examples when enabled
if(JSON_LIB_EMBEDDED)
  message(STATUS "Configuring JSON_Lib for embedded-friendly build")
  set(BUILD_TESTING OFF CACHE BOOL "Build unit tests" FORCE)
  set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
  set(JSON_LIB_ENABLE_LTO OFF CACHE BOOL "Enable link-time optimization" FORCE)
  set(JSON_LIB_NO_STDIO ON CACHE BOOL "Disable stdio/file I/O support" FORCE)
endif()

# No-exceptions build disables tests/examples (PCH compiled with flag would mismatch test TUs)
if(JSON_LIB_NO_EXCEPTIONS AND NOT JSON_LIB_EMBEDDED)
  message(STATUS "Configuring JSON_Lib for no-exceptions build (tests/examples disabled)")
  set(BUILD_TESTING OFF CACHE BOOL "Build unit tests" FORCE)
  set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
endif()

# Configure embedded mode values for generated config header
macro(json_lib_set_flag_value OPTION_NAME)
  if(${OPTION_NAME})
    set(${OPTION_NAME}_VALUE 1)
  else()
    set(${OPTION_NAME}_VALUE 0)
  endif()
endmacro()

json_lib_set_flag_value(JSON_LIB_EMBEDDED)
json_lib_set_flag_value(JSON_LIB_NO_EXCEPTIONS)

if(JSON_LIB_NO_HEAP OR JSON_LIB_NO_DYNAMIC_MEMORY)
  set(JSON_LIB_NO_HEAP ON CACHE BOOL "Disable heap allocation support" FORCE)
  set(JSON_LIB_NO_DYNAMIC_MEMORY ON CACHE BOOL "Disable dynamic memory usage" FORCE)
endif()

json_lib_set_flag_value(JSON_LIB_NO_HEAP)
json_lib_set_flag_value(JSON_LIB_NO_DYNAMIC_MEMORY)
json_lib_set_flag_value(JSON_LIB_ALLOCATOR_INTERFACE)
json_lib_set_flag_value(JSON_LIB_NO_STDIO)

# Numeric resource-limit overrides — pass value directly (0 means "use library default")
set(JSON_LIB_MAX_PARSER_DEPTH_VALUE ${JSON_LIB_MAX_PARSER_DEPTH})
set(JSON_LIB_MAX_STRING_LENGTH_VALUE ${JSON_LIB_MAX_STRING_LENGTH})

# Configure version header
configure_file(JSON_Config.h.in JSON_Config.hpp)

# Library name
set(JSON_LIBRARY_NAME ${PROJECT_NAME})

set(JSON_CORE_SOURCES
  classes/source/JSON.cpp
  classes/source/implementation/JSON_Impl.cpp
  classes/source/interface/IParser.cpp
  classes/source/implementation/file/JSON_File.cpp
  classes/source/implementation/translator/Default_Translator.cpp
  classes/source/implementation/converter/${PLATFORM}/JSON_Converter.cpp
)

set(JSON_PARSER_SOURCES
  classes/source/implementation/parser/Default_Parser.cpp
)

set(JSON_INCLUDES
  JSON_Config.hpp
  classes/include/JSON.hpp
  classes/include/JSON_Core.hpp
  classes/include/implementation/JSON_Impl.hpp
  classes/include/implementation/common/JSON_Error.hpp
  classes/include/implementation/variants/JSON_Hole.hpp
  classes/include/implementation/variants/JSON_Object.hpp
  classes/include/implementation/variants/JSON_Array.hpp
  classes/include/implementation/variants/JSON_Number.hpp
  classes/include/implementation/variants/JSON_String.hpp
  classes/include/implementation/variants/JSON_Boolean.hpp
  classes/include/implementation/variants/JSON_Null.hpp
  classes/include/implementation/node/JSON_Node.hpp
  classes/include/implementation/node/JSON_Node_Index.hpp
  classes/include/implementation/node/JSON_Node_Reference.hpp
  classes/include/implementation/node/JSON_Node_Creation.hpp
  classes/include/implementation/io/JSON_Sources.hpp
  classes/include/implementation/io/JSON_BufferSource.hpp
  classes/include/implementation/io/JSON_FileSource.hpp
  classes/include/implementation/io/JSON_Destinations.hpp
  classes/include/implementation/io/JSON_BufferDestination.hpp
  classes/include/implementation/io/JSON_FileDestination.hpp
  classes/include/implementation/converter/JSON_Converter.hpp
  classes/include/implementation/translator/Default_Translator.hpp
  classes/include/implementation/translator/XML_Translator.hpp
  classes/include/implementation/parser/Default_Parser.hpp
  classes/include/implementation/stringify/Default_Stringify.hpp
  classes/include/implementation/stringify/Bencode_Stringify.hpp
  classes/include/implementation/stringify/XML_Stringify.hpp
  classes/include/implementation/stringify/YAML_Stringify.hpp
  classes/include/interface/JSON_Interfaces.hpp
  classes/include/interface/IAction.hpp
  classes/include/interface/ISource.hpp
  classes/include/interface/IDestination.hpp
  classes/include/interface/ITranslator.hpp
  classes/include/interface/IParser.hpp
  classes/include/interface/IStringify.hpp
)

# Shared implementation include directories used by library targets and tests
set(JSON_IMPL_INCLUDE_DIRS
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
  ${PROJECT_BINARY_DIR}
)

# Create modular JSON_Lib targets
add_library(JSON_Lib_Core OBJECT ${JSON_CORE_SOURCES})
add_library(JSON_Lib_Parser OBJECT ${JSON_PARSER_SOURCES})
add_library(JSON_Lib_Stringify INTERFACE)

target_include_directories(JSON_Lib_Stringify INTERFACE ${JSON_IMPL_INCLUDE_DIRS})

target_include_directories(JSON_Lib_Core PRIVATE ${JSON_IMPL_INCLUDE_DIRS})

target_precompile_headers(JSON_Lib_Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h)

target_include_directories(JSON_Lib_Parser PRIVATE ${JSON_IMPL_INCLUDE_DIRS})

target_precompile_headers(JSON_Lib_Parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h)

# Dedicated embedded compile flag set
add_library(JSON_Lib_Embedded INTERFACE)

target_compile_definitions(JSON_Lib_Embedded INTERFACE
  $<$<BOOL:${JSON_LIB_EMBEDDED}>:JSON_LIB_EMBEDDED>
  $<$<BOOL:${JSON_LIB_NO_EXCEPTIONS}>:JSON_LIB_NO_EXCEPTIONS>
  $<$<BOOL:${JSON_LIB_NO_HEAP}>:JSON_LIB_NO_HEAP>
  $<$<BOOL:${JSON_LIB_NO_DYNAMIC_MEMORY}>:JSON_LIB_NO_DYNAMIC_MEMORY>
  $<$<BOOL:${JSON_LIB_ALLOCATOR_INTERFACE}>:JSON_LIB_ALLOCATOR_INTERFACE>
  $<$<BOOL:${JSON_LIB_NO_STDIO}>:JSON_LIB_NO_STDIO>
)

target_link_libraries(JSON_Lib_Core PRIVATE JSON_Lib_Embedded)

target_link_libraries(JSON_Lib_Parser PRIVATE JSON_Lib_Embedded)

target_link_libraries(JSON_Lib_Stringify INTERFACE JSON_Lib_Embedded)

# Create the public JSON_Lib wrapper target from component object libraries
add_library(${JSON_LIBRARY_NAME} STATIC
  $<TARGET_OBJECTS:JSON_Lib_Core>
  $<TARGET_OBJECTS:JSON_Lib_Parser>
)

target_link_libraries(${JSON_LIBRARY_NAME} PRIVATE JSON_Lib_Stringify)

target_include_directories(${JSON_LIBRARY_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)

# Apply link-time optimization if enabled
if(JSON_LIB_ENABLE_LTO)
  set_property(TARGET ${JSON_LIBRARY_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Add tests if enabled
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

# Add examples
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Install rules
install(TARGETS ${JSON_LIBRARY_NAME}
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

if(JSON_LIB_EXPORT_INTERFACE)
  install(FILES
    ${PROJECT_SOURCE_DIR}/classes/include/JSON.hpp
    ${PROJECT_SOURCE_DIR}/classes/include/JSON_Config.hpp
    ${PROJECT_SOURCE_DIR}/classes/include/interface/JSON_Interfaces.hpp
    ${PROJECT_SOURCE_DIR}/classes/include/interface/IAction.hpp
    ${PROJECT_SOURCE_DIR}/classes/include/interface/ISource.hpp
    ${PROJECT_SOURCE_DIR}/classes/include/interface/IDestination.hpp
    ${PROJECT_SOURCE_DIR}/classes/include/interface/ITranslator.hpp
    ${PROJECT_SOURCE_DIR}/classes/include/interface/IParser.hpp
    ${PROJECT_SOURCE_DIR}/classes/include/interface/IStringify.hpp
    DESTINATION include)
else()
  install(FILES ${JSON_INCLUDES} DESTINATION include)
endif()
