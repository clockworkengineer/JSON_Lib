#pragma once
// ======================
// Test framework include
// ======================
#include "catch2/catch.hpp"
// =========================
// Class under test includes
// =========================
#include "JSON.hpp"
#include "JSONSources.hpp"
#include "JSONDestinations.hpp"
#include "JSONTranslator.hpp"
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