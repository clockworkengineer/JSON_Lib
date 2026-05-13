#pragma once

#include "implementation/common/JSON_Attributes.hpp"
#include "JSON_ErrorBase.hpp"

namespace JSON_Lib {

// ====================
// Forward declarations
// ====================
class IDestination;
struct Node;

// ==================================
// Interface for JSON stringification
// ==================================
/// @brief Interface for converting a Node tree into a JSON text stream.
///
/// Implement this interface to provide a custom serialisation format
/// (e.g. Bencode, XML, YAML).  Pass an instance via
/// @c std::unique_ptr<IStringify> to the @c JSON constructor.
class IStringify
{
public:
  /// @brief Exception type thrown by IStringify implementations.
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("IStringify", message)) {}
  };
  virtual ~IStringify() = default;

  /// @brief Serialise @p jNode to @p destination.
  /// @param jNode     Root of the Node tree to serialise.
  /// @param destination Output sink to receive the serialised bytes.
  /// @param indent    Current indentation depth (0 = compact, >0 = pretty-print).
  virtual void stringify(const Node &jNode, IDestination &destination, unsigned long indent) const = 0;

  /// @brief Return the current pretty-print indentation step.
  /// @return Number of spaces per indent level; 0 if not applicable.
  JSON_LIB_NODISCARD virtual long getIndent() const JSON_LIB_NOEXCEPT { return 0; }

  /// @brief Set the pretty-print indentation step.
  /// @param indent Number of spaces per indent level; must be >= 0.
  virtual void setIndent(long indent) {}
};

/// @brief Factory helper: create a @c std::unique_ptr<IStringify> of type @p T.
/// @tparam T A concrete class that derives from IStringify.
/// @return Owning pointer to a new instance of @p T.
template <typename T> std::unique_ptr<IStringify> makeStringify() {
  return std::make_unique<T>();
}
}// namespace JSON_Lib