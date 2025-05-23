#pragma once
#include <string>

class HttpServer
{
public:
    HttpServer(int port);
    void start();

private:
    int port;
    int server_fd;

    void handle_client(int client_id);
    std::string read_request(int client_id);
    void send_response(int client_id, const std::string &response);
};