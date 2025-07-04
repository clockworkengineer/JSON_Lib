#pragma once

#include <climits>
#include <filesystem>
#include <limits>
#include <sstream>
#include <string>
#include <iostream>

#include "catch2/catch_all.hpp"

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "Default_Translator.hpp"

// Unit test constants
constexpr char kSingleJSONFile[] = "testfile001.json";
constexpr char kNonExistantJSONFile[] = "doesntexist.json";
// Unit test helper functions
std::string prefixTestDataPath(const std::string &jsonFileName);
std::string stripWhiteSpace(const std::string &jsonBuffer);
void checkObject(const JSON_Lib::Node &jNode);
void checkArray(const JSON_Lib::Node &jNode);
std::string generateRandomFileName(void );
std::string generateEscapes(unsigned char first, unsigned char last);
// Test files macro
#define TEST_FILE_LIST(file)                                     \
  auto file = GENERATE(values<std::string>({ "testfile001.json", \
    "testfile002.json",                                          \
    "testfile003.json",                                          \
    "testfile004.json",                                          \
    "testfile005.json",                                          \
    "testfile006.json",                                          \
    "testfile007.json",                                          \
    "testfile008.json",                                          \
    "testfile009.json",                                          \
    "testfile010.json",                                          \
    "testfile011.json",                                          \
    "testfile012.json",                                          \
    "testfile013.json",                                          \
    "testfile014.json",                                          \
    "testfile015.json",                                          \
    "testfile016.json",                                          \
    "testfile017.json",                                          \
    "testfile018.json",                                          \
    "testfile019.json",                                          \
    "testfile020.json" }))
// Floating point comparison (accurate to within an epsilon)
template<typename T> bool equalFloatingPoint(T a, T b, double epsilon) { return (std::fabs(a - b) <= epsilon); }

using namespace JSON_Lib;