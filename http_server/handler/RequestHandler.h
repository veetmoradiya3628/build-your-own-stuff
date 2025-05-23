#pragma once
#include <string>

class RequestHandler
{
public:
    static std::string handle(const std::string &rawRequest);
};