#pragma once

#include <memory>
#include "implementation/common/JSON_Error.hpp"

namespace JSON_Lib {

// ====================
// Forward declarations
// ====================

class ISource;
struct Node;

// =========================
// Interface for JSON parser
// =========================
/// @brief Interface for a JSON parser that converts a source stream into a Node tree.
///
/// Implement this interface to provide a custom parsing strategy.  Pass an
/// instance via @c std::unique_ptr<IParser> to the @c JSON constructor.
class IParser
{
public:
  /// @brief Exception type thrown when parsing fails.
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("IParser", message)) {}
  };
  virtual ~IParser() = default;

  /// @brief Parse JSON from @p source and return the root Node.
  /// @param source The input stream to read from.
  /// @return The root Node of the parsed JSON tree.
  /// @throws IParser::Error on malformed input.
  virtual Node parse(ISource &source) = 0;

  /// @brief Parse JSON from @p source and return a Result wrapping the root Node.
  ///
  /// The default implementation calls @c parse() and converts exceptions to a
  /// failed @c Result.  Override to provide a no-throw path from scratch.
  /// @param source The input stream to read from.
  /// @return A Result containing the root Node on success, or an error status.
  virtual Result<Node> parseResult(ISource &source);
};
}// namespace JSON_Lib