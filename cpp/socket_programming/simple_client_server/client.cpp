#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

// Function to receive a complete message
std::string receiveMessage(int socket) {
    int length;
    recv(socket, &length, sizeof(length), 0);
    length = ntohl(length);

    char *buffer = new char[length + 1];
    int received = recv(socket, buffer, length, 0);
    buffer[received] = '\0';

    std::string message(buffer);
    delete[] buffer;
    return message;
}

// Function to send a message
void sendMessage(int socket, const std::string &message) {
    int length = htonl(message.size());
    send(socket, &length, sizeof(length), 0);
    send(socket, message.c_str(), message.size(), 0);
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));

    while (true) {
        std::string msg;
        std::cout << "Enter message: ";
        std::getline(std::cin, msg);

        sendMessage(sock, msg);
        if (msg == "exit") break;

        std::string response = receiveMessage(sock);
        std::cout << "Server -> " << response << "\n";
    }

    close(sock);
    return 0;
}
