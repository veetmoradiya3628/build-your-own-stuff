#include "HttpServer.h"
#include "../handler/RequestHandler.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <thread>

HttpServer::HttpServer(int port) : port(port) {}

void HttpServer::start()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        return;
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        return;
    }

    std::cout << "HTTP server running on port " << port << "...\n";
    while (true)
    {
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr *)&client_addr, &len);
        if (client_fd >= 0)
        {
            std::thread([this, client_fd]()
                        { handle_client(client_fd); })
                .detach();
        }
    }
}

void HttpServer::handle_client(int client_fd)
{
    std::string request = read_request(client_fd);
    std::string response = RequestHandler::handle(request);
    send_response(client_fd, response);
    close(client_fd);
}

std::string HttpServer::read_request(int client_fd)
{
    char buffer[4096];
    ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        return std::string(buffer);
    }
    return "";
}

void HttpServer::send_response(int client_fd, const std::string &response)
{
    send(client_fd, response.c_str(), response.size(), 0);
}