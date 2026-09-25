//
// Fuzz target for JSON_Lib parser and stringifier
// Compatible with LLVM libFuzzer and OSS-Fuzz
//

#include "JSON.hpp"
#include "JSON_IO.hpp"
#include <cstddef>
#include <cstdint>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
  if (size == 0) {
    return 0;
  }

  const std::string_view input(reinterpret_cast<const char *>(data), size);
  JSON_Lib::JSON json;

  try {
    JSON_Lib::BufferSource source{ input };
    json.parse(source);

    // If parsing succeeded, verify serialization consistency
    JSON_Lib::BufferDestination dest;
    json.stringify(dest);
  } catch (...) {
    // Parsing exceptions on malformed input are normal and expected
  }

  return 0;
}
