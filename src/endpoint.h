#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

class endpoint
{
public:
    endpoint(int fd);
    ~endpoint();

    /*
     * send and recieve data in block mode
     */
    ssize_t send_data(const void * data, size_t len);
    ssize_t recv_data(void * data, size_t len);

    void run();
    void quit();
    void wait();
protected:
    virtual void on_recv_data(const void * data, size_t len) {}
    virtual void on_recv_error(void) {}

    virtual void on_connect(int32_t fd) {}
    virtual void on_close(void) {}

private:
    static void * thread_poll(void * pdata);


private:
    int         m_sockfd;
    size_t      m_max_buff;
    uint8_t   * m_rbuff;
    pthread_t   m_tid;
};

#endif // ENDPOINT_H
