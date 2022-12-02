#pragma once
// =====================
// Catch2 Test framework
// =====================
#include "catch2/catch_all.hpp"
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Translator.hpp"
#include "JSON_Core.hpp"
// =======
// C++ STL
// =======
#include <climits>
#include <filesystem>
#include <limits>
#include <sstream>
#include <string>
#include <iostream>
// ===================
// Unit test constants
// ===================
constexpr char kGeneratedJSONFile[] = "generated.json";
constexpr char kSingleJSONFile[] = "testfile001.json";
constexpr char kNonExistantJSONFile[] = "doesntexist.json";
// ==========================
// Unit test helper functions
// ==========================
const std::string prefixPath(const std::string &jsonFileName);
std::string stripWhiteSpace(const std::string &jsonBuffer);
void checkObject(const JSON_Lib::JNode &jNode);
void checkArray(const JSON_Lib::JNode &jNode);
// ================
// Test files macro
// ================
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
template<typename T> bool equalFloatingPoint(T a, T b, double epsilon) { return (std::fabs(a - b) <= epsilon); };