#include "reactor.h"

reactor::reactor(int fd_epoll, int fd_listen, int max_fds, event_handler &ev_handler)
{
    fd_epoll_ = fd_epoll;
    fd_listen_ = fd_listen;
    max_fds_ = max_fds;
    ev_handler_ = &ev_handler;

    max_buff_ = 2048;

    events_ = new struct epoll_event [max_fds_];
    buff_ = new uint8_t [max_buff_];
}

reactor::~reactor()
{
    if(events_) {
        delete [] events_;
        events_ = NULL;
    }

    if(buff_) {
        delete [] buff_;
        buff_ = NULL;
    }

    if(-1 != fd_epoll_) {
        close(fd_epoll_);
        fd_epoll_ = -1;
    }

    if(-1 == fd_listen_) {
        close(fd_listen_);
        fd_listen_ = -1;
    }
}

reactor * reactor::create(int fd_listen, int max_fds, event_handler &ev_handler)
{
    int fd_epoll;

    fd_epoll = epoll_create(max_fds);
    if(-1 == fd_epoll) {
        perror("epoll_create");
        return NULL;
    } else {
        return new reactor(fd_epoll, fd_listen, max_fds, ev_handler);
    }
}


void reactor::run()
{
    struct epoll_event ev;
    int ret, nfds, i, con_fd, fd_client;
    ssize_t rn;
    struct ev_data {
        int fd;
        void * ptr;
    };

    // add listen socket
    bzero(&ev, sizeof(struct epoll_event));
    ev.events = EPOLLIN;
    ev.data.ptr = new struct ev_data;
    ((struct ev_data *)(ev.data.ptr))->fd = fd_listen_;
    ((struct ev_data *)(ev.data.ptr))->ptr = NULL;
    ret = epoll_ctl(fd_epoll_, EPOLL_CTL_ADD, fd_listen_, &ev);
    if(-1 == ret) {
        perror("epoll_ctl");
        return ;
    }

    // loop
    while(1) {
        nfds = epoll_wait(fd_epoll_, events_, max_fds_, -1);
        if(-1 == nfds) {
            perror("epoll_wait");
            return ;
        }

        for(i = 0; i < nfds; i++) {
            if(((struct ev_data *)(events_[i].data.ptr))->fd == fd_listen_) {
                con_fd = accept(fd_listen_, NULL, NULL);
                if(-1 == con_fd) {
                    perror("accept");
                    return ;
                }

                setnonblocking(con_fd);
                bzero(&ev, sizeof(epoll_event));
                ev.events = EPOLLIN | EPOLLET;
                ev.data.ptr = new struct ev_data;
                ((struct ev_data *)(ev.data.ptr))->fd = con_fd;
                ((struct ev_data *)(ev.data.ptr))->ptr = NULL;
                ev_handler_->on_connect(con_fd, &(((struct ev_data *)(ev.data.ptr))->ptr));

                ret = epoll_ctl(fd_epoll_, EPOLL_CTL_ADD, con_fd, &ev);
                if(-1 == ret) {
                    perror("epoll_ctl");
                    return ;
                }
            } else {
                // dispach event and call
                fd_client = ((struct ev_data *)(events_[i].data.ptr))->fd;

                if(events_[i].events & EPOLLIN) {
                    rn = read(fd_client, buff_, max_buff_);
                    if(rn < 0) {
                        ev_handler_->on_read_err(fd_client, ((struct ev_data *)(events_[i].data.ptr))->ptr, errno);
                        continue ;
                    } else if(rn == 0) {
                        ev_handler_->on_close(fd_client, ((struct ev_data *)(events_[i].data.ptr))->ptr);

                        epoll_ctl(fd_epoll_, EPOLL_CTL_DEL, fd_client, &events_[i]);
                        close(fd_client);
                    } else {
                        ev_handler_->on_read(fd_client, ((struct ev_data *)(events_[i].data.ptr))->ptr, buff_, rn);
                    }
                } else if(events_[i].events & EPOLLOUT) {

                }
            }
        }
    }
}

int reactor::setnonblocking(int fd)
{
    int flags;
    if (-1 == (flags = fcntl(fd, F_GETFL, 0))) {
        flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}










