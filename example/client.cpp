#include <iostream>

#include "endpoint.h"
#include "tcp.h"

using namespace std;

int main(int argc, char * argv[])
{
    char recv_buff[1024];
    ssize_t recv_n;

    int32_t sockfd;
    tcp s;
    endpoint * edp;

    sockfd = s.Connect("0.0.0.0", 4000);
    if(-1 != sockfd) {
        edp = new endpoint(sockfd);

        edp->send_data("ping", 4);
        cout << "send msg to server" << endl;

        recv_n = edp->recv_data(recv_buff, 1024);
        recv_buff[recv_n] = 0;
        cout << recv_buff << endl;
    }

    return 0;
}


