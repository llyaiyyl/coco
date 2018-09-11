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

    const char * get_local_ip(void);
    const char * get_remote_ip(void);
    uint16_t get_local_port(void);
    uint16_t get_remote_port(void);

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

    char m_ipl[INET_ADDRSTRLEN], m_ipr[INET_ADDRSTRLEN];
    uint16_t m_portl, m_portr;
};

#endif // ENDPOINT_H
