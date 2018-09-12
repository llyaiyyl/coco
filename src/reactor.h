#ifndef REACTOR_H
#define REACTOR_H

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "event_handler.h"

class reactor
{
public:
    static reactor * create(int fd_listen, int max_fds, event_handler &ev_handler);
    ~reactor();


    void run(void);

private:
    reactor(int fd_epoll, int fd_listen, int max_fds, event_handler &ev_handler);
    int setnonblocking(int fd);

private:
    struct epoll_event * events_;
    event_handler * ev_handler_;
    uint8_t * buff_;
    int fd_listen_, fd_epoll_;
    int max_fds_, max_buff_;
};

#endif // REACTOR_H
