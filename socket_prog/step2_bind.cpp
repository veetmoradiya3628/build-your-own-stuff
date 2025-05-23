#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    // 1. Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        return 1;
    }

    // 2. Set address structure
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;                // IPv4
    addr.sin_port = htons(8080);              // Port 8080 (network byte order)
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all local interfaces (127.0.0.1, eth0, etc.)

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        close(sockfd);
        return 1;
    }

    std::cout << "Socket bound to port 8080!\n";

    close(sockfd);
    return 0;
}