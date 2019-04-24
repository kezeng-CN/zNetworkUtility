# zNetworkUtility

在学习网络编程时积累的工具代码

## 同时支持IPv6和IPv4

1. 怎么准确识别用户输入的地址或者域名是属于 IPv4 网络还是 IPv6 网络？
2. 怎么屏蔽网络连接细节，提供给用户一个统一的接口？

### IPv4 与 IPv6 对比

在IPv4网络下，网络编程依靠的是socket连接。在客户端基本步骤为创建一个socket，使用socket连接服务器，最后通过TCP或者UDP协议进行数据读写。如果把这套方法移植到IPv6网络下，就需要在原来的基础上引入新的协议族、新的数据结构以及新的地址域名转换函数等。

|				|IPv4					|IPv6					|
|---------------|:----------------------|:----------------------|
|协议簇			|AF_INET				|AF_INET6				|
|地址簇			|PF_INET				|PF_INET6				|
|数据结构		|in_addr sockaddr_in	|in6_addr sockaddr_in6	|
|网络地址转换函数	|inet_aton/inet_addr	|inet_pton				|
|				|inet_ntoa				|inet_ntop				|
|地址转换函数		|gethostbyname()		|getipnodebyname()		|
|				|gethostbyaddr()		|getipnodebyaddr()		|
|				|						|getnameinfo()			|
|				|						|getaddrinfo()			|

getaddrinfo函数在IPv6和IPv4网络下都能实现独立于协议的名称解析，它返回的指向addrinfo结构的链表中会存放所有由输入参数nodename解新出的所有对应的IP信息，包括IP地址，协议簇信息等。只要对const struct addrinfo *hints进行一些配置，就可以利用这个函数识别连接目标的网络协议属性。

    int getaddrinfo(
    const char* nodename,			//主机名或数字地址（IPv4点分10进制/IPv6的16进制）
    const char* servname,			//10进制端口号或ftp、http此类服务名
    const struct addrinfo* hints,	//空指针或addrinfo指针，调用者填写关于它所想返回的信息类型线索
    struct addrinfo **res			//存放返回addrinfo结构链表的指针
    );

getaddrinfo调用前通常需要设置6个参数

1. nodename
2. servname
3. hints.ai_flags
4. hints.ai_family
5. hints.ai_socktype
6. hints.ai_protocol

参数hints.ai_family、hints.ai_socktype和hints.ai_protocol说明

|参数			|取值			|值	|说明							|
|:--------------|:--------------|:--|:------------------------------|
|ai_family		|AF_INET		|2	|IPv4							|
|				|AF_INET6		|23	|IPv6							|
|				|AF_UNSPEC		|0	|协议无关						|
|ai_protocol	|IPPROTO_IP		|0	|IP协议							|
|				|IPPROTO_IPV4	|4	|IPv4							|
|				|IPPROTO_IPV6	|41	|IPv6							|
|				|IPPROTO_UDP	|17	|UDP							|
|				|IPPROTO_TCP	|6	|TCP							|
|ai_socktype	|SOCK_STREAM	|1	|流								|
|				|SOCK_DGRAM		|2	|数据报							|
|ai_flag		|AI_PASSIVE		|1	|被动，常用于server socket的bind	|
|				|AI_CANONNAME	|2	|								|
|				|AI_NUMBERICHOST|4	|地址为数字串						|

前三个参数nodename、servname和hints.ai_flags为重要参数


