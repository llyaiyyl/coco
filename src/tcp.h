#ifndef TCP_H
#define TCP_H

#include <iostream>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

class tcp
{
public:
    tcp() {}

    /*
     * connect to remote ip addr, use for tcp client
     * return: socket fd
     */
    static int32_t Connect(const char * ipstr, uint16_t port);
    static ssize_t Read(int32_t fd, void * buff, size_t n);
    static ssize_t Write(int32_t fd, const void * buff, size_t n);

    /*
     * listen on port, use for server
     * return 0 = ok, -1 = error
     */
    static int32_t Listen(uint16_t port);

private:
    static void saddr_init(struct sockaddr_in &saddr, const char * ipstr, uint16_t port);
};

#endif // TCP_H
