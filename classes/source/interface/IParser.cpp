#include "JSON.hpp"
#include "JSON_Node_Core.hpp"

namespace JSON_Lib {

Result<Node> IParser::parseResult(ISource &source)
{
  try {
    return {Status::Ok, std::make_unique<Node>(parse(source)), {}, {0, 0}};
  } catch (const SyntaxError &ex) {
    return {Status::SyntaxError, nullptr, ex.what(), source.getPosition()};
  } catch (const std::exception &ex) {
    return {Status::UnknownError, nullptr, ex.what(), source.getPosition()};
  } catch (...) {
    return {Status::UnknownError, nullptr, "Unknown exception during parse.", source.getPosition()};
  }
}

} // namespace JSON_Lib
