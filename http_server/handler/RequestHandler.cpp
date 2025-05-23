#include "RequestHandler.h"
#include "../utils/FileUtils.h"
#include <sstream>
#include <iostream>

std::string RequestHandler::handle(const std::string &rawRequest)
{
    std::istringstream ss(rawRequest);
    std::string method, path, http_version;

    ss >> method >> path >> http_version;

    if (method == "GET" && path == "/")
    {
        std::string html = FileUtils::readFile("static/index.html");
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
               std::to_string(html.size()) + "\r\n\r\n" + html;
    }

    if (method == "POST" && path == "/submit")
    {
        std::string body = rawRequest.substr(rawRequest.find("\r\n\r\n") + 4);
        return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nReceived POST data:\n" + body;
    }

    return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found";
}