#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <sys/poll.h>

#include "resp_parser.hpp"

#define CONNECTION_BACKLOG 5
#define MAX_CONNECTIONS 20

int main(int argc, char **argv)
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    int server_fd = -1, client_fd = -1;
    struct sockaddr_in server_addr{};
    struct pollfd fds[MAX_CONNECTIONS + 1]{};
    int reuse = 1, nfds = 1, rc;
    int timeout = 3 * 60 * 1000;
    char buffer[1024];
    bool squeeze_array = false;
    int bytes_read, i, j;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(6379);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port 6379\n";
        return 1;
    }

    if (listen(server_fd, CONNECTION_BACKLOG) != 0)
    {
        std::cerr << "listen failed\n";
        return 1;
    }

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here!\n";

    // Using pollfd structure for storing listening and client sockets
    for (i = 0; i < MAX_CONNECTIONS + 1; i++)
        fds[i].fd = -1;

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    // Event loop
    while (true)
    {

        rc = poll(fds, nfds, timeout);

        if (rc < 0)
        {
            std::cerr << "poll() failed.";
            return 1;
        }

        if (rc == 0)
        {
            std::cout << "poll() timed out. End program.\n";
            return 0;
        }

        if (fds[0].revents == POLLIN)
        {
            client_fd = accept(server_fd, nullptr, nullptr);

            if (client_fd < 0)
            {
                std::cerr << "Failed to accept connection.";
                return 1;
            }

            std::cout << "Client connected: FD = " << client_fd << std::endl;

            fds[nfds].fd = client_fd;
            fds[nfds].events = POLLIN;
            nfds++;
        }

        for (int i = 1; i < nfds; i++)
        {
            if (fds[i].fd < 0 || fds[i].revents == 0)
                continue;
            if (fds[i].revents == POLLIN)
            {
                std::cout << "Reading from socket with FD = " << fds[i].fd << std::endl;
                bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0);

                if (bytes_read < 0)
                {
                    std::cerr << "Failed to read message.";
                    return 1;
                }

                if (bytes_read == 0)
                {
                    // Close the connection
                    std::cout << "Closing client socket with FD = " << fds[i].fd << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    squeeze_array = true;
                }
                else
                {
                    std::string response = RespUtil::parse(buffer, bytes_read);
                    send(fds[i].fd, response.c_str(), response.size(), 0);
                }
            }
        }

        if (squeeze_array)
        {
            for (i = 1; i < nfds; i++)
            {
                squeeze_array = false;
                if (fds[i].fd == -1)
                {
                    for (j = i; j < nfds; j++)
                    {
                        fds[j].fd = fds[j + 1].fd;
                    }
                    i--;
                    nfds--;
                }
            }
        }
    }

    close(server_fd);
    return 0;
}