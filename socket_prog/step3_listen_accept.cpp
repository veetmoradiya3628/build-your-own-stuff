#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return 1;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);       // port 8080
    addr.sin_addr.s_addr = INADDR_ANY; // any local IP

    if (bind(server_fd, (sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }
    std::cout << "Server is listening on 8080" << std::endl;

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
    {
        perror("accept");
        close(server_fd);
        return 1;
    }
    std::cout << "Client connected! File descriptor: " << client_fd << "\n";

    close(client_fd);
    close(server_fd);
    return 0;
}