#include <iostream>
#include "JSON.hpp"
#include "implementation/io/JSON_Sources.hpp"
#include "implementation/io/JSON_Destinations.hpp"

int main()
{
    using namespace JSON_Lib;

    const std::string jsonInput = R"({"device":"sensor","enabled":true,"threshold":42})";
    EmbeddedJSON embedded;

    // Parse input from an in-memory buffer.
    embedded.parse(BufferSource{jsonInput});

    // Serialize into a fixed-size output buffer.
    FixedBufferDestination<128> destination;
    embedded.stringify(destination);

    std::cout << "Embedded build: " << std::boolalpha << EmbeddedJSON::isEmbeddedBuild() << "\n";
    std::cout << "Embedded string limit: " << EmbeddedJSON::Limits::maxStringLength() << "\n";
    std::cout << "Parser depth limit: " << EmbeddedJSON::Limits::maxParserDepth() << "\n";
    std::cout << "Serialized JSON: " << destination.toString() << "\n";
    return 0;
}
