#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void send_file(int out_fd, int in_fd, off_t offset, size_t count) {
    off_t sent_bytes = 0;
    while (sent_bytes < count) {
        // sendfile 系统调用是实现零拷贝的一种方法。
        // 它允许在两个文件描述符之间直接传输数据，而不需要在内核和用户空间之间复制数据。
        // 具体来说，sendfile 系统调用将文件描述符之间的数据传输委托给内核，
        // 内核会直接在内核空间中将数据从一个文件描述符复制到另一个文件描述符，
        // 从而避免了在用户空间和内核空间之间的额外数据复制。
        // 这种方式称为零拷贝，因为在数据传输过程中没有发生数据的实际拷贝。
        // 总的来说，sendfile 系统调用通过在内核空间中直接传输数据，避免了在用户空间和内核空间之间的数据复制，
        // 从而提高了数据传输的效率。
        ssize_t ret = sendfile(out_fd, in_fd, &offset, count - sent_bytes);
        if (ret <= 0) {
            // Handle error or end of file
            break;
        }
        sent_bytes += ret;
    }
}

int test2() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 获取文件大小
    struct stat stat_buf;
    fstat(fd, &stat_buf);

    // 将文件映射到内存
    void* addr = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    // 连接服务器
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 1;
    }

    // 发送文件内容
    ssize_t sent_bytes = send(sock, addr, stat_buf.st_size, 0);
    if (sent_bytes == -1) {
        perror("send");
        return 1;
    }

    std::cout << "Sent " << sent_bytes << " bytes" << std::endl;

    // 关闭套接字
    close(sock);

    // 取消内存映射
    munmap(addr, stat_buf.st_size);

    // 关闭文件描述符
    close(fd);

    return 0;
}
void test1() {
    std::cout << "test1==============================" << std::endl;
    int input_fd = open("test_zero_copy_input.txt", O_RDONLY);
    // 可以是套接字描述符
    int output_fd = open("test_zero_copy_output.txt", O_WRONLY | O_CREAT, 0644);

    // 获取文件大小
    struct stat stat_buf;
    fstat(input_fd, &stat_buf);
    off_t file_size = stat_buf.st_size;
    std::cout << "file_size: " << file_size << std::endl;

    // 使用 sendfile 进行零拷贝传输
    send_file(output_fd, input_fd, 0, file_size);

    // 关闭文件描述符
    close(input_fd);
    close(output_fd);
}
int main() {
    test1(); // send_file 方式
    test2();// mmap方式
   

    return 0;
}
