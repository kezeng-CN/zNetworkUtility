# 前言

1. 网络
2. 操作系统
3. 数据库
4. C++
5. Linux 命令


## 网络

OSI 七层网络模型

|      OSI 网络七层模型      | TCP/IP 模型 |
| :------------------------: | :---------: |
| 应用层<br>表示层<br>会话层 |   应用层    |
|           传输层           |   传输层    |
|           网络层           |   网络层    |
|      链路层<br>物理层      |   接口层    |

### 应用层

1. http 状态码

### 传输层

1. TCP 和 UDP 的差异

|              TCP               |                UDP                |
| :----------------------------: | :-------------------------------: |
|             字节流             |              数据报               |
| 每个包都有 TCP 部首（20 字节） | 仅有第一个包有 UDP 部首（8 字节） |
|  无差错、有序、不丢失、不重复  |              不可靠               |
|           有连接状态           |            无连接状态             |
|         有拥塞控制算法         |          无拥塞控制算法           |

### net-tools和iproute2

* net-tools
  * ifconfig
    1. 网卡命名
       * eth0 第一块网卡
       * CentOS一致性网络设备命名
         * eno1 板载网卡
         * ens33 PCI-E网卡
         * enp0s3 无法获取物理信息的PCI-E网卡
    2. 网络接口命名修改
       * biosdevname和net.ifnames两个参数
         * biosdevname=0 net.ifnames=1 则 网卡名为ens33
         * biosdevname=1 net.ifnames=0 则 网卡名为em1
         * biosdevname=0 net.ifnames=0 则 网卡名为eth0
       * 编辑/etc/default/grub
         * 增加biosdevname=0 net.ifnnet.ifnames=0
       * 更新grub
         * $sudo grub2-mkconfig -o /boot/grub2/grub.cfg
       * 重启
         * $sudo reboot
    3. 配置IP
       * ipconfig eth0 192.168.1.2 netmask 255.255.255.0 设置IP
       * ifup eth0 启用网卡
       * ifdown eth0 禁用网卡
  * netstat -ntpl 查看端口号对应的服务 -n不反解域名 -t只查看TCP连接 -p查看对应服务 -l之查看listen状态服务(默认是established)
  * route
    1. route -n 查看网关 -n不反解域名(较快)
    2. 添加网关
       * route add default gw 192.168.1.1 添加默认网关 192.168.1.1
       * route add -host 10.0.0.2 gw 192.168.1.2 添加访问10.0.0.2主机的专用网关192.168.1.2
       * route add -net 10.0.0.0 netmask 255.255.255.0 gw 192.168.1.3 添加访问10.0.0.0网段的专用网关192.168.1.3
* iproute2
  * ip
    1. ip addr ls 查看网络状态 ifconfig
    2. ip link set dev eth0 up 启用网卡 ifup eth0
    3. ip addr add 192.168.1.2/24 dev eth0 配置IP地址 ifconfig eth0 192.168.1.2 netmask 255.255.255.0
    4. ip route add 10.0.0/24 via 192.168.1.3 配置10.0.0.0网段的专用网关 route add -net 10.0.0.0 netmask 255.255.255.0 gw 192.168.1.3
  * ss 同netstat基本一致
* mii-tool eth0 查看网卡物理连接情况
* ping 192.168.1.1 ICMP判断是否能连通主机
* traceroute -w 1 192.168.1.1 查看到192.168.1.1路由路径-w 1长时间等待只等待1秒
* mtr 更详细的路由信息mtr为My traceroute的简写
* nslookup www.baidu.com 解析域名www.baidu.com对应IP
* telnet www.baidu.com 80 查看连接到www.baidu.com域名对应主机80端口是否可用
  * 退出使用ctrl+] 输入quit退出
* tcpdump -i any -n host 10.0.0.1 and port 80 -w /tmp/filename 抓取-i any任意网卡 -n不反解域名host 10.0.0.1 port 80的数据包 -w保存至/tmp/filename中

### SysV和systemd管理网络服务
* /etc/sysconfig/network-scripts/ifcfg-eth0 网卡eth0配置
  * 配置完成后SysV(自CentOS8后不再启用该服务)或者systemd(推荐)重启服务使服务生效
