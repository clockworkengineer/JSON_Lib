#include "JSON.hpp"
#include "JSON_Core.hpp"

namespace JSON_Lib {

Result<Node> IParser::parseResult(ISource &source)
{
  return {Status::Ok, std::make_unique<Node>(parse(source)), {}, {0, 0}};
}

} // namespace JSON_Lib
