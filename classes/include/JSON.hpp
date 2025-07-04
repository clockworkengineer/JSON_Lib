#pragma once

#include <array>
#include <stdexcept>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <filesystem>
#include <set>
#include <unordered_map>
#include <cstring>
#include <functional>
#include <map>
#include <string_view>

namespace JSON_Lib {

// =========================
// JSON forward declarations
// =========================
class IStringify;
class IParser;
class ISource;
class IDestination;
class IAction;
class JSON_Impl;
struct Node;

class JSON
{
public:
  // Possible JSON Node initializer list types
  using InitializerListTypes =
    std::variant<int, long, long long, float, double, long double, bool, std::string, std::nullptr_t, Node>;
  // Array initializer list
  using ArrayInitializer = std::initializer_list<InitializerListTypes>;
  // Object initializer list
  using ObjectInitializer = std::initializer_list<std::pair<std::string, InitializerListTypes>>;
  // JSON file formats
  enum class Format : uint8_t { utf8 = 0, utf8BOM, utf16BE, utf16LE, utf32BE, utf32LE };
  // Pass any user defined translator/converter here
  explicit JSON([[maybe_unused]]IStringify *stringify=nullptr, [[maybe_unused]] IParser *parser= nullptr);
  // Pass in default JSON to parse
  explicit JSON(const std::string_view &jsonString);
  // Construct an array
  JSON(const ArrayInitializer &array);
  // Construct object
  JSON(const ObjectInitializer &object);
  // No other constructors supported
  JSON(const JSON &other) = delete;
  JSON &operator=(const JSON &other) = delete;
  JSON(JSON &&other) = delete;
  JSON &operator=(JSON &&other) = delete;
  // Provide own destructor
  ~JSON();
  // Get JSON library version
  [[nodiscard]] static std::string version();
  // Parse JSON into the tree
  void parse(ISource &source) const;
  void parse(ISource &&source) const;
  // Create JSON text string from Node tree (no whitespace)
  void stringify(IDestination &destination) const;
  void stringify(IDestination &&destination) const;
  // Create JSON text string from Node tree (pretty printed)
  void print(IDestination &destination) const;
  void print(IDestination &&destination) const;
  // Strip whitespace from JSON string
  static void strip(ISource &source, IDestination &destination);
  static void strip(ISource &source, IDestination &&destination) ;
  static void strip(ISource &&source, IDestination &destination) ;
  static void strip(ISource &&source, IDestination &&destination) ;
  // Traverse JSON tree
  void traverse(IAction &action);
  void traverse(IAction &action) const;
  // Set print ident value
  static void setIndent(long indent);
  // Get the root of JSON tree
  [[nodiscard]] Node &root();
  [[nodiscard]] const Node &root() const;
  // Search for JSON object entry with a given key
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  // Get JSON array entry at index
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  // Read/Write JSON from the file
  static std::string fromFile(const std::string_view &fileName);
  static void toFile(const std::string_view &fileName, const std::string_view &jsonString, Format format = Format::utf8);
  // Get JSON file format
  static Format getFileFormat(const std::string_view &fileName);

private:
  // JSON implementation
  const std::unique_ptr<JSON_Impl> implementation;
};
}// namespace JSON_Lib