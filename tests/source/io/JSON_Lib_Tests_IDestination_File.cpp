#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check IDestination (File) interface.", "[JSON][IDestination][File]")
{
  const std::string testFileName{ prefixPath(kGeneratedJSONFile) };
  SECTION("Create FileDestination.", "[JSON][IDestination][File][Construct]")
  {
    std::filesystem::remove(testFileName);
    REQUIRE_NOTHROW(FileDestination(testFileName));
  }
  SECTION("Create FileDestination when file already exists.", "[JSON][IDestination][File][Construct]")
  {
    FileDestination file{ testFileName };
    REQUIRE_NOTHROW(FileDestination(testFileName));
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[JSON][IDestination][File][Construct]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    REQUIRE_FALSE(!std::filesystem::exists(testFileName));
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.", "[JSON][IDestination][File][Add]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add('t');
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an integer string and check result.", "[JSON][IDestination][File][Add]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(JSON::fromFile(testFileName) == "65767");
  }
  SECTION("Create FileDestination, add to it, clear file and then add to it again and check result.",
    "[JSON][IDestination][File][Clear]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(JSON::fromFile(testFileName) == ("65767"));
    file.clear();
    REQUIRE(std::filesystem::file_size(filePath) == 0);
    file.add("65767");
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(JSON::fromFile(testFileName) == ("65767"));
  }
  SECTION("Create FileDestination and add content with linefeeds.", "[JSON][IDestination][Buffer][Linefeed]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767\n");
    file.add("22222\n");
    file.add("33333\n");
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 21);
    REQUIRE(JSON::fromFile(testFileName) == ("65767\n22222\n33333\n"));
  }
}
