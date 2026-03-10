function(process_file target)
  add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/source/${target})
  target_precompile_headers(${target} REUSE_FROM JSON_Lib)
  target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/external)
  target_link_libraries(${target} JSON_Lib)
  install(TARGETS ${target} DESTINATION bin)
endfunction()

# Get example program list
file(GLOB EXAMPLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/ "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# Build each program
foreach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})
  process_file(${EXAMPLE_PROGRAM})
endforeach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})

add_custom_command(TARGET JSON_Parse_File.cpp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files ${CMAKE_CURRENT_BINARY_DIR}/files)
