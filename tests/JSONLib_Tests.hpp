#pragma once
// =====================
// Catch2 Test framework
// =====================
#include "catch2/catch.hpp"
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Translator.hpp"
#include "JSON_Types.hpp"

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
inline static const char *kGeneratedJSONFile = "generated.json";
inline static const char *kSingleJSONFile = "testfile001.json";
inline static const char *kNonExistantJSONFile = "doesntexist.json";
// ==========================
// Unit test helper functions
// ==========================
std::string prefixTestDataPath(const std::string &jsonFileName);
std::string readFromFile(const std::string &jsonFileName);
void writeToFile(const std::string &jsonFileName, const std::string &jsonString);
void checkObject(const JSONLib::JNode &jNode);
void checkArray(const JSONLib::JNode &jNode);
std::string stripWhiteSpace(const JSONLib::JSON &json, const std::string &jsonBuffer);
// ================
// Test files macro
// ================
#define TEST_FILE_LIST(file)                                                   \
  auto file = GENERATE(values<std::string>({                                   \
      "testfile001.json",                                                      \
      "testfile002.json",                                                      \
      "testfile003.json",                                                      \
      "testfile004.json",                                                      \
      "testfile005.json",                                                      \
      "testfile006.json",                                                      \
      "testfile007.json",                                                      \
      "testfile008.json",                                                      \
      "testfile009.json",                                                      \
      "testfile010.json",                                                      \
      "testfile011.json",                                                      \
      "testfile012.json"                                                      \
  }))
//      "testfile013.json",                                                      \
// FLoating point comparison (accurate to within an epsilon)
template <typename T> bool equalFloatingPoint(T a, T b, double epsilon) {
  return (std::fabs(a - b) <= epsilon);
};