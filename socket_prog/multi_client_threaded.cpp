#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void handle_client(int client_fd)
{
    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = recv(client_fd, buffer, sizeof(buffer), 0)) > 0)
    {
        send(client_fd, buffer, bytes_read, 0); // Echo back
    }

    std::cout << "Client disconnected.\n";
    close(client_fd);
}

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

    if (listen(server_fd, 10) == -1)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port 8080...\n";

    while (true)
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
        if (client_fd == -1)
        {
            perror("accept");
            continue;
        }

        std::cout << "New client connected. FD: " << client_fd << "\n";
        std::thread t(handle_client, client_fd);
        t.detach(); // Detach thread so it runs independently
    }

    close(server_fd);
    return 0;
}
