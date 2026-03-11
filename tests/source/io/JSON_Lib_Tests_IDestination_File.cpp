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
  SECTION("Create FileDestination and add content with linefeeds.", "[JSON][IDestination][File][Linefeed]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    std::filesystem::path filePath{ fileName };
    file.add("65767\n");
    file.add("22222\n");
    file.add("33333\n");
    file.close();
    // Each '\n' is expanded to '\r\n' on disk, giving 18 chars + 3 extra = 21
    REQUIRE(std::filesystem::file_size(filePath) == 21);
    REQUIRE(JSON::fromFile(fileName) == ("65767\n22222\n33333\n"));
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination, add char and verify content.", "[JSON][IDestination][File][Add]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    file.add('X');
    file.close();
    REQUIRE(JSON::fromFile(fileName) == "X");
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination, add a std::string_view and check result.", "[JSON][IDestination][File][Add]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    std::filesystem::path filePath{ fileName };
    std::string_view sv{ "hello" };
    file.add(sv);
    file.close();
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(JSON::fromFile(fileName) == "hello");
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination and check last() returns the most recently added character.",
    "[JSON][IDestination][File][Last]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    file.add("abc");
    REQUIRE(file.last() == 'c');
    file.add('Z');
    REQUIRE(file.last() == 'Z');
    file.close();
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination and check size() grows correctly.", "[JSON][IDestination][File][Size]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    REQUIRE(file.size() == 0);
    file.add("ab");
    REQUIRE(file.size() == 2);
    file.add('c');
    REQUIRE(file.size() == 3);
    file.close();
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination and check getFileName() returns the correct path.",
    "[JSON][IDestination][File][GetFileName]")
  {
    const std::string fileName{ generateRandomFileName() };
    FileDestination file{ fileName };
    REQUIRE(file.getFileName() == fileName);
    file.close();
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination and use with JSON stringify.", "[JSON][IDestination][File][Stringify]")
  {
    const std::string fileName{ generateRandomFileName() };
    const JSON json(R"({"key":42})");
    {
      FileDestination file{ fileName };
      json.stringify(file);
      file.close();
    }
    const std::string result{ JSON::fromFile(fileName) };
    REQUIRE_FALSE(result.empty());
    const JSON roundtrip(result);
    BufferDestination buffer;
    roundtrip.stringify(buffer);
    REQUIRE(result == buffer.toString());
    std::filesystem::remove(fileName);
  }
}
