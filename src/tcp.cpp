#include "tcp.h"

int32_t tcp::Connect(const char *ipstr, uint16_t port)
{
    int32_t fd, ret;
    struct sockaddr_in saddr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd) {
        perror("socket");
        return -1;
    }
    saddr_init(saddr, ipstr, port);

    ret = connect(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if(-1 == ret) {
        perror("connect");
        return -1;
    }

    return fd;
}

ssize_t tcp::Read(int32_t fd, void *buff, size_t n)
{
    return read(fd, buff, n);
}

ssize_t tcp::Write(int32_t fd, const void *buff, size_t n)
{
    return write(fd, buff, n);
}

std::string tcp::Read(int32_t fd, size_t n)
{
    char * buff  = new char [n];
    ssize_t rn;
    std::string str;

    rn = read(fd, buff, n);
    if(rn < 0) {
        perror("read");
    } else if(0 == rn) {
        perror("server close");
    } else {
        str = std::string(buff, rn);
    }

    delete [] buff;
    return str;
}

ssize_t tcp::Write(int32_t fd, const std::string &str)
{
    return write(fd, str.c_str(), str.length());
}

int32_t tcp::Listen(uint16_t port)
{
    int32_t fd, ret;
    struct sockaddr_in saddr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd) {
        perror("socket");
        return -1;
    }
    saddr_init(saddr, "0.0.0.0", port);

    ret = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if(-1 == ret) {
        perror("bind");
        return -1;
    }

    ret = listen(fd, 5);
    if(-1 == ret) {
        perror("listen");
        return -1;
    }

    return fd;
}

// private function
void tcp::saddr_init(struct sockaddr_in &saddr, const char *ipstr, uint16_t port)
{
    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipstr, &saddr.sin_addr.s_addr);
    saddr.sin_port = htons(port);
}





















