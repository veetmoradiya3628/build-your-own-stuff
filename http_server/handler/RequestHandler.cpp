#include "RequestHandler.h"
#include "../utils/FileUtils.h"
#include <sstream>
#include <iostream>
#include <filesystem>

std::string RequestHandler::handle(const std::string &rawRequest)
{
    std::istringstream ss(rawRequest);
    std::string method, path, http_version;
    ss >> method >> path >> http_version;

    // Map "/" → "/index.html"
    if (path == "/")
    {
        path = "/index.html";
    }

    // Build full path
    std::string fullPath = "static" + path;
    std::string content = FileUtils::readFile(fullPath);

    if (content.empty())
    {
        return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile Not Found";
    }

    std::string mime = FileUtils::getMimeType(fullPath);

    return "HTTP/1.1 200 OK\r\nContent-Type: " + mime +
           "\r\nContent-Length: " + std::to_string(content.size()) +
           "\r\n\r\n" + content;
}