* SysV
  * service network start|stop|restart 启动|停止|重启网络服务
  * chkconfig --list network 显示各个级别SysV服务启用情况
* systemd
  * systemctl list-unit-files NetworkManager.service 查看服务启用情况
  * systemctl start|stop|restart NetworkManager 启动|停止|重启NetworkManager
  * systemctl enable|disable NetworkManager 启用|禁用NetworkManager
* /etc/hosts 配置主机名和IP对应关系
  * hostname 查看主机名
    * hostname temporary 临时修改主机名为temporary
    * hostname set-hostname permanent 永久修改主机名为permanent
  * `127.0.0.1 permanent`添加至hosts文件

## 操作系统

Linux含义：
1. Linus编写的操作系统内核
2. 广义的操作系统

Linux版本：
* 内核版本，如4.18.0
   
| 主版本号 | 次版本号               | 末版本号 |
| -------- | ---------------------- | -------- |
| 4        | 18                     | 0        |
|          | 奇数开发版，偶数稳定版 |          |

* 发行版本

| 发行版 | 特点                   |
| ------ | ---------------------- |
| redhat | 稳定、经过测试、收费   |
| fedora | 新功能、社区维护、免费 |
| centos | 基于redhat、免费       |
| debian | 图形界面桌面系统、免费 |
| ubuntu | 图形界面桌面系统、免费 |

### 系统操作

* 图形终端
* 命令行终端
* 远程终端(SSH、VNC)

#### 常用目录结构

* / 根目录
* /root 用户root的家目录
* /home/username 用户username的家目录
* /etc 配置文件目录
* /bin 命令目录
* /sbin 管理命令目录
* /usr/bin /user/sbin 系统预装的其它命令

#### 帮助命令

| 命令 | 说明   | 演示                             |
| ---- | ------ | -------------------------------- |
| man  | manual | man -a man (查看man所有章节帮助) |
| help | help   | help cd(外部)或ls --help(内部)   |
| info | info   | info ls(比help命令更详细)        |

```
1   Executable programs or shell commands
2   System calls (functions provided by the kernel)
3   Library calls (functions within program libraries)
4   Special files (usually found in /dev)
5   File formats and conventions eg /etc/passwd
6   Games
7   Miscellaneous (including macro packages and conventions), e.g. man(7), groff(7)
8   System administration commands (usually only for root)
9   Kernel routines [Non standard]
```

#### 文件管理

一切皆文件

* 文件查看
  1. pwd
  2. cd
  3. ls
* 目录文件的创建和删除
  1. mkdir
  2. rmdir
  3. rm -r
* 通配符
  1. * 匹配任何字符串
  2. ? 匹配1个字符
  3. [xyz] 匹配xyz任意一个字符
  4. [a-z] 匹配一个范围
  5. [!xyz]或[^xyz] 不匹配
* 文件操作
  1. cp -r 文件夹 -p 时间 -a 时间和权限
  2. mv
* 文件内容查看
  1. cat 文件内容显示到终端
  2. head 查看文件开头
  3. tail 查看文件结尾 -f 文件更新同步更新
  4. wc 统计文件内容信息
* 打包和压缩
  1. tar cf /tmp/etc.bak.tar /etc
  2. tar czf /tmp/etc.bak.tar.gz /etc
  3. tar cjf /tmp/etc.bak.tar.bz2 /etc
* vi
  * 配置文件 /etc/vimrc
  * normal 正常模式
    * yy 复制
    * dd 剪切
    * p 粘贴到上一行
    * P 粘贴到下一行
    * i 光标前插入
    * I 行首插入
    * a 光标后插入
    * A 行尾插入
    * o 行后新建行
    * O 行前新建行
    * u 撤销
    * g 移到首行
    * G 移到尾行
  * insert 插入模式
    * 正常输入文本
  * command 命令模式
    * :set nu 显示行号
    * :w 保存
    * :q 退出
    * :! ip addr 临时执行其它命令 如ip addr
    * :/x 查找 如查找x
    * :s/x/X 单次替换当前行中x为X
    * :s/x/X/G 多次替换当前行中x为X
    * :3,5s/x/X 单次替换3-5行中所有的x为X
    * :%s/x/X 单次替换所有行中的x为X
  * visual 可视模式
    * v 字符可视模式
    * V 行可视模式
    * ctrl+v 块可视模式

