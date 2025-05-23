#include "FileUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool ends_with(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string FileUtils::readFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Cannot open file: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string FileUtils::getMimeType(const std::string &path)
{
    if (ends_with(path, ".html"))
        return "text/html";
    if (ends_with(path, ".css"))
        return "text/css";
    if (ends_with(path, ".js"))
        return "application/javascript";
    if (ends_with(path, ".png"))
        return "image/png";
    if (ends_with(path, ".jpg"))
        return "image/jpeg";
    if (ends_with(path, ".ico"))
        return "image/x-icon";
    return "text/plain";
}
