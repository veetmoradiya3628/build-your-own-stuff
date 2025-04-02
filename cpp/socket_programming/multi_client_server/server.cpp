#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

std::vector<int> clients;
std::mutex client_mutex;
int server_socket;

void broadcastMessage(const std::string &message, int sender_client_socket)
{
    std::lock_guard<std::mutex> lock(client_mutex);
    for (int client : clients)
    {
        if (client != sender_client_socket)
        {
            int length = htonl(message.size());
            send(client, &length, sizeof(length), 0);
            send(client, message.c_str(), message.length(), 0);
        }
    }
}

void handleClient(int client_socket)
{
    char buffer[BUFFER_SIZE];
    while (true)
    {
        int msgLength;
        int bytesReceived = recv(client_socket, &msgLength, sizeof(msgLength), 0);
        if (bytesReceived <= 0)
        {
            // Handle disconnection
            close(client_socket);
            return;
        }
        msgLength = ntohl(msgLength); // Convert from network to host byte order

        // Receive the actual message
        bytesReceived = recv(client_socket, buffer, msgLength, 0);
        buffer[bytesReceived] = '\0'; // Null terminate the message

        if (bytesReceived <= 0)
        {
            std::cout << "Client disconnected" << std::endl;
            std::lock_guard<std::mutex> lock(client_mutex);
            clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
            close(client_socket);
            return;
        }

        std::cout << "Client says: " << buffer << std::endl;
        broadcastMessage(buffer, client_socket);
    }
}

void shutdownServer(int signum)
{
    std::cout << "Shutting down server..." << std::endl;
    std::lock_guard<std::mutex> lock(client_mutex);
    for (int client : clients)
    {
        close(client);
    }
    close(server_socket);
    exit(0);
}

int main()
{
    signal(SIGINT, shutdownServer);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
        return -1;
    }

    if (listen(server_socket, 5) == -1)
    {
        std::cerr << "Error listening on socket" << std::endl;
        return -1;
    }

    std::cout << "Server is running on port " << PORT << std::endl;

    while (true)
    {
        sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr *)&client_addr, &client_size);

        if (client_socket == -1)
        {
            std::cerr << "Error accepting client" << std::endl;
            continue;
        }

        std::cout << "New client connected" << std::endl;
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            clients.push_back(client_socket);
        }

        std::thread clientThread(handleClient, client_socket);
        clientThread.detach();
    }

    return 0;
}
