#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Bind socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        return 1;
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed\n";
        return 1;
    }
    std::cout << "Server is listening on port " << PORT << "...\n";

    // Accept client connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0)
    {
        std::cerr << "Accept failed\n";
        return 1;
    }

    // Read data from client
    read(new_socket, buffer, 1024);
    std::cout << "Received: " << buffer << std::endl;

    // Send response
    send(new_socket, buffer, strlen(buffer), 0);
    std::cout << "Echo message sent\n";

    // Close socket
    close(new_socket);
    close(server_fd);
    return 0;
}
