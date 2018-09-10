#include "endpoint.h"

endpoint::endpoint(int fd)
{
    m_sockfd = fd;

    m_max_buff = 2048;
    m_rbuff = new uint8_t [m_max_buff];

    on_connect(m_sockfd);
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

void endpoint::run()
{
    // create thread to process client data
    pthread_create(&m_tid, NULL, thread_poll, this);
}

void endpoint::quit()
{
    pthread_cancel(this->m_sockfd);
}

void endpoint::wait()
{
    pthread_join(this->m_tid, NULL);

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
            pthread_exit((void *)0);
        }

        ptr->on_recv_data(ptr->m_rbuff, recv_n);
    }
}
