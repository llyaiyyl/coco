#include <iostream>

#include "tcp.h"

using namespace std;

class ser_edp : public endpoint
{
protected:
    void on_recv_data(const void * data, size_t len)
    {
        // data[len] = 0;

        // cout << "recv: " << data << endl;
        this->send_data("coco", 4);
    }

    void on_recv_error(void)
    {
        cout << "rev error" << endl;
    }

    void on_connect(int32_t fd)
    {
        cout << "client connect" << endl;
    }

    void on_close(void)
    {
        cout << "client close" << endl;
    }
};


int main(int argc, char * argv[])
{
    tcp s;
    endpoint * edp;

    s.Bind(4000);
    while(1) {
        edp = s.Accept();
        if(edp) {
            edp->run();
        }
    }

    return 0;
}
