//
// Class: JSON_Impl_File
//
// Description: JSON class implementation layer to read and write
// JSON files in a number of different formats. Note that these
// methods are all static and do not need a JSON object to invoke.
// For more information on byte marks and their meaning check out link
// https://en.wikipedia.org/wiki/Byte_order_mark.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON.hpp"
#include "JSON_Impl.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ===========================
// PRIVATE TYPES AND CONSTANTS
// ===========================
// ==========================
// PUBLIC TYPES AND CONSTANTS
// ==========================
// ========================
// PRIVATE STATIC VARIABLES
// ========================
// =======================
// PUBLIC STATIC VARIABLES
// =======================
// =================
// PRIVATE FUNCTIONS
// =================
/// <summary>
/// Write JSON string to a file stream.
/// </summary>
/// <param name="jsonFile">JSON file stream</param>
/// <param name="jsonString">JSON string</param>
/// <param name="format">JSON file format</param>
void writeJSONString(std::ofstream &jsonFile, const std::string &jsonString) { jsonFile << jsonString; }
void writeJSONString(std::ofstream &jsonFile, const std::u16string &jsonString, JSON::Format format)
{
  if (format == JSON::Format::utf16BE) {
    jsonFile << static_cast<unsigned char>(0xFE) << static_cast<unsigned char>(0xFF);
    for (auto ch : jsonString) {
      jsonFile.put(static_cast<unsigned char>(ch >> 8));
      jsonFile.put(static_cast<unsigned char>(ch & 0xFF));
    }
  } else if (format == JSON::Format::utf16LE) {
    jsonFile << static_cast<unsigned char>(0xFF) << static_cast<unsigned char>(0xFE);
    for (auto ch : jsonString) {
      jsonFile.put(static_cast<unsigned char>(ch & 0xFF));
      jsonFile.put(static_cast<unsigned char>(ch >> 8));
    }
  }
}
/// <summary>
/// Read JSON string from a file stream.
/// </summary>
/// <param name="jsonFile">JSON file stream</param>
/// <param name="format">JSON file format</param>
/// <returns>JSON string.</returns>
std::string readJSONString(std::ifstream &jsonFile)
{
  std::ostringstream jsonFileBuffer;
  jsonFileBuffer << jsonFile.rdbuf();
  return (jsonFileBuffer.str());
}
const std::u16string readJSONString(std::ifstream &jsonFile, JSON::Format format)
{
  std::u16string utf16String;
  // Move past byte order mark
  jsonFile.seekg(2);
  if (format == JSON::Format::utf16BE)
    while (true) {
      char16_t char16Bit = static_cast<char16_t>((jsonFile.get() << 8) | jsonFile.get());
      if (jsonFile.eof()) break;
      utf16String.push_back(char16Bit);
    }
  else if (format == JSON::Format::utf16LE) {
    while (true) {
      char16_t char16Bit = static_cast<char16_t>(jsonFile.get() | jsonFile.get() << 8);
      if (jsonFile.eof()) break;
      utf16String.push_back(char16Bit);
    }
  }
  return (utf16String);
}
// ===============
// PRIVATE METHODS
// ===============
// ==============
// PUBLIC METHODS
// ==============
/// <summary>
/// Return format of JSON file after checking for any byte order marks at
/// the beginning of the JSON file.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <returns>JSON file format.</returns>
JSON::Format JSON_Impl::getFileFormat(const std::string &fileName)
{
  uint32_t byteOrderMark;
  std::ifstream jsonFile{ fileName, std::ios_base::binary };
  byteOrderMark = static_cast<unsigned char>(jsonFile.get()) << 24;
  byteOrderMark |= static_cast<unsigned char>(jsonFile.get()) << 16;
  byteOrderMark |= static_cast<unsigned char>(jsonFile.get()) << 8;
  byteOrderMark |= static_cast<unsigned char>(jsonFile.get());
  if (byteOrderMark == 0x0000FEFF) { return (JSON::Format::utf32BE); }
  if (byteOrderMark == 0xFFFE0000) { return (JSON::Format::utf32LE); }
  if ((byteOrderMark & 0xFFFFFF00) == 0xEFBBBF00) { return (JSON::Format::utf8BOM); }
  if ((byteOrderMark & 0xFFFF0000) == 0xFEFF0000) { return (JSON::Format::utf16BE); }
  if ((byteOrderMark & 0xFFFF0000) == 0xFFFE0000) { return (JSON::Format::utf16LE); }
  return (JSON::Format::utf8);
}
/// <summary>
/// Open a JSON file, read its contents into a string buffer and return
/// the buffer. Note any CRLF in the source file are translated to just a
/// LF internally.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <returns>JSON string.</returns>
const std::string JSON_Impl::fromFile(const std::string &fileName)
{
  const char *kCRLF = "\x0D\x0A";
  const char *kLF = "\x0A";
  // Initialise converter
  intializeConverter();
  // Get file format
  JSON::Format format = JSON::getFileFormat(fileName);
  // Read in JSON
  std::ifstream jsonFile{ fileName, std::ios_base::binary };
  std::string translated;
  switch (format) {
  case JSON::Format::utf8BOM:
    jsonFile.seekg(3);// Move past byte order mark
  case JSON::Format::utf8:
    translated = readJSONString(jsonFile);
    break;
  case JSON::Format::utf16BE:
  case JSON::Format::utf16LE:
    translated = m_converter->toUtf8(readJSONString(jsonFile, format));
    break;
  default:
    throw Error("Unsupported JSON file format (Byte Order Mark) encountered.");
  }
  // Translate CRLF -> LF
  size_t pos = translated.find(kCRLF);
  while (pos != std::string::npos) {
    translated.replace(pos, 2, kLF);
    pos = translated.find(kCRLF, pos + 1);
  }
  return (translated);
}
/// <summary>
/// Create an JSON file and write JSON string to it.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <param name="jsonString">JSON string</param>
/// <param name="format">JSON file format</param>
void JSON_Impl::toFile(const std::string &fileName, const std::string &jsonString, JSON::Format format)
{
  // Initialise converter
  intializeConverter();
  std::ofstream jsonFile{ fileName, std::ios::binary };
  switch (format) {
  case JSON::Format::utf8BOM:
    jsonFile << static_cast<unsigned char>(0xEF) << static_cast<unsigned char>(0xBB)
             << static_cast<unsigned char>(0xBF);
  case JSON::Format::utf8:
    writeJSONString(jsonFile, jsonString);
    break;
  case JSON::Format::utf16BE:
  case JSON::Format::utf16LE:
    writeJSONString(jsonFile, m_converter->toUtf16(jsonString), format);
    break;
  default:
    throw Error("Unsupported JSON file format (Byte Order Mark) specified.");
  }
  jsonFile.close();
}
}// namespace JSON_Lib
