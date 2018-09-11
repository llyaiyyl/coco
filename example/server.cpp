#include <iostream>
#include <string.h>

#include "endpoint.h"
#include "tcp.h"

using namespace std;

class ser_edp : public endpoint
{
public:
    ser_edp(int fd) : endpoint(fd) {}
    ~ser_edp()
    {
        cout << "ser_edp distroy" << endl;
    }

protected:
    void on_recv_data(const void * data, size_t len)
    {
        memcpy(rbuff, data, len);
        rbuff[len] = 0;
        cout << "recv: " << rbuff << endl;

        this->send_data("coco", 4);
    }

    void on_recv_error(void)
    {
        cout << "rev error" << endl;
    }

    void on_connect(int32_t fd)
    {
        cout << "client " << this->get_remote_ip() << ":"
             << this->get_remote_port() << endl;
    }

    void on_close(void)
    {
        cout << "client has close" << endl;
    }

private:
    char rbuff[1024];
};


int main(int argc, char * argv[])
{
    int32_t sockfd;
    tcp s;
    ser_edp * sedp;

    s.Bind(4000);
    while(1) {
        sockfd = s.Accept();
        if(-1 != sockfd) {
            sedp = new ser_edp(sockfd);
            sedp->run();
        }
    }

    return 0;
}
