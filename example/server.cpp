#include <iostream>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "tcp.h"
#include "reactor.h"
#include "event_handler.h"

class handler : public event_handler
{
public:
    handler() {}

    void on_connect(int fd, void ** pdata)
    {
        struct sockaddr_in saddr;
        socklen_t slen;
        char ipstr[INET_ADDRSTRLEN];
        uint16_t port;
        char buff[30];

        slen = sizeof(struct sockaddr_in);
        bzero(&saddr, slen);
        if(0 == getpeername(fd, (struct sockaddr *)&saddr, &slen)) {
            port = ntohs(saddr.sin_port);
            inet_ntop(saddr.sin_family, &(saddr.sin_addr.s_addr), ipstr, INET_ADDRSTRLEN);
        } else {
            ipstr[0] = 0;
            port = 0;
        }

        sprintf(buff, "%s:%d", ipstr, port);
        *pdata = new std::string(buff);

        std::cout << "client connect: " << *(std::string *)(*pdata) << std::endl;
    }
    void on_read_err(int fd, void * pdata, int err)
    {
        errno = err;
        perror("on_read_err");
    }
    void on_read(int fd, void * pdata, const void * rbuff, size_t rn)
    {
        std::string str((char *)rbuff, rn);
        std::cout << *(std::string *)pdata << " " << str.c_str() << std::endl;

        write(fd, str.c_str(), str.size());
    }
    void on_close(int fd, void * pdata)
    {
        std::cout << "on_close: " << *(std::string *)pdata << std::endl;
    }
};


int main(int argc, char * argv[])
{
    int32_t sockfd;
    handler hd;
    reactor * r;

    sockfd = tcp::Listen(4000);
    r = reactor::create(sockfd, 128, hd);
    r->run();

    return 0;
}
