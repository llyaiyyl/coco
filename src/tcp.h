#ifndef TCP_H
#define TCP_H

#include <iostream>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class tcp
{
public:
    tcp();
    ~tcp();

    /*
     * connect to remote ip addr, use for tcp client
     * return: socket fd
     */
    int32_t Connect(const char * ipstr, uint16_t port);

    /*
     * bind local port, use for tcp server
     * return 0 = ok, -1 = error
     */
    int32_t Bind(uint16_t port);

    /*
     * run as server
     * return: socket fd
     */
    int32_t Accept();

private:
    void saddr_init(struct sockaddr_in &saddr, const char * ipstr, uint16_t port);

private:
    int m_sockfd;
};

#endif // TCP_H
