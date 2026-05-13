#pragma once

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <variant>

#include "JSON_Config.hpp"
#include "implementation/common/JSON_Attributes.hpp"
#include "implementation/common/JSON_Error.hpp"

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
struct String;
class Default_Parser;
struct Node;

class JSON_LIB_API JSON
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
  explicit JSON(std::unique_ptr<IStringify> stringify=nullptr, std::unique_ptr<IParser> parser=nullptr);
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
  JSON_LIB_NODISCARD static std::string version();
  // Parse JSON into the tree
  void parse(ISource &source);
  void parse(ISource &&source);
  JSON_LIB_NODISCARD Result<Node> parseResult(ISource &source);
  JSON_LIB_NODISCARD Result<Node> parseResult(ISource &&source);
  // Create JSON text string from Node tree (no whitespace)
  void stringify(IDestination &destination) const;
  void stringify(IDestination &&destination) const;
  JSON_LIB_NODISCARD Result<void> stringifyResult(IDestination &destination) const;
  JSON_LIB_NODISCARD Result<void> stringifyResult(IDestination &&destination) const;
  // Create JSON text string from Node tree (pretty printed)
  void print(IDestination &destination) const;
  void print(IDestination &&destination) const;
  JSON_LIB_NODISCARD Result<void> printResult(IDestination &destination) const;
  JSON_LIB_NODISCARD Result<void> printResult(IDestination &&destination) const;
  // Strip whitespace from JSON string
  static void strip(ISource &source, IDestination &destination);
  static void strip(ISource &source, IDestination &&destination) ;
  static void strip(ISource &&source, IDestination &destination) ;
  static void strip(ISource &&source, IDestination &&destination) ;
  // Traverse JSON tree
  void traverse(IAction &action);
  void traverse(IAction &action) const;
  JSON_LIB_NODISCARD Result<void> traverseResult(IAction &action);
  JSON_LIB_NODISCARD Result<void> traverseResult(IAction &action) const;
  // Set print ident value
  void setIndent(long indent);
  // Set/get maximum parser recursion depth
  void setMaxParserDepth(unsigned long depth);
  JSON_LIB_NODISCARD unsigned long getMaxParserDepth() const noexcept;
  // Get the root of JSON tree
  JSON_LIB_NODISCARD Node &root();
  JSON_LIB_NODISCARD const Node &root() const;
  // Search for JSON object entry with a given key
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  // Get JSON array entry at index
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  // Resize the root array explicitly without silent growth.
  void resize(std::size_t index);
#if !JSON_LIB_NO_STDIO
  // Read/Write JSON from the file
  static std::string fromFile(const std::string_view &fileName);
  static void toFile(const std::string_view &fileName, const std::string_view &jsonString, Format format = Format::utf8);
  // Get JSON file format
  static Format getFileFormat(const std::string_view &fileName);
#endif

private:
  // JSON implementation
  const std::unique_ptr<JSON_Impl> implementation;
};

class JSON_LIB_API EmbeddedJSON final : public JSON
{
public:
  using JSON::JSON;
  using Format = JSON::Format;
  using InitializerListTypes = JSON::InitializerListTypes;
  using ArrayInitializer = JSON::ArrayInitializer;
  using ObjectInitializer = JSON::ObjectInitializer;

  static constexpr bool isEmbeddedBuild() noexcept { return JSON_LIB_EMBEDDED; }
  static constexpr bool isExceptionFreeBuild() noexcept { return JSON_LIB_NO_EXCEPTIONS; }
  static constexpr bool isNoStdIoBuild() noexcept { return JSON_LIB_NO_STDIO; }
  static constexpr bool isNoDynamicMemoryBuild() noexcept { return JSON_LIB_NO_DYNAMIC_MEMORY; }

  // Exception-free API — preferred for embedded targets
  JSON_LIB_NODISCARD Result<Node> parseNoThrow(ISource &source);
  JSON_LIB_NODISCARD Result<Node> parseNoThrow(ISource &&source);
  JSON_LIB_NODISCARD Result<void> stringifyNoThrow(IDestination &destination) const  { return stringifyResult(destination); }
  JSON_LIB_NODISCARD Result<void> stringifyNoThrow(IDestination &&destination) const { return stringifyResult(std::move(destination)); }
  JSON_LIB_NODISCARD Result<void> printNoThrow(IDestination &destination) const  { return printResult(destination); }
  JSON_LIB_NODISCARD Result<void> printNoThrow(IDestination &&destination) const { return printResult(std::move(destination)); }
  JSON_LIB_NODISCARD Result<void> traverseNoThrow(IAction &action)       { return traverseResult(action); }
  JSON_LIB_NODISCARD Result<void> traverseNoThrow(IAction &action) const { return traverseResult(action); }

  struct Limits
  {
    // Compile-time resource limits (reflect JSON_LIB_MAX_PARSER_DEPTH / JSON_LIB_MAX_STRING_LENGTH)
    static constexpr unsigned long kMaxParserDepth  = JSON_LIB_MAX_PARSER_DEPTH  ? JSON_LIB_MAX_PARSER_DEPTH  : 10UL;
    static constexpr uint64_t      kMaxStringLength = JSON_LIB_MAX_STRING_LENGTH ? JSON_LIB_MAX_STRING_LENGTH : 16384ULL;
    // Runtime accessors (may differ if changed via set* at startup)
    static uint64_t maxStringLength() noexcept;
    static unsigned long maxParserDepth() noexcept;
  };
};
}// namespace JSON_Lib