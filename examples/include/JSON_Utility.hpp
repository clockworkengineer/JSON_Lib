#pragma once

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

#include "JSON.hpp"
#include "JSON_Core.hpp"

class Utility
{
public:
  static std::vector<std::string> createJSONFileList()
  {
    std::vector<std::string> fileList;
    for (auto &file : std::filesystem::directory_iterator((std::filesystem::current_path() / "files"))) {
      if (file.path().extension() == ".json") { fileList.push_back(file.path().string()); }
    }
    sort(fileList.begin(), fileList.end());
    return (fileList);
  }
  static std::string createFileName(const std::string &torrentFileName, const std::string newExtension)
  {
    std::string newFileName = torrentFileName;
    return (newFileName.erase(newFileName.find(".json"), newFileName.length()) + newExtension);
  }
};