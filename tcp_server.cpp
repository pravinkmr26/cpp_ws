
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <strings.h>
#include <unistd.h>

int main()
{
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    auto server_sock_fd = socket(AF_INET, SOCK_STREAM,  0);
    bzero(&server_addr, sizeof(struct sockaddr_in));
    in_addr_t addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8081);

    bind(server_sock_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));

    listen(server_sock_fd, 10);

    auto client_addr_len = sizeof(struct sockaddr_in);
    std::cout << "waiting for connection" << std::endl;
    auto connection_fd = accept(server_sock_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_len);
    if (connection_fd < 0)
    {
        perror("accept");
        return connection_fd;
    }
    std::cout << "connection accepted" << std::endl;
    char buffer[6] = "";
    auto ret = read(connection_fd, buffer, 6);
    if (ret < 0)
    {
        perror("recv");
        return ret;
    }
    std::cout << "Received message: " << buffer << std::endl;
    close(connection_fd);
    close(server_sock_fd);
}
