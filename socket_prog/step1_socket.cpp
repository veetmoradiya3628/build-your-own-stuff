#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;
int main()
{
    // Create socket: AF_INET = IPv4, SOCK_STREAM = TCP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        cerr << "failed to create socket" << endl;
        return 1;
    }
    cout << "Socket created. File descriptor : " << sockfd << endl;
    close(sockfd);
    return 0;
}