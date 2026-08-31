//
// Class: JSON_Impl_File
//
// Description: JSON class implementation layer to read and write
// JSON files in a number of different formats. Note that these
// methods are all static and do not need a JSON object to invoke.
// For more information on byte marks and their meaning check out link
// https://en.wikipedia.org/wiki/Byte_order_mark.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON_Impl.hpp"
#include "implementation/file/JSON_FileIO.hpp"

namespace JSON_Lib {

#if !JSON_LIB_NO_STDIO

JSON::Format JSON_Impl::getFileFormat(const std::string_view &fileName)
{
  return JSON_FileIO::getFileFormat(fileName);
}

std::string JSON_Impl::fromFile(const std::string_view &fileName)
{
  return JSON_FileIO::fromFile(fileName);
}

void JSON_Impl::toFile(const std::string_view &fileName, const std::string_view &jsonString, const JSON::Format format)
{
  JSON_FileIO::toFile(fileName, jsonString, format);
}

#endif // !JSON_LIB_NO_STDIO
} // namespace JSON_Lib

