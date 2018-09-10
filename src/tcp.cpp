#include "tcp.h"

using namespace std;

tcp::tcp()
{
    m_sockfd = -1;
}

tcp::~tcp()
{
    if(-1 == m_sockfd) {
        close(m_sockfd);
        m_sockfd = -1;
    }
}

int32_t tcp::Bind(uint16_t port)
{
    int32_t ret;
    struct sockaddr_in saddr;

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_sockfd) {
        return -1;
    }
    saddr_init(saddr, "0.0.0.0", port);

    ret = bind(m_sockfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if(-1 == ret) {
        return -1;
    }

    return 0;
}

endpoint * tcp::Connect(const char *ipstr, uint16_t port)
{
    int32_t ret;
    struct sockaddr_in saddr;

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_sockfd) {
        return NULL;
    }
    saddr_init(saddr, ipstr, port);

    ret = connect(m_sockfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if(-1 == ret) {
        return NULL;
    }

    return new endpoint(m_sockfd);
}

endpoint * tcp::Accept()
{
    int fd;

    listen(m_sockfd, 5);
    while(1) {
        fd = accept(m_sockfd, NULL, NULL);
        if(-1 == fd) {
            cerr << "accept error, wait 10s" << endl;
            sleep(10);
            continue ;
        }
        return new endpoint(fd);
    }
}



// private function
void tcp::saddr_init(struct sockaddr_in &saddr, const char *ipstr, uint16_t port)
{
    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipstr, &saddr.sin_addr.s_addr);
    saddr.sin_port = htons(port);
}





















