#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

// Function to receive a complete message
void receiveMessages(int socket)
{
    while (true)
    {
        int length;
        int received = recv(socket, &length, sizeof(length), 0);
        if (received <= 0)
        {
            std::cout << "Server disconnected." << std::endl;
            close(socket);
            exit(0);
        }

        length = ntohl(length);
        char *buffer = new char[length + 1];
        received = recv(socket, buffer, length, 0);
        if (received <= 0)
        {
            std::cout << "Error receiving message." << std::endl;
            delete[] buffer;
            continue;
        }

        buffer[received] = '\0';
        std::cout << "\n[Server Broadcast]: " << buffer << "\nEnter message: ";
        std::cout.flush();
        delete[] buffer;
    }
}

// Function to send a message
void sendMessage(int socket, const std::string &message)
{
    int length = htonl(message.size());
    send(socket, &length, sizeof(length), 0);
    send(socket, message.c_str(), message.size(), 0);
}

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to server." << std::endl;

    // Start a separate thread to listen for incoming messages
    std::thread receiverThread(receiveMessages, sock);
    receiverThread.detach();

    // Main loop to send messages
    while (true)
    {
        std::string msg;
        std::cout << "Enter message: ";
        std::getline(std::cin, msg);

        sendMessage(sock, msg);
        if (msg == "exit")
            break;
    }

    close(sock);
    return 0;
}
