#pragma once

#include <memory>
#include <unordered_map>
#include "JSON.hpp"
#include "JSON_Converter.hpp"

#if __has_include("JSON_Error.hpp")
#include "JSON_Error.hpp"
#else
#include "implementation/common/JSON_Error.hpp"
#endif

#if __has_include("JSON_Throw.hpp")
#include "JSON_Throw.hpp"
#else
#include "implementation/common/JSON_Throw.hpp"
#endif

#if __has_include("IEncodingHandler.hpp")
#include "IEncodingHandler.hpp"
#else
#include "interface/IEncodingHandler.hpp"
#endif

namespace JSON_Lib {

#if !JSON_LIB_NO_STDIO

/// @brief Encoding handler for UTF-8 without BOM.
class Utf8EncodingHandler final : public IEncodingHandler
{
public:
  std::string read(std::ifstream &file) const override
  {
    const auto initialPosition = file.tellg();
    if (initialPosition == -1) { return {}; }
    file.seekg(0, std::ios_base::end);
    const auto length = file.tellg();
    file.seekg(initialPosition, std::ios_base::beg);
    std::string content;
    if (length > 0) {
      content.reserve(static_cast<std::size_t>(length));
    }
    content.assign(std::istreambuf_iterator<char>(file), {});
    return content;
  }

  void write(std::ofstream &file, const std::string_view &utf8Content) const override
  {
    file << utf8Content;
  }
};

/// @brief Encoding handler for UTF-8 with BOM (0xEF, 0xBB, 0xBF).
class Utf8BOMEncodingHandler final : public IEncodingHandler
{
public:
  std::string read(std::ifstream &file) const override
  {
    // Skip 3 BOM bytes if present
    file.seekg(3, std::ios_base::beg);
    Utf8EncodingHandler baseHandler;
    return baseHandler.read(file);
  }

  void write(std::ofstream &file, const std::string_view &utf8Content) const override
  {
    file << static_cast<unsigned char>(0xEF)
         << static_cast<unsigned char>(0xBB)
         << static_cast<unsigned char>(0xBF);
    file << utf8Content;
  }
};

/// @brief Encoding handler for UTF-16 Big Endian.
class Utf16BEEncodingHandler final : public IEncodingHandler
{
public:
  std::string read(std::ifstream &file) const override
  {
    file.seekg(2, std::ios_base::beg);
    std::u16string utf16String;
    while (true) {
      int b1 = file.get();
      int b2 = file.get();
      if (b1 == EOF || b2 == EOF) break;
      char16_t ch16 = (static_cast<unsigned char>(b1) << 8) | static_cast<unsigned char>(b2);
      utf16String.push_back(ch16);
    }
    return toUtf8(utf16String);
  }

  void write(std::ofstream &file, const std::string_view &utf8Content) const override
  {
    file << static_cast<unsigned char>(0xFE) << static_cast<unsigned char>(0xFF);
    std::u16string u16 = toUtf16(utf8Content);
    for (const char16_t ch : u16) {
      file.put(static_cast<char>(ch >> 8));
      file.put(static_cast<char>(ch & 0xFF));
    }
  }
};

/// @brief Encoding handler for UTF-16 Little Endian.
class Utf16LEEncodingHandler final : public IEncodingHandler
{
public:
  std::string read(std::ifstream &file) const override
  {
    file.seekg(2, std::ios_base::beg);
    std::u16string utf16String;
    while (true) {
      int b1 = file.get();
      int b2 = file.get();
      if (b1 == EOF || b2 == EOF) break;
      char16_t ch16 = static_cast<unsigned char>(b1) | (static_cast<unsigned char>(b2) << 8);
      utf16String.push_back(ch16);
    }
    return toUtf8(utf16String);
  }

  void write(std::ofstream &file, const std::string_view &utf8Content) const override
  {
    file << static_cast<unsigned char>(0xFF) << static_cast<unsigned char>(0xFE);
    std::u16string u16 = toUtf16(utf8Content);
    for (const char16_t ch : u16) {
      file.put(static_cast<char>(ch & 0xFF));
      file.put(static_cast<char>(ch >> 8));
    }
  }
};

/// @brief Factory for obtaining IEncodingHandler implementations by Format.
class EncodingHandlerFactory
{
public:
  static std::unique_ptr<IEncodingHandler> create(JSON::Format format, bool isWrite = false)
  {
    switch (format) {
    case JSON::Format::utf8:
      return std::make_unique<Utf8EncodingHandler>();
    case JSON::Format::utf8BOM:
      return std::make_unique<Utf8BOMEncodingHandler>();
    case JSON::Format::utf16BE:
      return std::make_unique<Utf16BEEncodingHandler>();
    case JSON::Format::utf16LE:
      return std::make_unique<Utf16LEEncodingHandler>();
    default:
      if (isWrite) {
        JSON_THROW(UnsupportedEncodingError("Unsupported JSON file format (Byte Order Mark) specified."));
      } else {
        JSON_THROW(UnsupportedEncodingError("Unsupported JSON file format (Byte Order Mark) encountered."));
      }
      return nullptr;
    }
    return nullptr;
  }
};

#endif // !JSON_LIB_NO_STDIO

} // namespace JSON_Lib
