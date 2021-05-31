#include <arpa/inet.h> // inet_addr
#include <gflags/gflags.h>
#include <netinet/in.h> // sockaddr_in{} and other Internet defns
#include <stdio.h>
#include <string>
#include <strings.h>
#include <sys/socket.h> // basic socket definitions
#include <unistd.h>

DEFINE_string(ip_addr, "localhost", "ip_addr default 'localhost'");
DEFINE_int32(port, 12345, "port default '12345'");

size_t readn(int fd, void *buffer, size_t size)
{
    char *buffer_pointer = (char *)buffer;
    int length = size;

    while (length > 0)
    {
        int result = read(fd, buffer_pointer, length);

        if (result < 0)
        {
            if (errno == EINTR)
                continue; // 考虑非阻塞的情况，这里需要再次调用read
            else
                return (-1);
        }
        else if (result == 0)
            break; // EOF(End of File)表示套接字关闭

        length -= result;
        buffer_pointer += result;
    }
    return (size - length); // 返回的是实际读取的字节数
}

void read_data(int sockfd)
{
    ssize_t n;
    char buf[1024];

    int time = 0;
    for (;;)
    {
        fprintf(stdout, "block in read\n");
        if ((n = readn(sockfd, buf, 1024)) == 0)
            return;

        time++;
        fprintf(stdout, "1K read for %d \n", time);
        usleep(1000);
    }
}

int main(int argc, char **argv)
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    int listenfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(FLAGS_ip_addr.c_str());
    servaddr.sin_port = htons(FLAGS_port);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    // listen的backlog为1024
    listen(listenfd, 1024);

    // 循环处理用户请求
    for (;;)
    {
        clilen = sizeof(cliaddr);
        int connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        read_data(connfd); // 读取数据
        close(connfd);     // 关闭连接套接字，注意不是监听套接字
    }

    return 0;
}