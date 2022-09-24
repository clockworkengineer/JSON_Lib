#pragma once
// =======
// C++ STL
// =======
#include <set>
#include <sstream>
#include <unordered_map>
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Types.hpp"
#include "IAction.hpp"
// ==================
// JSON Tree Analysis
// ==================
class JSON_Indexer : public JSON_Lib::IAction
{
public:
  // ========================
  // Constructors/destructors
  // ========================
  JSON_Indexer() = default;
  explicit JSON_Indexer(const std::string &fileName) : m_fileName(fileName) {}
  virtual ~JSON_Indexer() = default;
  // Create index to object key mapping and modify object key to use index
  virtual void onObject(JSON_Lib::JNode &jNode) override
  {
    for (auto &entry : JSON_Lib::JRef<JSON_Lib::Object>(jNode).getObjectEntries()) {
      auto &key = JSON_Lib::JRef<JSON_Lib::String>(entry.getKey()).getString();
      if (!m_index.contains(key)) {
        m_index[key] = m_currentIndex;
        m_array[m_currentIndex++] = key;
      }
      entry.getKey() = std::to_string(m_index[key]);
    }
  }
  virtual void onNumber([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onBoolean([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onNull([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onArray([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onJNode([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onString([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  // Const api not used
  virtual void onNumber([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onBoolean([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onNull([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onArray([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onObject([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onJNode([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onString([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  // Save away index (JSON array) to file
  void save() { m_array.stringify(JSON_Lib::FileDestination{ m_fileName + ".idx" }); }

private:
  std::string m_fileName;
  int m_currentIndex{};
  std::unordered_map<std::string, int> m_index;
  JSON_Lib::JSON m_array;
};