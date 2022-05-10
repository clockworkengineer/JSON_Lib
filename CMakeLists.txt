cmake_minimum_required(VERSION 3.23.1)

project("JSONLib" VERSION 0.1.0 DESCRIPTION "JSON C++ Library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

if(MSVC)
  add_compile_options(/W4 /D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Werror -pedantic)
endif()

configure_file(JSONConfig.h.in JSONConfig.hpp)

set(JSON_LIBRARY_NAME ${PROJECT_NAME})

# JSON sources and includes

set (JSON_SOURCES
    ./classes/JSON.cpp
    ./classes/JSONImplementation.cpp
    ./classes/JSONTranslator.cpp
)

set (JSON_INCLUDES
    JSONConfig.hpp
    ./include/JSON.hpp
    ./include/JSONImplementation.hpp
    ./include/JNode.hpp
    ./include/JSONSources.hpp
    ./include/JSONDestinations.hpp
    ./include/JSONTranslator.hpp
    ./include/interface/Isource.hpp
    ./include/interface/IDestination.hpp
    ./include/interface/ITranslator.hpp
)

# JSON library

add_library(${JSON_LIBRARY_NAME} ${JSON_SOURCES})
target_include_directories(${JSON_LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/interface ${PROJECT_BINARY_DIR} )

# Add tests

add_subdirectory(tests)

# Add exmaples

add_subdirectory(examples)

# install

install(TARGETS ${JSON_LIBRARY_NAME} DESTINATION lib)
install(FILES  ${JSON_INCLUDES} DESTINATION include)


