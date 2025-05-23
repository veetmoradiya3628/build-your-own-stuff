#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 1) == -1)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Waiting for client...\n";

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
    {
        perror("accept");
        close(server_fd);
        return 1;
    }

    std::cout << "Client connected. Receiving data...\n";

    char buffer[1024];
    ssize_t bytes_received;
    while ((bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        std::cout << "Received : " << buffer << std::endl;
        send(client_fd, buffer, bytes_received, 0);
    }

    if (bytes_received == 0)
    {
        std::cout << "\nClient disconnected.\n";
    }
    else
    {
        perror("recv");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
