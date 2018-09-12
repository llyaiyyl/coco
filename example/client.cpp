#include <iostream>
#include <unistd.h>
#include "tcp.h"

int main(int argc, char * argv[])
{
    char rbuff[1024];
    ssize_t rn;
    int32_t sockfd, num;

    sockfd = tcp::Connect("0.0.0.0", 4000);
    if(-1 != sockfd) {
        num = 0;
        while(num <= 20) {
            tcp::Write(sockfd, "ping", 4);
            std::cout << "send msg to server" << std::endl;

            rn = tcp::Read(sockfd, rbuff, 1024);
            rbuff[rn] = 0;
            std::cout << rbuff << std::endl;

            sleep(3);
            num++;
        }

        close(sockfd);
    }

    return 0;
}


