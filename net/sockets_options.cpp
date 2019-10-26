//
// Created by root on 19-10-26.
//

#include "sockets_options.h"
#include <fcntl.h>
namespace sockets
{

    int createNonblockingSocket(sa_family_t family)
    {
        int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
        if(sockfd < 0)
            return -1;
        return setNonblockingAndCloseOnExec(sockfd);
    }

    int setNonblockingAndCloseOnExec(int fd)
    {
        int flags = ::fcntl(fd, F_GETFL, 0);
        flags = flags | O_NONBLOCK;
        ::fcntl(fd, F_SETFL, flags);

        flags = ::fcntl(fd, F_GETFD, 0);
        flags |= O_CLOEXEC;
        ::fcntl(fd, F_SETFD, flags);
        return 0;
    }

    const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr)
    {
        return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
    }

    struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr)
    {
        return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
    }

    const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr)
    {
        return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
    }

    const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr)
    {
        return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
    }

    const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr)
    {
        return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
    }

    void bind(int sockfd, const struct sockaddr* addr)
    {
        int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
        if(ret < 0)
            return ;
    }

    void listen(int sockfd)
    {
        int ret = ::listen(sockfd, 10);
        if(ret)
            return ;
    }

    int accept(int sockfd, struct sockaddr_in6 *addr)
    {
        socklen_t addrLen = static_cast<socklen_t>(sizeof(*addr));
        int connfd = :: accept(sockfd, sockaddr_cast(addr), &addrLen);
        if(connfd < 0)
        {
            switch(errno)
            {
             case EAGAIN:
             case ECONNABORTED:
             case EINTR:
             case EPROTO:
             case EPERM:
             case EMFILE:
                 //do some thing
                 return -errno;
             case EBADF:
             case EFAULT:
             case EINVAL:
             case ENFILE:
             case ENOBUFS:
             case ENOMEM:
             case ENOTSOCK:
             case EOPNOTSUPP:
                 //do some thing
                   return -errno;
             default:
                 return -errno;
            }
        }
        return connfd;
    }

    int connect(int sockfd, const struct sockaddr* addr)
    {
        return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    }

    ssize_t read(int sockfd, void *buf, size_t count)
    {
        return ::read(sockfd, buf, count);
    }

    ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt)
    {
        return ::readv(sockfd, iov, iovcnt);
    }

    ssize_t write(int sockfd, const void *buf, size_t count)
    {
        return ::write(sockfd, buf, count);
    }

    void close(int sockfd)
    {
        if (::close(sockfd) < 0)
        {
            return;
        }
    }

    void shutdownWrite(int sockfd)
    {
        if (::shutdown(sockfd, SHUT_WR) < 0)
        {
           // LOG_SYSERR << "sockets::shutdownWrite";
        }
    }

    void toIpPort(char* buf, size_t size,
                           const struct sockaddr* addr)
    {
        toIp(buf,size, addr);
        size_t end = ::strlen(buf);
        const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
        uint16_t port = sockets::networkToHost16(addr4->sin_port);
        assert(size > end);
        snprintf(buf+end, size-end, ":%u", port);
    }

    void toIp(char* buf, size_t size,
                       const struct sockaddr* addr)
    {
        if (addr->sa_family == AF_INET)
        {
            assert(size >= INET_ADDRSTRLEN);
            const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
            ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
        }
        else if (addr->sa_family == AF_INET6)
        {
            assert(size >= INET6_ADDRSTRLEN);
            const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
            ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
        }
    }

    void fromIpPort(const char* ip, uint16_t port,
                             struct sockaddr_in* addr)
    {
        addr->sin_family = AF_INET;
        addr->sin_port = hostToNetwork16(port);
        if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
        {
        }
    }

    void fromIpPort(const char* ip, uint16_t port,
                             struct sockaddr_in6* addr)
    {
        addr->sin6_family = AF_INET6;
        addr->sin6_port = hostToNetwork16(port);
        if (::inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0)
        {
        }
    }

    int getSocketError(int sockfd)
    {
        int optval;
        socklen_t optlen = static_cast<socklen_t>(sizeof optval);

        if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
        {
            return errno;
        }
        else
        {
            return optval;
        }
    }

    struct sockaddr_in6 getLocalAddr(int sockfd)
    {
        struct sockaddr_in6 localaddr;
        bzero(&localaddr, sizeof localaddr);
        socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
        if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0)
        {

        }
        return localaddr;
    }

    struct sockaddr_in6 getPeerAddr(int sockfd)
    {
        struct sockaddr_in6 peeraddr;
        bzero(&peeraddr, sizeof peeraddr);
        socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
        if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0)
        {

        }
        return peeraddr;
    }

}