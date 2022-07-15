cmake_minimum_required(VERSION 3.23.1)

project("JSONLib" VERSION 0.1.0 DESCRIPTION "JSON C++ Library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

if(MSVC)
  add_compile_options(/W4)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Werror -pedantic)
endif()

configure_file(JSON_Config.h.in JSON_Config.hpp)

set(JSON_LIBRARY_NAME ${PROJECT_NAME})

# JSON sources and includes

set (JSON_SOURCES
    ./classes/JSON.cpp
    ./classes/implementation/JSON_Impl.cpp
    ./classes/implementation/JSON_API.cpp
    ./classes/implementation/JSON_JNode.cpp
    ./classes/implementation/JSON_Translator.cpp
    ./classes/implementation/JSON_Converter.cpp)

set (JSON_INCLUDES
    JSON_Config.hpp
    ./include/JSON.hpp
    ./include/implementation/JSON_Types.hpp
    ./include/implementation/JSON_Error.hpp
    ./include/implementation/JSON_JNodeNumeric.hpp
    ./include/implementation/JSON_JNode.hpp
    ./include/implementation/JSON_JNodeVariant.hpp
    ./include/implementation/JSON_JNodeCreation.hpp
    ./include/implementation/JSON_Impl.hpp
    ./include/implementation/JSON_Sources.hpp
    ./include/implementation/JSON_Destinations.hpp
    ./include/implementation/JSON_Translator.hpp
    ./include/implementation/JSON_Converter.hpp
    ./include/interface/ISource.hpp
    ./include/interface/IDestination.hpp
    ./include/interface/ITranslator.hpp
    ./include/interface/IConverter.hpp)

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


