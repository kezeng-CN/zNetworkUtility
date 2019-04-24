#include "netutil.h"

#include <arpa\inet.h>
#include <netdb.h>
#include <string.h>
#include <sys\socket.h>

BaseSocket::BaseSocket(/* args */)
{
}

BaseSocket::~BaseSocket()
{
}

BaseSocket *BaseSocket::CreateSmartSocket(char *ipaddr)
{
    // 接收getaddrindo返回addrinfo链表信息Answer指针；入参信息Hint；遍历返回链表临时指针
    struct addrinfo *paiAns, aiHint, *paiCurr;

    // 参数Hint初始化
    bzero(&aiHint, sizeof(bzero));
    aiHint.ai_family = AF_UNSPEC;
    aiHint.ai_socktype = SOCK_STREAM;

    // 调用getaddrinfo
    int ret = getaddrinfo(ipaddr, NULL, &aiHint, &paiAns);
    if (ret != 0)
    {
        return NULL;
    }

    DeleteSmartSocket();

    char strIP[128];
    struct sockaddr_in *psaIPv4;
    struct sockaddr_in6 *psaIPv6;

    // 遍历返回链表
    for (paiCurr = paiAns; paiCurr != NULL; paiCurr++)
    {
        switch (paiCurr->ai_family)
        {
        case AF_UNSPEC:
            /* code */
            break;

        case AF_INET:
            psaIPv4 = reinterpret_cast<struct sockaddr_in *>(paiCurr->ai_addr);
            inet_ntop(AF_INET, &psaIPv4->sin_addr.s_addr, strIP, sizeof(strIP));
            smartSocketmap[typeIPv4] = new SocketV4(strIP);
            break;

        case AF_INET6:
            psaIPv6 = reinterpret_cast<struct sockaddr_in6 *>(paiCurr->ai_addr);
            inet_ntop(AF_INET6, &psaIPv6->sin6_addr, strIP, sizeof(strIP));
            smartSocketmap[typeIPv6] = new SocketV6(strIP);
            break;

        default:
            /* code */
            break;
        }
    }

    freeaddrinfo(paiAns);
    if (!smartSocketmap.empty())
    {
        smartSocketmap[0] = new BaseSocket();
    }
    return smartSocketmap[0];
}
