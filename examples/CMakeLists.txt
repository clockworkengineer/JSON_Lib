cmake_minimum_required(VERSION 3.18.1)

project("JSON_Lib Examples" VERSION 1.1.0 DESCRIPTION "JSON_Lib example programs")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(process_file target)
  add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/source/${target})
  target_precompile_headers(${target} REUSE_FROM JSON_Lib)
  target_include_directories(${target} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/external)
  target_link_libraries(${target} JSON_Lib)
  install(TARGETS ${target} DESTINATION bin)
endfunction()

# Get example program list
file(GLOB EXAMPLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/ "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# Build each program
foreach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})
  process_file(${EXAMPLE_PROGRAM})
endforeach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})

add_custom_command(TARGET JSON_Parse_File.cpp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/files)
