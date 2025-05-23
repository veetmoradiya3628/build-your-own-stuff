#include "FileUtils.h"
#include <fstream>
#include <sstream>

std::string FileUtils::readFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return "<h1>File not found</h1>";
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
