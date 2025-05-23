#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <vector>
#include <algorithm>

const int PORT = 8080;
const int MAX_CLIENTS = 10;

int main()
{
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("socket");
        return 1;
    }
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(listener, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(listener);
        return 1;
    }
    if (listen(listener, SOMAXCONN) < 0)
    {
        perror("listen");
        close(listener);
        return 1;
    }
    std::cout << "Listening on port " << PORT << "...\n";

    // keeps the track of all connected clients
    std::vector<int> clients;

    while (true)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);

        FD_SET(listener, &read_fds);
        int max_fd = listener;

        for (int fd : clients)
        {
            FD_SET(fd, &read_fds);
            if (fd > max_fd)
                max_fd = fd;
        }

        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (activity < 0)
        {
            perror("select");
            break;
        }

        // New connection
        if (FD_ISSET(listener, &read_fds))
        {
            sockaddr_in client_addr;
            socklen_t addrlen = sizeof(client_addr);
            int new_fd = accept(listener, (sockaddr *)&client_addr, &addrlen);
            if (new_fd < 0)
            {
                perror("accept");
            }
            else
            {
                std::cout << "New client: FD = " << new_fd << "\n";
                clients.push_back(new_fd);
            }
        }

        // Existing clients
        char buffer[1024];
        std::vector<int> disconnected;

        for (int fd : clients)
        {
            if (FD_ISSET(fd, &read_fds))
            {
                ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
                if (bytes <= 0)
                {
                    std::cout << "Client disconnected: FD = " << fd << "\n";
                    close(fd);
                    disconnected.push_back(fd);
                }
                else
                {
                    buffer[bytes] = '\0';
                    std::cout << "From FD " << fd << ": " << buffer;
                    send(fd, buffer, bytes, 0); // echo back
                }
            }
        }

        // Remove disconnected clients
        for (int fd : disconnected)
        {
            clients.erase(std::remove(clients.begin(), clients.end(), fd), clients.end());
        }
    }

    close(listener);
    for (int fd : clients)
        close(fd);

    return 0;
}