#### 进程管理

* ps 进程状态
  * ps -e | more 
  * ps -ef 显示 UID启动用户ID(有效用户ID) PPID父进程ID CMD执行文件路径
  * ps -L 显示 LWP线程
* pstree 进程状态树
* top 动态显示负载情况
  * load average: 0.00, 0.00, 0.00 平均负载:1分钟,5分钟,15分钟的采样结果
  * Tasks: total running sleeping stopped zombie 
  * %Cpu(s): us sy ni id wa hi si st
    * 所有状态数值相加为100
  * PID USER PR NI VIRT RES SHR S %CPU %MEM TIME+ COMMAND

%Cpu(s)状态说明

| 状态 | 含义            | 说明                                          |
| ---- | --------------- | --------------------------------------------- |
| us   | user CPU time   | 用户空间占用CPU百分比                         |
| sy   | system CPU time | 内核空间占用CPU百分比                         |
| ni   | nice CPU time   | 用户进程空间内改变过优先级的进程占用CPU百分比 |
| id   | idle            | 空闲CPU百分比                                 |
| wa   | iowait          | 等待输入输出的CPU时间百分比                   |
| hi   | hardware irq    | 硬件中断                                      |
| si   | software irq    | 软件中断                                      |
| st   | steal time      | 实时                                          |

top参数说明

| 参数    | 说明                                                        |
| :------ | :---------------------------------------------------------- |
| PID     | 进程号                                                      |
| USER    | 用户名                                                      |
| PR      | 优先级                                                      |
| NI      | nice值(-20~19优先级由高到低)                                |
| RES     | 进程使用未被换出的物理内存大小(kb)                          |
| SHR     | 共享内存大小(kb)                                            |
| S       | D 不可中断睡眠<br>R 运行<br>S 睡眠<br>T 跟踪/停止<br>Z 僵尸 |
| %CPU    | CPU使用率                                                   |
| %MEM    | 进程使用的物理内存百分比                                    |
| TIME+   | 进程使用的CPU时间总计(1/100秒)                              |
| COMMAND | 执行的命令                                                  |

* nice -n 10 ./a.sh 执行a.sh优先级10
  * sudo renice -n 15 21610 修改进程21610的优先级为15
* jobs 查看后台运行进程
  * ctrl+z 停止进程
  * fg 切到前台
  * bg 切到后台
* nohup tail -f /var/log/messages & 即使关掉终端tail进程依然得以运行 & 配合后台运行空出终端 tail输出追加到nohup.out文件中

| 守护进程daemon | 一般进程     | 通过nohup启动的进程        |
| -------------- | ------------ | -------------------------- |
| 不需要终端     | 通过终端启动 | 终端启动忽略挂起hangup信号 |
| 不可卸载目录   | 任意工作目录 | 占用进程启动目录           |
| 输出到特殊文件 | 输出到终端   | 输出到特殊文件             |

#### 包管理

| 发行版           | 工具 | 软件包 |
| ---------------- | ---- | ------ |
| centos<br>redhat | yum  | rpm    |
| debian<br>ubuntu | apt  | deb    |

* yum源安装
  * /etc/yum.repos.d/CentOS-Base.repo yum源配置
  * sudo wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-8.repo 下载阿里云镜像源配置替换官方yum源配置
  * yum makecache 生成缓存
  * yum update 软件升级
* 二进制安装
* 源代码安装
  1. wget https://openresty.org/download/openresty-1.15.8.3.tar.gz 如下载openresty源代码
  2. tar zxf openresty-1.15.8.3.tar.gz 解压源代码包
  3. cd openresty-1.15.8.3.tar.gz 切换到源代码路径
  4. ./configure --prefix=/usr/local/openresty 配置
     1. sudo yum install openssl-devel 安装相应依赖openssl -devel代表开发库
  5. make -j 生成到build目录
  6. make install 将build目录部署到指定的目录当中

### 性能

性能指标

1. 高并发(吞吐)
2. 响应快(延迟)

#### 