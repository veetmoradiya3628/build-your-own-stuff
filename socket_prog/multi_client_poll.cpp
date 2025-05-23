#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

const int PORT = 8080;
const int MAX_CLIENTS = 100;

int main()
{
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0)
	{
		perror("socket");
		return 1;
	}

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(listener, (sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return 1;
	}

	if (listen(listener, SOMAXCONN) < 0)
	{
		perror("listen");
		return 1;
	}

	std::cout << "Listening with poll() on port " << PORT << "...\n";

	std::vector<pollfd> fds;
	fds.push_back({listener, POLLIN, 0});

	while (true)
	{
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity < 0)
		{
			perror("poll");
			break;
		}

		for (size_t i = 0; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == listener)
				{
					sockaddr_in client_addr;
					socklen_t len = sizeof(client_addr);
					int client_fd = accept(listener, (sockaddr *)&client_addr, &len);
					if (client_fd > 0)
					{
						std::cout << "New client: FD = " << client_fd << "\n";
						fds.push_back({client_fd, POLLIN, 0});
					}
				}
				else
				{
					char buffer[1024];
					ssize_t bytes = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (bytes <= 0)
					{
						std::cout << "Client FD " << fds[i].fd << " disconnected.\n";
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						i--;
					}
					else
					{
						send(fds[i].fd, buffer, bytes, 0);
					}
				}
			}
		}
	}

	for (auto &p : fds)
		close(p.fd);
	return 0;
}
