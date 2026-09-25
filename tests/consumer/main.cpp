#include <JSON_Lib.hpp>
#include <iostream>
#include <string>

int main()
{
  try {
    JSON_Lib::JSON json;
    json.parse(JSON_Lib::BufferSource{ R"({"name":"JSON_Lib","status":"verified","version":1.2})" });

    JSON_Lib::BufferDestination destination;
    json.stringify(destination);

    if (destination.toString().empty()) {
      std::cerr << "Verification failed: stringification produced empty output." << std::endl;
      return 1;
    }

    std::cout << "Consumer verification succeeded: " << destination.toString() << std::endl;
    return 0;
  } catch (const std::exception &ex) {
    std::cerr << "Consumer verification exception: " << ex.what() << std::endl;
    return 1;
  }
}
