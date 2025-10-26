//
// Created by pravinkumar on 10/11/25.
//


#include <cstdio>
#include <iostream>
#include <ostream>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    auto socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("socket");
        return socket_fd;
    }
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8081);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }


    std::cout << "connecting to server" << std::endl;
    auto ret = connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
    if (ret < 0)
    {
        std::cout << "connection failed" << std::endl;
        perror("connect");
        return ret;
    }
    std::cout << "connected to server, sending message" << std::endl;
    char message[6] = "hello";
    ret = write(socket_fd, message, sizeof(message));
    if (ret < 0)
    {
        perror("send");
        return ret;
    }
    std::cout << "message sent" << std::endl;
    close(socket_fd);
}
