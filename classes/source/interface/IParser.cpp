#include "JSON.hpp"
#include "JSON_Node_Core.hpp"

namespace JSON_Lib {

Result<Node> IParser::parseResult(ISource &source)
{
  try {
    return Result<Node>::ok(parse(source));
  } catch (const SyntaxError &ex) {
    return Result<Node>::error(Status::SyntaxError, ex.what(), source.getPosition());
  } catch (const UnsupportedEncodingError &ex) {
    return Result<Node>::error(Status::UnsupportedEncoding, ex.what(), source.getPosition());
  } catch (const Error &ex) {
    return Result<Node>::error(Status::InvalidInput, ex.what(), source.getPosition());
  } catch (const std::exception &ex) {
    return Result<Node>::error(Status::UnknownError, ex.what(), source.getPosition());
  } catch (...) {
    return Result<Node>::error(Status::UnknownError, "Unknown exception during parse.", source.getPosition());
  }
}

} // namespace JSON_Lib
