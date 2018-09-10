#include <iostream>

#include "tcp.h"

using namespace std;

class cli_edp : public endpoint
{
protected:
    void on_recv_data(const void * data, size_t len)
    {
        // data[len] = 0;
        // cout << "recv: " << data << endl;

        this->quit();
    }

    void on_recv_error(void)
    {
        cout << "rev error" << endl;
    }

    void on_connect(int32_t fd)
    {
        cout << "client connect" << endl;
        cout << "send data to server" << endl;
        this->send_data("hello", 5);
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

    edp = s.Connect("0.0.0.0", 4000);
    if(edp) {
        edp->run();
        edp->wait();
    }

    return 0;
}


