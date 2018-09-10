#ifndef TCP_H
#define TCP_H

#include <iostream>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "endpoint.h"


class tcp
{
public:
    tcp();
    ~tcp();

    /*
     * connect to remote ip addr, use for tcp client
     */
    endpoint * Connect(const char * ipstr, uint16_t port);

    /*
     * bind local port, use for tcp server
     */
    int32_t Bind(uint16_t port);

    /*
     * run as server, return endpoint
     */
    endpoint * Accept();

private:
    void saddr_init(struct sockaddr_in &saddr, const char * ipstr, uint16_t port);

private:
    int m_sockfd;
};

#endif // TCP_H
