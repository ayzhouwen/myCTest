#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <time.h>
#include <sys/time.h>
#define SERVER_PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024


// 该文件的代码是由chatgpt生成，稍微进行了些修改

// 返回当前的年月日时分秒毫秒字符串
char* get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm* timeinfo;
    timeinfo = localtime(&tv.tv_sec);

    char* time_string = (char*)malloc(sizeof(char) * 24);
    sprintf(time_string, "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
            tv.tv_usec / 1000);

    return time_string;
}


int main() {
    int optval = 1;
    int server_sockfd, epoll_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    struct epoll_event events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];

    // 创建服务器 socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
        // 设置 SO_REUSEADDR 选项
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }


    // 设置服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // 绑定地址和端口
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听连接请求
    if (listen(server_sockfd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", SERVER_PORT);

    // 创建 epoll 实例
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("Epoll creation failed");
        exit(EXIT_FAILURE);
    }

    // 添加服务器 socket 到 epoll 实例中
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_sockfd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_sockfd, &event) < 0) {
        perror("Epoll control failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // 等待事件发生
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events < 0) {
            perror("Epoll wait failed");
            exit(EXIT_FAILURE);
        }

        // 处理事件
        int i;
        for ( i = 0; i < num_events; i++) {
            if (events[i].data.fd == server_sockfd) {
                // 处理新的连接请求
                client_addr_len = sizeof(client_addr);
                int client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_sockfd < 0) {
                    perror("Accept failed");
                    exit(EXIT_FAILURE);
                }

                // 将客户端 socket 添加到 epoll 实例中
                event.events = EPOLLIN;
                event.data.fd = client_sockfd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sockfd, &event) < 0) {
                    perror("Epoll control failed");
                    exit(EXIT_FAILURE);
                }

                printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            } else {
                // 处理已连接的客户端请求
                int client_sockfd = events[i].data.fd;

                // 接收客户端数据
                memset(buffer, 0, sizeof(buffer));
                int bytes_received = recv(client_sockfd, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    // 客户端断开连接或出错，关闭客户端 socket 并从 epoll 实例中移除
                    close(client_sockfd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sockfd, NULL);
                } else {

                    char* current_time = get_current_time();
                    // 处理客户端请求并发送响应
                    printf("time:%s Received from client: %s\n", current_time,buffer);
                     free(current_time);

                    char response[] = "Hello, Client!";
                    send(client_sockfd, response, strlen(response), 0);
                }
            }
        }
    }

    // 关闭服务器 socket 和 epoll 实例
    close(server_sockfd);
    close(epoll_fd);

    return 0;
}