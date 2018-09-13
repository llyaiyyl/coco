#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <stdio.h>

class event_handler
{
public:
    event_handler();

    virtual void on_connect(int &fd, void ** pdata) = 0;
    virtual void on_read_err(int &fd, void * pdata, int err) = 0;
    virtual void on_read(int &fd, void * pdata, const void * rbuff, size_t rn) = 0;
    virtual void on_close(int &fd, void * pdata) = 0;
};

#endif // EVENT_HANDLER_H
