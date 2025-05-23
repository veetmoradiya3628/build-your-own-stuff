#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_EVENTS 1024

int make_socket_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return 1;
    }

    make_socket_non_blocking(server_fd);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) < 0)
    {
        perror("listen");
        return 1;
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        return 1;
    }

    epoll_event event{};
    event.data.fd = server_fd;
    event.events = EPOLLIN;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1)
    {
        perror("epoll_ctl: listen_fd");
        return 1;
    }

    epoll_event events[MAX_EVENTS];

    std::cout << "Server using epoll() on port " << PORT << "...\n";

    while (true)
    {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n < 0)
        {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; i++)
        {
            if (events[i].data.fd == server_fd)
            {
                // New connection
                sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);
                int client_fd = accept(server_fd, (sockaddr *)&client_addr, &len);
                if (client_fd != -1)
                {
                    make_socket_non_blocking(client_fd);
                    epoll_event client_event{};
                    client_event.data.fd = client_fd;
                    client_event.events = EPOLLIN;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event);
                    std::cout << "New client connected. FD: " << client_fd << "\n";
                }
            }
            else
            {
                // Client data
                int client_fd = events[i].data.fd;
                char buffer[1024];
                ssize_t bytes = recv(client_fd, buffer, sizeof(buffer), 0);
                if (bytes <= 0)
                {
                    std::cout << "Client disconnected: FD = " << client_fd << "\n";
                    close(client_fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
                }
                else
                {
                    send(client_fd, buffer, bytes, 0); // echo back
                }
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}
