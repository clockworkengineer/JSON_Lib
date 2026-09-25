cmake_minimum_required(VERSION 3.21)
project(JSON_Lib_Consumer_Verification LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(JSON_Lib CONFIG REQUIRED)

add_executable(consumer_test main.cpp)
target_link_libraries(consumer_test PRIVATE JSON_Lib::JSON_Lib)
