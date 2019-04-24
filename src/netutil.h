#ifndef NETUTIL_H
#define NETUTIL_H

#include <map>

#define typeIPv4 0
#define typeIPv6 1

// 使用getaddrinfo识别IPv4和IPv6
class BaseSocket
{
  private:
    static std::map<int, BaseSocket *> smartSocketmap;

  protected:
    BaseSocket(/* args */);
    ~BaseSocket();
    void DeleteSmartSocket();

  public:
    static BaseSocket *
    CreateSmartSocket(char *ipaddr);
    void DeleteSmartSocket();
};
std::map<int, BaseSocket *> BaseSocket::smartSocketmap;

class SocketV4 : public BaseSocket
{
  public:
    SocketV4(const char[]) {}
};

class SocketV6 : public BaseSocket
{
  public:
    SocketV6(const char[]) {}
};

#endif