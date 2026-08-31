//
// Class: JSON_FileIO
//
// Description: Dedicated service layer for reading/writing JSON files with BOM format detection.
//

#include "implementation/file/JSON_FileIO.hpp"
#include "implementation/io/encoding/EncodingHandlers.hpp"
#include "JSON_Throw.hpp"

#if !JSON_LIB_NO_STDIO
#include <fstream>
#endif

namespace JSON_Lib {

#if !JSON_LIB_NO_STDIO

JSON::Format JSON_FileIO::getFileFormat(const std::string_view &fileName)
{
  std::ifstream jsonFile{ fileName.data(), std::ios_base::binary };
  if (!jsonFile.is_open()) {
    return JSON::Format::utf8;
  }
  const unsigned char first  = static_cast<unsigned char>(jsonFile.get());
  const unsigned char second = static_cast<unsigned char>(jsonFile.get());
  const unsigned char third  = static_cast<unsigned char>(jsonFile.get());
  const unsigned char fourth = static_cast<unsigned char>(jsonFile.get());

  if (first == 0x00u && second == 0x00u && third == 0xFEu && fourth == 0xFFu) { return JSON::Format::utf32BE; }
  if (first == 0xFFu && second == 0xFEu && third == 0x00u && fourth == 0x00u) { return JSON::Format::utf32LE; }
  if (first == 0xEFu && second == 0xBBu) {
    if (third != 0xBFu) {
      JSON_THROW(UnsupportedEncodingError("Invalid UTF-8 Byte Order Mark sequence detected."));
    }
    return JSON::Format::utf8BOM;
  }
  if (first == 0xFEu && second == 0xFFu) { return JSON::Format::utf16BE; }
  if (first == 0xFFu && second == 0xFEu) { return JSON::Format::utf16LE; }
  return JSON::Format::utf8;
}

std::string JSON_FileIO::fromFile(const std::string_view &fileName)
{
  constexpr auto kCRLF = "\x0D\x0A";
  const JSON::Format format = getFileFormat(fileName);

  std::ifstream jsonFile{ fileName.data(), std::ios_base::binary };
  if (!jsonFile.is_open()) {
    return {};
  }

  auto handler = EncodingHandlerFactory::create(format);
  std::string translated = handler->read(jsonFile);
  jsonFile.close();

  // Translate CRLF -> LF
  size_t pos = translated.find(kCRLF);
  while (pos != std::string::npos) {
    constexpr auto kLF = "\x0A";
    translated.replace(pos, 2, kLF);
    pos = translated.find(kCRLF, pos + 1);
  }
  return translated;
}

void JSON_FileIO::toFile(const std::string_view &fileName, const std::string_view &jsonString, const JSON::Format format)
{
  std::ofstream jsonFile{ fileName.data(), std::ios::binary };
  if (!jsonFile.is_open()) {
    JSON_THROW(Error("File output stream failed to open or cannot be created."));
  }

  auto handler = EncodingHandlerFactory::create(format, true);
  handler->write(jsonFile, jsonString);
  jsonFile.close();
}

#endif // !JSON_LIB_NO_STDIO

} // namespace JSON_Lib
