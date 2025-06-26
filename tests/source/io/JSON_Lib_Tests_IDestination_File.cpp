#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check IDestination (File) interface.", "[JSON][IDestination][File]")
{
  SECTION("Create FileDestination.", "[JSON][IDestination][File][Construct]")
  {
    const std::string fileName{ generateRandomFileName() };
    REQUIRE_NOTHROW(FileDestination(fileName));
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination when file already exists.", "[JSON][IDestination][File][Construct]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    REQUIRE_NOTHROW(FileDestination(fileName));
    file.close();
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[JSON][IDestination][File][Construct]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    std::filesystem::path filePath{ fileName };
    REQUIRE_FALSE(!std::filesystem::exists(fileName));
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 0);
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination and add one character.", "[JSON][IDestination][File][Add]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    std::filesystem::path filePath{ fileName };
    file.add('t');
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 1);
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination, add an integer string and check result.", "[JSON][IDestination][File][Add]")
  {
    const std::string fileName{ generateRandomFileName() };

    FileDestination file{ fileName };
    std::filesystem::path filePath{ fileName };
    file.add("65767");
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(JSON::fromFile(fileName) == "65767");
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination, add to it, clear file and then add to it again and check result.",
    "[JSON][IDestination][File][Clear]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    std::filesystem::path filePath{ fileName };
    file.add("65767");
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(JSON::fromFile(fileName) == ("65767"));
    file.clear();
    REQUIRE(std::filesystem::file_size(filePath) == 0);
    file.add("65767");
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(JSON::fromFile(fileName) == ("65767"));
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination and add content with linefeeds.", "[JSON][IDestination][Buffer][Linefeed]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    std::filesystem::path filePath{ fileName };
    file.add("65767\n");
    file.add("22222\n");
    file.add("33333\n");
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 21);
    REQUIRE(JSON::fromFile(fileName) == ("65767\n22222\n33333\n"));
    std::filesystem::remove(fileName);
  }
}
