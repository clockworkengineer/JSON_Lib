#pragma once
// ======================
// Test framework include
// ======================
#include "catch2/catch.hpp"
// =========================
// Class under test includes
// =========================
#include "JSON.hpp"
#include "JSON_Nodes.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Translator.hpp"
// ============
// STL includes
// ============
#include <filesystem>
#include <string>
#include <sstream>
// ===================
// Unit test constants
// ===================
#define kGeneratedJSONFile "generated.json"
#define kSingleJSONFile "testfile001.json"
#define kNonExistantJSONFile "doesntexist.json"
// ==========================
// Unit test helper functions
// ==========================
std::string prefixTestDataPath(const std::string &file);
std::string readJSONFromFile(const std::string &jsonFileName);
void checkObject(JSONLib::JNode &jNode);
void checkArray(JSONLib::JNode &jNode);
std::string stripWhiteSpace(JSONLib::JSON &json, const std::string &jsonBuffer);