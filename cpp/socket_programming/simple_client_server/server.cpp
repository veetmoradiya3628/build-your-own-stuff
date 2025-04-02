#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

// Function to receive a complete message
std::string receiveMessage(int clientSocket) {
    int length;
    recv(clientSocket, &length, sizeof(length), 0); // Receive message length
    length = ntohl(length); // Convert to host byte order

    char *buffer = new char[length + 1];
    int received = recv(clientSocket, buffer, length, 0);
    buffer[received] = '\0';

    std::string message(buffer);
    delete[] buffer;
    return message;
}

// Function to send a message
void sendMessage(int clientSocket, const std::string &message) {
    int length = htonl(message.size()); // Convert to network byte order
    send(clientSocket, &length, sizeof(length), 0);
    send(clientSocket, message.c_str(), message.size(), 0);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server waiting for connection...\n";
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    while (true) {
        std::string msg = receiveMessage(new_socket);
        if (msg == "exit") break;
        std::cout << "Client: " << msg << "\n";

        std::string response = "Received: " + msg;
        sendMessage(new_socket, response);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
