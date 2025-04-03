#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <algorithm>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

std::vector<int> clients;
std::mutex client_mutex;
std::map<std::string, int> client_mapp;
int server_socket;

enum EventType
{
    LOGIN,
    DIRECT,
    BROADCAST,
    LOGOUT,
    ONLINE_USERS,
    INVALID
};

std::map<std::string, EventType> event_map = {
    {"login", LOGIN},
    {"direct", DIRECT},
    {"broadcast", BROADCAST},
    {"logout", LOGOUT},
    {"online-users", ONLINE_USERS}};

void sendMessageToClient(int client_socket, const std::string &message)
{
    int length = htonl(message.size());
    send(client_socket, &length, sizeof(length), 0);
    send(client_socket, message.c_str(), message.length(), 0);
}

void broadcastMessage(const std::string &message, int sender_client_socket)
{
    std::lock_guard<std::mutex> lock(client_mutex);
    for (int client : clients)
    {
        if (client != sender_client_socket)
        {
            sendMessageToClient(client, "Broadcast: " + message);
        }
    }
    sendMessageToClient(sender_client_socket, "Broadcast sent successfully.");
}

void handleLoginClient(const std::string &username, int sender_client_socket)
{
    client_mapp[username] = sender_client_socket;
    sendMessageToClient(sender_client_socket, "Login successful.");
}

void handleLogoutClient(int sender_client_socket)
{
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        for (auto it = client_mapp.begin(); it != client_mapp.end();)
        {
            if (it->second == sender_client_socket)
            {
                it = client_mapp.erase(it);
            }
            else
            {
                ++it;
            }
        }
        clients.erase(std::remove(clients.begin(), clients.end(), sender_client_socket), clients.end());
    }
    sendMessageToClient(sender_client_socket, "Logout successful.");
    close(sender_client_socket);
}

void handleOnlineUsers(int sender_client_socket)
{
    std::lock_guard<std::mutex> lock(client_mutex);
    std::string online_users = "Online Users:\n";
    for (const auto &client : client_mapp)
    {
        online_users += client.first + "\n";
    }
    sendMessageToClient(sender_client_socket, online_users);
}

void handleDirectMessage(const std::string &event_data, int sender_client_socket)
{
    size_t delimiter_pos = event_data.find(':');
    if (delimiter_pos != std::string::npos)
    {
        std::string rec_username = event_data.substr(0, delimiter_pos);
        std::string message = event_data.substr(delimiter_pos + 1);
        if (client_mapp.find(rec_username) != client_mapp.end())
        {
            sendMessageToClient(client_mapp[rec_username], "Direct message from sender: " + message);
            sendMessageToClient(sender_client_socket, "Direct message sent successfully.");
        }
        else
        {
            sendMessageToClient(sender_client_socket, "Error: Receiver not found or offline.");
        }
    }
    else
    {
        sendMessageToClient(sender_client_socket, "Error: Invalid message format.");
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
            handleLogoutClient(client_socket);
            return;
        }
        msgLength = ntohl(msgLength);

        bytesReceived = recv(client_socket, buffer, msgLength, 0);
        if (bytesReceived <= 0)
        {
            handleLogoutClient(client_socket);
            return;
        }
        buffer[bytesReceived] = '\0';

        std::string message(buffer);
        size_t delimiter_pos = message.find(':');
        std::string event = (delimiter_pos != std::string::npos) ? message.substr(0, delimiter_pos) : message;
        std::string event_data = (delimiter_pos != std::string::npos) ? message.substr(delimiter_pos + 1) : "";

        EventType event_type = event_map.count(event) ? event_map[event] : INVALID;

        switch (event_type)
        {
        case LOGIN:
            handleLoginClient(event_data, client_socket);
            break;
        case DIRECT:
            handleDirectMessage(event_data, client_socket);
            break;
        case BROADCAST:
            broadcastMessage(event_data, client_socket);
            break;
        case LOGOUT:
            handleLogoutClient(client_socket);
            return;
        case ONLINE_USERS:
            handleOnlineUsers(client_socket);
            break;
        default:
            sendMessageToClient(client_socket, "Error: Invalid event type.");
            break;
        }
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
