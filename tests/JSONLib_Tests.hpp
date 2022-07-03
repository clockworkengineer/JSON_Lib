#pragma once
// =====================
// Catch2 Test framework
// =====================
#include "catch2/catch.hpp"
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Types.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Translator.hpp"
// =======
// C++ STL
// =======
#include <filesystem>
#include <string>
#include <sstream>
// ===================
// Unit test constants
// ===================
inline static const char *kGeneratedJSONFile = "generated.json";
inline static const char *kSingleJSONFile = "testfile001.json" ;
inline static const char *kNonExistantJSONFile = "doesntexist.json";
// ==========================
// Unit test helper functions
// ==========================
std::string prefixTestDataPath(const std::string &file);
std::string readFromFile(const std::string &jsonFileName);
void checkObject(const JSONLib::JNode &jNode);
void checkArray(const JSONLib::JNode &jNode);
std::string stripWhiteSpace(JSONLib::JSON &json, const std::string &jsonBuffer);