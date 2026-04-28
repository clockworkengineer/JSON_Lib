// Program: JSON_Deep_Search.cpp
//
// Description: Recursively search for all values matching a given key in a JSON file.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

#include <vector>
#include <string>
#include <iostream>

// Recursively search for all values with the given key
static std::string nodeToString(const js::Node &node)
{
  js::BufferDestination destination;
  js::Default_Stringify{}.stringify(node, destination, 0);
  return destination.toString();
}

void findAllByKey(const js::Node &node, const std::string &key, std::vector<std::string> &results)
{
  if (js::isA<js::Object>(node)) {
    for (const auto &entry : js::NRef<js::Object>(node).value()) {
      const std::string_view entryKey = entry.getKey();
      const js::Node &entryNode = entry.getNode();
      if (entryKey == key) { results.push_back(nodeToString(entryNode)); }
      findAllByKey(entryNode, key, results);
    }
  } else if (js::isA<js::Array>(node)) {
    for (const auto &item : js::NRef<js::Array>(node).value()) { findAllByKey(item, key, results); }
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    init(plog::debug, "JSON_Deep_Search.log");
    PLOG_INFO << "JSON_Deep_Search started ...";
    PLOG_INFO << js::JSON().version();
    std::string inputFile = "files/testfile001.json";
    std::string searchKey = "id";// Change as needed

    js::JSON json;
    json.parse(js::FileSource{ inputFile });

    std::vector<std::string> results;
    findAllByKey(json.root(), searchKey, results);

    std::cout << "Found " << results.size() << " values for key '" << searchKey << "':\n";
    for (const auto &val : results) { std::cout << val << std::endl; }
  } catch (const std::exception &ex) {
    PLOG_ERROR << ex.what();
    return 1;
  }
  return 0;
}
