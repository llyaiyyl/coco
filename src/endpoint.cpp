#include <iostream>
#include <string.h>

#include "endpoint.h"

endpoint::endpoint(int fd)
{
    struct sockaddr_in saddr;
    socklen_t slen;

    m_sockfd = fd;

    m_max_buff = 2048;
    m_rbuff = new uint8_t [m_max_buff];

    m_tid = 0;


    // init remote and local ip addr
    m_ipl[0] = 0;
    m_ipr[0] = 0;
    m_portl = 0;
    m_portr = 0;

    slen = sizeof(struct sockaddr_in);
    bzero(&saddr, slen);
    if(0 == getsockname(m_sockfd, (struct sockaddr *)&saddr, &slen)) {
        m_portl = ntohs(saddr.sin_port);
        inet_ntop(saddr.sin_family, &(saddr.sin_addr.s_addr), m_ipl, INET_ADDRSTRLEN);
    }
    slen = sizeof(struct sockaddr_in);
    bzero(&saddr, slen);
    if(0 == getpeername(m_sockfd, (struct sockaddr *)&saddr, &slen)) {
        m_portr = ntohs(saddr.sin_port);
        inet_ntop(saddr.sin_family, &(saddr.sin_addr.s_addr), m_ipr, INET_ADDRSTRLEN);
    }
}

endpoint::~endpoint()
{
    if(m_rbuff) {
        delete [] m_rbuff;
        m_rbuff = NULL;
    }

    if(-1 != m_sockfd) {
        close(m_sockfd);
        m_sockfd = -1;
    }
}


ssize_t endpoint::send_data(const void * data, size_t len)
{
    return send(m_sockfd, data, len, 0);
}

ssize_t endpoint::recv_data(void * data, size_t len)
{
    return recv(m_sockfd, data, len, 0);
}

const char * endpoint::get_local_ip(void)
{
    return m_ipl;
}

const char * endpoint::get_remote_ip(void)
{
    return m_ipr;
}

uint16_t endpoint::get_local_port()
{
    return m_portl;
}

uint16_t endpoint::get_remote_port()
{
    return m_portr;
}


void endpoint::run()
{
    on_connect(m_sockfd);

    // create thread to process client data
    pthread_create(&m_tid, NULL, thread_poll, this);
}

void endpoint::quit()
{
    pthread_cancel(m_tid);
}

void endpoint::wait()
{
    pthread_join(m_tid, NULL);

    close(m_sockfd);
    m_sockfd = -1;
}


void * endpoint::thread_poll(void *pdata)
{
    endpoint * ptr = (endpoint *)pdata;
    ssize_t recv_n;

    while(1) {
        recv_n = recv(ptr->m_sockfd, ptr->m_rbuff, ptr->m_max_buff, 0);
        if(-1 == recv_n) {
            if(errno == EINTR) {
                continue ;
            } else {
                ptr->on_recv_error();
            }
        } else if(0 == recv_n) {
            ptr->on_close();
            delete ptr;

            pthread_exit((void *)0);
        }

        ptr->on_recv_data(ptr->m_rbuff, recv_n);
    }
}
