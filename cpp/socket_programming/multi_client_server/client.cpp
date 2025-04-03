#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

std::string username;

// Function to receive a complete message
void receiveMessages(int socket)
{
    while (true)
    {
        int length;
        int received = recv(socket, &length, sizeof(length), 0);
        if (received <= 0)
        {
            std::cout << "\n[INFO]: Server disconnected." << std::endl;
            close(socket);
            exit(0);
        }

        length = ntohl(length);
        char *buffer = new char[length + 1];
        received = recv(socket, buffer, length, 0);
        if (received <= 0)
        {
            std::cout << "\n[ERROR]: Error receiving message." << std::endl;
            delete[] buffer;
            continue;
        }

        buffer[received] = '\0';
        std::cout << "\n[Server Message]: " << buffer << "\nEnter message: ";
        std::cout.flush();
        delete[] buffer;
    }
}

// Function to send a message
void sendMessage(int socket, const std::string &message)
{
    int length = htonl(message.size());
    if (send(socket, &length, sizeof(length), 0) <= 0)
    {
        std::cerr << "\n[ERROR]: Failed to send message length." << std::endl;
        return;
    }
    if (send(socket, message.c_str(), message.size(), 0) <= 0)
    {
        std::cerr << "\n[ERROR]: Failed to send message content." << std::endl;
    }
    else
    {
        std::cout << "\n[INFO]: Message sent successfully." << std::endl;
    }
}

// Function to display client startup instructions
void displayClientInstructions()
{
    std::cout << "Client Command Help:\n";
    std::cout << "1. online-users              : Get the list of currently online users.\n";
    std::cout << "2. direct:<receiver-username>:<message> : Send a direct message to a specific user.\n";
    std::cout << "3. broadcast:<message>       : Send a message to all connected users.\n";
    std::cout << "4. logout                    : Log out from the server.\n";
    std::cout << "-------------------------------------------------------------\n";
}

// Function to initialize client
void clientStartUp()
{
    displayClientInstructions();
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    if (username.empty())
    {
        std::cerr << "[ERROR]: Username cannot be empty. Exiting..." << std::endl;
        exit(1);
    }
}

// Main function
int main()
{
    clientStartUp();

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "[ERROR]: Socket creation failed." << std::endl;
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
    {
        std::cerr << "[ERROR]: Invalid server IP address." << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "[ERROR]: Connection to server failed." << std::endl;
        return -1;
    }

    std::cout << "[INFO]: Connected to server." << std::endl;

    // Send login message with username
    sendMessage(sock, "login:" + username);

    // Start a separate thread to listen for incoming messages
    std::thread receiverThread(receiveMessages, sock);
    receiverThread.detach();

    // Main loop to send messages
    while (true)
    {
        std::string msg;
        std::cout << "Enter message: ";
        std::getline(std::cin, msg);

        if (msg.empty())
        {
            std::cout << "[WARNING]: Message cannot be empty. Try again." << std::endl;
            continue;
        }

        sendMessage(sock, msg);

        if (msg == "logout")
        {
            std::cout << "[INFO]: Logging out..." << std::endl;
            break;
        }
    }

    close(sock);
    std::cout << "[INFO]: Disconnected from server. Goodbye!" << std::endl;
    return 0;
}
