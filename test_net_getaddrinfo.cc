#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>
#include <string>

int main() {
    struct addrinfo hints, *result, *rp;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    // IPv4 或 IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          // Any protocol

    s = getaddrinfo("stanford.edu", NULL, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        // 处理每个地址信息结构体 rp
        // struct addrinfo {
        //     int              ai_flags;     // 用于控制地址信息的获取方式
        //     int              ai_family;    // 地址族，如 AF_INET、AF_INET6 等
        //     int              ai_socktype;  // 套接字类型，如 SOCK_STREAM、SOCK_DGRAM 等
        //     int              ai_protocol;  // 使用的协议，如 IPPROTO_TCP、IPPROTO_UDP 等
        //     socklen_t        ai_addrlen;   // 地址长度
        //     struct sockaddr *ai_addr;      // 指向 sockaddr 结构的指针，指向实际的地址信息
        //     char            *ai_canonname; // 规范化的主机名
        //     struct addrinfo *ai_next;      // 指向下一个 addrinfo 结构的指针，用于构成链表
        // };
        for (int i = 0; i < 14; i++) {
            printf("%02x ", (unsigned char)rp->ai_addr->sa_data[i]);
        }
        printf("rp->ai_addr: %s    rp->ai_addrlen: %d\n", std::string(rp->ai_addr->sa_data), rp->ai_addrlen);
    }

    freeaddrinfo(result);
    return 0;
}
