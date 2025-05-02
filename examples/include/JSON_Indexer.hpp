#pragma once

#include <set>
#include <sstream>
#include <unordered_map>
#include <utility>

#include "JSON.hpp"
#include "JSON_Core.hpp"

class JSON_Indexer : public JSON_Lib::IAction
{
public:
  JSON_Indexer() = default;
  explicit JSON_Indexer(std::string fileName) : fileName(std::move(fileName)) {}
  virtual ~JSON_Indexer() = default;
  // Create index to object key mapping and modify object key to use index
  virtual void onObject(JSON_Lib::JNode &jNode) override
  {
    for (auto &entry : JSON_Lib::JRef<JSON_Lib::Object>(jNode).value()) {
      auto key = std::string(entry.getKey());
      if (!index.contains(key)) {
        index[key] = currentIndex;
        array[currentIndex++] = key;
      }
      entry.getKey() = std::to_string(index[key]);
    }
  }
  // Save away index (JSON array) to file
  void save() { array.stringify(JSON_Lib::FileDestination{ fileName + ".idx" }); }

private:
  std::string fileName;
  int currentIndex{};
  std::unordered_map<std::string, int> index;
  JSON_Lib::JSON array;
};