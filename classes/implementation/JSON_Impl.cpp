//
// Class: JSON_Impl
//
// Description: High level entry points in JSON class implementation layer.
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
// ===============
// PRIVATE METHODS
// ===============
// ==============
// PUBLIC METHODS
// ==============
/// <summary>
///  Get JSON_Lib version.
/// </summary>
std::string JSON_Impl::version() const
{
  std::stringstream versionString;
  versionString << "JSON_Lib Version  " << JSON_VERSION_MAJOR << "." << JSON_VERSION_MINOR << "." << JSON_VERSION_PATCH;
  return (versionString.str());
}
/// <summary>
/// Set translator for JSON strings.
/// </summary>
/// <param name=translator>Custom JSON string translator.</param>
void JSON_Impl::translator(ITranslator *translator)
{
  if (translator == nullptr) {
    m_translator = std::make_unique<JSON_Translator>(*m_converter);
  } else {
    m_translator.reset(translator);
  }
}
/// <summary>
/// Set converter for JSON strings.
/// </summary>
/// <param name=converter>Custom JSON string translator.</param>
void JSON_Impl::converter(IConverter *converter)
{
  if (converter == nullptr) {
    m_converter = std::make_unique<JSON_Converter>();
  } else {
    m_converter.reset(converter);
  }
}
/// <summary>
/// Create JNode structure by recursively parsing JSON on the source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
void JSON_Impl::parse(ISource &source) { m_jNodeRoot = parseJNodes(source); }
/// <summary>
/// Recursively parse JNode tree structure and building its JSON string (no whitespace)
/// in destination stream.
/// </summary>
/// <param name=destination>Destination stream for stringified JSON.</param>
void JSON_Impl::stringify(IDestination &destination) const
{
  if (m_jNodeRoot.isEmpty()) { throw Error("No JSON to stringify."); }
  stringifyJNodes(m_jNodeRoot, destination, 0);
}
/// <summary>
/// Recursively parse JNode tree structure and building its JSON string (pretty printed)
/// in destination stream.
/// </summary>
/// <param name=destination>Destination stream for printed JSON.</param>
void JSON_Impl::print(IDestination &destination) const
{
  if (m_jNodeRoot.isEmpty()) { throw Error("No JSON to print."); }
  stringifyJNodes(m_jNodeRoot, destination, m_indent);
}
/// <summary>
/// Strip all whitespace from a JSON source.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="destination">Destination for stripped JSON.</param>
void JSON_Impl::strip(ISource &source, IDestination &destination) const
{// Note: That it is assumed that the JSON on the source stream is valid with no errors.
  while (source.more()) {
    if (!source.isWS()) {
      destination.add(source.current());
      if (source.current() == '"') {
        source.next();
        while (source.more() && source.current() != '"') {
          if (source.current() == '\\') {
            destination.add(source.current());
            source.next();
          }
          destination.add(source.current());
          source.next();
        }
        destination.add(source.current());
      }
    }
    source.next();
  }
}
/// <summary>
/// Recursively traverse JNode structure calling IAction methods.
/// </summary>
/// <param name=action>Action methods to call during traversal.</param>
void JSON_Impl::traverse(IAction &action)
{
  if (m_jNodeRoot.isEmpty()) { throw Error("No JSON to traverse."); }
  traverseJNodes(m_jNodeRoot, action);
}
void JSON_Impl::traverse(IAction &action) const
{
  if (m_jNodeRoot.isEmpty()) { throw Error("No JSON to traverse."); }
  traverseJNodes(m_jNodeRoot, action);
}
/// <summary>
/// Return object entry for the passed in key creating a root object if it does not exist
/// or a placeholder for a new JNode to be created into if the key does not exist.
/// </summary>
/// <param name=key>Object entry (JNode) key.</param>
JNode &JSON_Impl::operator[](const std::string &key)
{
  try {
    if (m_jNodeRoot.isEmpty()) { m_jNodeRoot = JNode::make<Object>(); }
    return (m_jNodeRoot[key]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JRef<Object>(m_jNodeRoot).add(Object::Entry(key, JNode::make<Hole>()));
    return (m_jNodeRoot[key]);
  }
}
const JNode &JSON_Impl::operator[](const std::string &key) const { return ((m_jNodeRoot)[key]); }
/// <summary>
/// Return array entry for the passed in index creating a root array if it does not exist
/// or a placeholder(s) for a new JNode to be created into if the index(s) do not exist.
/// </summary>
/// <param name=index>Array entry (JNode) index.</param>
JNode &JSON_Impl::operator[](std::size_t index)
{
  try {
    if (m_jNodeRoot.isEmpty()) { m_jNodeRoot = JNode::make<Array>(); }
    return (m_jNodeRoot[index]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JRef<Array>(m_jNodeRoot).resize(index);
    return (m_jNodeRoot[index]);
  }
}
const JNode &JSON_Impl::operator[](std::size_t index) const { return ((m_jNodeRoot)[index]); }
/// <summary>
/// Return format of JSON file.
/// </summary>
/// <param name="jsonFileName">JSON file name</param>
/// <returns>JSON file format.</returns>
JSON::Format JSON_Impl::getFileFormat(const std::string &jsonFileName)
{
  uint32_t bom;
  std::ifstream jsonFile;
  jsonFile.open(jsonFileName, std::ios_base::binary);
  bom = static_cast<unsigned char>(jsonFile.get()) << 24;
  bom |= static_cast<unsigned char>(jsonFile.get()) << 16;
  bom |= static_cast<unsigned char>(jsonFile.get()) << 8;
  bom |= static_cast<unsigned char>(jsonFile.get());
  if (bom == 0x0000FEFF) { return (JSON::Format::utf32BE); }
  if (bom == 0xFFFE0000) { return (JSON::Format::utf32LE); }
  if ((bom & 0xFFFFFF00) == 0xEFBBBF00) { return (JSON::Format::utf8BOM); }
  if ((bom & 0xFFFF0000) == 0xFEFF0000) { return (JSON::Format::utf16BE); }
  if ((bom & 0xFFFF0000) == 0xFFFE0000) { return (JSON::Format::utf16LE); }
  return (JSON::Format::utf8);
}
/// <summary>
/// Open a JSON file, read its contents into a string buffer and return
/// the buffer. Note any CRLF in the source file are translated to just a
/// LF internally.
/// </summary>
/// <param name="jsonFileName">JSON file name</param>
/// <returns>JSON string.</returns>
const std::string JSON_Impl::fromFile(const std::string &jsonFileName)
{
  const char *kCRLF = "\x0D\x0A";
  const char *kLF = "\x0A";
  // Read in JSON
  std::ifstream jsonFile;
  jsonFile.open(jsonFileName, std::ios_base::binary);
  std::string translated;
  JSON::Format fileFormat = JSON::getFileFormat(jsonFileName);
  if (fileFormat == JSON::Format::utf8BOM) {
    std::ostringstream jsonFileBuffer;
    jsonFile.get();
    jsonFile.get();
    jsonFile.get();
    jsonFileBuffer << jsonFile.rdbuf();
    translated = jsonFileBuffer.str();
  } else if ((fileFormat == JSON::Format::utf16BE) || (fileFormat == JSON::Format::utf16LE)) {
    std::u16string utf16String;
    jsonFile.get();
    jsonFile.get();
    char16_t first = (jsonFile.get() << 8) | jsonFile.get();
    if ((first == '[') || (first == '{')) {
      utf16String.push_back(first);
      while (!jsonFile.eof()) {
        first = (jsonFile.get() << 8) | jsonFile.get();
        utf16String.push_back(first);
      }
      utf16String.pop_back();
    } else {
      jsonFile.unget();
      jsonFile.unget();
      first = jsonFile.get() | jsonFile.get() << 8;
      utf16String.push_back(first);
      while (!jsonFile.eof()) {
        first = jsonFile.get() | jsonFile.get() << 8;
        utf16String.push_back(first);
      }
      utf16String.pop_back();
    }
    translated = m_converter->toUtf8(utf16String);
  } else if (fileFormat == JSON::Format::utf8) {
    std::ostringstream jsonFileBuffer;
    jsonFileBuffer << jsonFile.rdbuf();
    translated = jsonFileBuffer.str();
  } else {
    throw Error("Unsupported JSON file format encountered.");
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
/// <param name="jsonFileName">JSON file name</param>
/// <param name="jsonString">JSON string</param>
void JSON_Impl::toFile(const std::string &jsonFileName, const std::string &jsonString)
{
  std::remove(jsonFileName.c_str());
  std::ofstream jsonFile;
  jsonFile.open(jsonFileName, std::ios::binary);
  jsonFile << jsonString;
  jsonFile.close();
}
}// namespace JSON_Lib
