#pragma once
#include <string>

class FileUtils
{
public:
    static std::string readFile(const std::string &path);
    static std::string getMimeType(const std::string& path);
};
