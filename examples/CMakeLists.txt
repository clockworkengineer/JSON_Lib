# Modern CMake: Add all example executables except those in EXCLUDE_EXAMPLES
file(GLOB EXAMPLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/ "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

set(EXCLUDE_EXAMPLES
  JSON_Custom_Struct.cpp
)

foreach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})
  list(FIND EXCLUDE_EXAMPLES ${EXAMPLE_PROGRAM} _exclude_idx)

  if(_exclude_idx EQUAL -1)
    add_executable(${EXAMPLE_PROGRAM} source/${EXAMPLE_PROGRAM})
    target_precompile_headers(${EXAMPLE_PROGRAM} REUSE_FROM JSON_Lib)
    target_include_directories(${EXAMPLE_PROGRAM} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/external)
    target_link_libraries(${EXAMPLE_PROGRAM} JSON_Lib)
    install(TARGETS ${EXAMPLE_PROGRAM} DESTINATION bin)
  endif()
endforeach()

# Copy example files after build for a specific example (if needed)
add_custom_command(TARGET JSON_Parse_File.cpp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files ${CMAKE_CURRENT_BINARY_DIR}/files)
