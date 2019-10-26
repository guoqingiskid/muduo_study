//
// Created by root on 19-10-26.
//

#ifndef REPOS_MY_MUDUO_SOCKETS_OPTIONS_H
#define REPOS_MY_MUDUO_SOCKETS_OPTIONS_H

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <endian.h>
#include <assert.h>
#include <stdio.h>

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
    return f;
}

namespace sockets
{
    int createNonblockingSocket(sa_family_t family);
    int setNonblockingAndCloseOnExec(int fd);
    int connect(int sockfd, const struct sockaddr* addr);
    void bind(int sockfd, const struct sockaddr* addr);
    void listen(int sockfd);
    int accept(int sokcfd, struct sockaddr_in6 *addr);
    ssize_t read(int sockfd, void *buf, size_t count);
    ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
    ssize_t write(int sockfd, const void *buf, size_t count);
    void close(int sockfd);
    void shutdownWrite(int sockfd);

    void toIpPort(char* buf, size_t size,
                  const struct sockaddr* addr);
    void toIp(char* buf, size_t size,
              const struct sockaddr* addr);

    void fromIpPort(const char* ip, uint16_t port,
                    struct sockaddr_in* addr);
    void fromIpPort(const char* ip, uint16_t port,
                    struct sockaddr_in6* addr);

    int getSocketError(int sockfd);

    const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
    const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
    struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
    const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
    const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);

    struct sockaddr_in6 getLocalAddr(int sockfd);
    struct sockaddr_in6 getPeerAddr(int sockfd);
    bool isSelfConnect(int sockfd);

    inline uint64_t hostToNetwork64(uint64_t host64)
    {
        return htobe64(host64);
    }

    inline uint32_t hostToNetwork32(uint32_t host32)
    {
        return htobe32(host32);
    }

    inline uint16_t hostToNetwork16(uint16_t host16)
    {
        return htobe16(host16);
    }

    inline uint64_t networkToHost64(uint64_t net64)
    {
        return be64toh(net64);
    }

    inline uint32_t networkToHost32(uint32_t net32)
    {
        return be32toh(net32);
    }

    inline uint16_t networkToHost16(uint16_t net16)
    {
        return be16toh(net16);
    }
}


#endif //REPOS_MY_MUDUO_SOCKETS_OPTIONS_H
