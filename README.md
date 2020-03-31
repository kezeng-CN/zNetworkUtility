# 前言

* 网络
* 操作系统
* 数据库
* C++
* Linux 命令


## 网络

OSI 七层网络模型

|      OSI 网络七层模型      | TCP/IP 模型 |
| :------------------------: | :---------: |
| 应用层<br>表示层<br>会话层 |   应用层    |
|           传输层           |   传输层    |
|           网络层           |   网络层    |
|      链路层<br>物理层      |   接口层    |

### 应用层

* http 状态码

### 传输层

* TCP 和 UDP 的差异

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
    * 网卡命名
       * eth0 第一块网卡
       * CentOS一致性网络设备命名
         * eno1 板载网卡
         * ens33 PCI-E网卡
         * enp0s3 无法获取物理信息的PCI-E网卡
    * 网络接口命名修改
       1. 编辑/etc/default/grub
       2. 增加biosdevname=0 net.ifnnet.ifnames=0
       3. $sudo grub2-mkconfig -o /boot/grub2/grub.cfg 更新grub
       4. $sudo reboot 重启
       * biosdevname和net.ifnames两个参数
         * biosdevname=0 net.ifnames=1 则 网卡名为ens33
         * biosdevname=1 net.ifnames=0 则 网卡名为em1
         * biosdevname=0 net.ifnames=0 则 网卡名为eth0
    * 配置IP
       * ipconfig eth0 192.168.1.2 netmask 255.255.255.0 设置IP
       * ifup eth0 启用网卡
       * ifdown eth0 禁用网卡
  * netstat -ntpl 查看端口号对应的服务 -n不反解域名 -t只查看TCP连接 -p查看对应服务 -l之查看listen状态服务(默认是established)
  * route
    * route -n 查看网关 -n不反解域名(较快)
    * route add default gw 192.168.1.1 添加默认网关 192.168.1.1
    * route add -host 10.0.0.2 gw 192.168.1.2 添加访问10.0.0.2主机的专用网关192.168.1.2
    * route add -net 10.0.0.0 netmask 255.255.255.0 gw 192.168.1.3 添加访问10.0.0.0网段的专用网关192.168.1.3
* iproute2
  * ip
    * ip addr ls 查看网络状态 ifconfig
    * ip link set dev eth0 up 启用网卡 ifup eth0
    * ip addr add 192.168.1.2/24 dev eth0 配置IP地址 ifconfig eth0 192.168.1.2 netmask 255.255.255.0
    * ip route add 10.0.0/24 via 192.168.1.3 配置10.0.0.0网段的专用网关 route add -net 10.0.0.0 netmask 255.255.255.0 gw 192.168.1.3
  * ss 同netstat基本一致
* mii-tool eth0 查看网卡物理连接情况
* ping 192.168.1.1 ICMP判断是否能连通主机
* traceroute -w 1 192.168.1.1 查看到192.168.1.1路由路径-w 1长时间等待只等待1秒
* mtr 更详细的路由信息mtr为My traceroute的简写
* nslookup www.baidu.com 解析域名www.baidu.com对应IP
* telnet www.baidu.com 80 查看连接到www.baidu.com域名对应主机80端口是否可用
  * 退出使用ctrl+] 输入quit退出
* tcpdump -i any -n host 10.0.0.1 and port 80 -w /tmp/filename 抓取-i any任意网卡 -n不反解域名host 10.0.0.1 port 80的数据包 -w保存至/tmp/filename中
* iftop -P 查看本地端口的带宽使用情况 -P混杂模式

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
* Linus编写的操作系统内核
* 广义的操作系统

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

### 启动过程

1. BIOS
2. MBR
   * dd if=/dev/sda of=mbr.bin bs=446 count=1 记录MBR信息到mbr.bin文件
   * dd if=/dev/sda of=mbr2.bin bs=512 count=1 记录含分区表的MBR信息到mbr2.bin文件
     * hexdump -C mbr2.bin | more 按照16进制显示mbr2.bin文件 以55aa结尾表示此设备可引导
3. BootLoader(grub)
   * /boot/grub2/ 根据grub的配置文件选择内核
   * grub2-editenv list 查看默认引导内核
   * uname -r 查看当前使用内核
4. kernel
   * centos6使用/usr/sbin/init进行引导
     * /etc/rd.d 读取各项服务启动脚本
   * centos6以后的版本使用systemd进行引导
     * /etc/systemd/system 默认启动级别
       * /usr/lib/systemd/system 读取各项服务配置文件
5. systemd 启动各项服务
6. 系统初始化
7. shell
   * file /sbin/grub2-mkconfig 系统中通过命令执行的脚本代表该文件实际是脚本

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
  *. pwd
  *. cd
  *. ls
* 目录文件的创建和删除
  *. mkdir
  *. rmdir
  *. rm -r
* 通配符
  *. * 匹配任何字符串
  *. ? 匹配1个字符
  *. [xyz] 匹配xyz任意一个字符
  *. [a-z] 匹配一个范围
  *. [!xyz]或[^xyz] 不匹配
* 文件操作
  *. cp -r 文件夹 -p 时间 -a 时间和权限
  *. mv
* 文件内容查看
  *. cat 文件内容显示到终端
  *. head 查看文件开头
  *. tail 查看文件结尾 -f 文件更新同步更新
     *. tail -f /var/log/message 通过tail查看系统日志
     *. tail -f /var/log/secure 通过tail查看安全日志
     *. tail -f /var/log/dmesg 通过tail查看内核日志
     *. tail -f /var/log/cron 通过tail查看即发(周期性)任务日志
  *. wc 统计文件内容信息
* 打包和压缩
  *. tar cf /tmp/etc.bak.tar /etc
  *. tar czf /tmp/etc.bak.tar.gz /etc
  *. tar cjf /tmp/etc.bak.tar.bz2 /etc
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
* sar -uh 1 10 系统状态显示 -u显示cpu -r内存 -b磁盘 -d每一块磁盘 -q进程的使用 1时间间隔1s 10次数

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

* screen 在screen环境中遇到的中断可以恢复
  * $ sudo yum install epel-release 在CentOS8环境中需要先安装依赖
  * ctrl+a d 退出(detached)screen环境
  * screen -ls 查看screen会话
  * screen -r 22592 恢复22592的screen会话
* init 由内核启动的
  * 0 停机或者关机(千万不能将initdefault设置为0)
  * 1 单用户模式
  * 2 多用户模式(NFS不可用Net File System)
  * 3 完全多用户模式(标准的运行级别)
  * 4 安全模式
  * 5 图形化(图形界面)
  * 6 重启(千万不要把initdefault设置为6)
* service
  * /etc/init.d 存放service启动脚本
  * chkconfig --list 查看服务在不同init级别下的启动状态
  * service network start|stop|restart 启动|停止|重启网络服务
* systemctl
  * /usr/lib/systemd/system/ 存放systemctl启动脚本
  * ls -l /lib/systemd/system/runlevel*.target 查看服务init级别
    * systemctl get-default 查看当前运行的级别
    * systemctl set-default multi-user.target 设置默认运行级别为多用户级别(字符级别)
      * runlevel0.target -> poweroff.target
      * runlevel1.target -> rescue.target
      * runlevel2.target -> multi-user.target
      * runlevel3.target -> multi-user.target
      * runlevel4.target -> multi-user.target
      * runlevel5.target -> graphical.target
      * runlevel6.target -> reboot.target
  * systemctl list-dependencies 查看服务依赖关系
  * systemctl start|stop|restart NetworkManager 启动|停止|重启NetworkManager
  * systemctl enable|disable NetworkManager 启用|禁用NetworkManager
* selinux 安全子系统 强制访问控制
  * getenforce 查看当前selinux状态
    * enforcing - SELinux security policy is enforced.
    * permissive - SELinux prints warnings instead of enforcing.
    * disabled - No SELinux policy is loaded.
  * vim /etc/selinux/config 修改selinux级别(重启生效)
  * setenforce 0 临时修改selinux级别为Permissive(重启失效)
    * 1 Enforcing
    * 0 Permissive

#### 内存和磁盘管理

* free 按照KB为单位查看内存大小 -m按单位MB -g按单位GB
* fdisk -l 查看磁盘分区
  1. fdisk /dev/sdb 在磁盘sdb上创建分区
  2. 根据fdisk帮助创建:n创建分区→p创建主\e拓展分区→分区数量→分区大小→w保存
  3. mkfs.ext4 /dev/sdb1 通过ext4格式化分区sdb1
  4. mkdir /mnt/sdb1 创建挂载路径
  5. mount /dev/sdb1 /mnt/sdb1/ 挂载sdb1磁盘到/mnt/sdb1路径
  6. /etc/fstab 新增`/dev/sdb1 /mnt/sdb1 ext4 defaults 0 0`
* parted -l 通过分区工具查看磁盘分区
  * 大于2t的磁盘分区
* df -h 查看分区对应目录
* du -h 查看文件占用实际大小
  1. dd if=/dev/zero bs=4M count=10 of=hole seek=20 创建一个空洞文件
  2. ls -lh hole 展示文件大小120M
  3. du -h 展示文件占用磁盘空间40M
* 软raid使用
  1. mdadm -C /dev/md0 -a yes -l1 -n2 /dev/sdb[1,2] 创建软raid分区md0 -C创建 -a yes询问全部选择yes -l1模式raid1 -n2依赖2块磁盘 /dev/sdb[1,2]指定分区sdb1和sdb2
     * mdadm -S /dev/md0 删除raid分区md0
  2. mdadm -D /dev/md0 查看raid分区md0
  3. echo DEVICE /dev/sdb[1,2] >> /etc/mdadm.conf 追加磁盘信息至mdadm配置文件
  4. mdadm -Evs >> /etc/mdadm.conf
  5. mkfs.xfs /dev/md0 格式化raid分区md0
  6. mount /dev/md0 /mnt/md0
* LVM逻辑卷
  1. pvcreate /dev/sd[b,c,d]1 通过sdb1 sdc1和sdd1创建物理卷
  2. pvs 查看物理卷
  3. vgcreate vg1 /dev/sd[b,c]1 通过sdb1和sdc1创建卷组vg1
  4. vgs 查看卷组
  5. lvcreate -L 100M -n lv1 vg1 创建逻辑卷 -L 100M指定大小100M -n lv1逻辑卷名称 vg1所属卷组名称
  6. mkdir /mnt/lv1 创建逻辑卷对应挂载点
  7. mkfs.xfs /dev/vg1/lv1 格式化逻辑卷lv1
  8. mount /dev/vg1/lv1 /mnt/lv1/ 挂载逻辑卷到对应路径
  9. /etc/fstab 新增逻辑卷挂载节点
  10. vgextend vg1 /dev/sdd1 拓展卷组vg1
  11. lvextend -L 59G /dev/vg1/lv1 拓展逻辑卷lv1
  12. xfs_growfs /dev/vg1/lv1 文件系统拓展

#### 文件系统

| 文件系统 | 特点   |
| -------- | ------ |
| ext4     |
| xfs      |
| ntfs     | 有版权 |

* ext4
  * 超级块
  * 超级块副本
  * i节点(inode)
    * 文件名称记录在父目录的i节点中
    * 文件权限 组 用户 大小 日期 记录在i节点中 即ls查看的信息
  * 数据块(datablock)
    * 当目录或者文件数据过多i节点记录数据块位置 数据块记录数据 du统计数据块大小
  * facl 文件访问控制(Filesystem Access Control List)
    * getfacl filename 查看文件访问控制权限
    * setfacl -m u:user1:r filename 赋予user1用户读权限 -m赋予权限 -x收回权限
    * setfacl -m g:user2:rw filename 赋予user2用户组读写权限 
* xfs文件系统用户磁盘配额
  1. mkfs.xfs -f /dev/sdb1 格式化/dev/sdb1为xfs文件系统
  2. mkdir /mnt/sdb1 创建挂载路径
  3. mount -o uquota,gquota /dev/sdb1 /mnt/sdb1/ 挂载sdb1磁盘到/mnt/sdb1路径 支持uquota用户磁盘配额 gquota组磁盘配额
  4. mount 命令查看挂载状态`/dev/sdb1 on /mnt/sdb1 type xfs (rw,relatime,seclabel,attr2,inode64,usrquota,grpquota)`显示支持usrquota和grpquota
  5. chmod 1777 /mnt/sdb1/ 赋予sdb1可用权限
  6. xfs_quota -xc 'report -ugibh' /dev/sdb1 查看磁盘配额 u用户 g组 i节点 b块 h人性化显示大小
  7. xfs_quota -xc 'limit -u isoft=5 ihard=10 user1' /mnt/sdb1/ 限制user1在sdb1磁盘创建inode数量 isoft=5软限制在宽限时间内可以超过 ihard=10绝对不可以超过
* swap
  * 以磁盘分区挂载swap
    1. mkswap /dev/sdb1 格式化sdb1分区为swap
    2. swapon /dev/sdb1 启用sdb1为swap
    3. swapoff /dev/sdb1 关闭sdb1为swap
    4. /etc/fstab 新增`/dev/sdb1 swap swap defaults 0 0`
  * 以文件挂载swap
    1. dd if=/dev/zero bs=4M count=1 seek=2047 of=/swapfile 创建swapfile
    2. mkswap /swapfile `mkswap: /swapfile: insecure permissions 0644, 0600 suggested.` 初始化swapfile为swap
    3. chmod 0600 /swapfile 修改文件权限为0600
    4. swapon /swapfile 启用swapfile为swap
    3. swapoff /swapfile 关闭swapfile为swap
    5. /etc/fstab 新增`/swapfile swap swap defaults 0 0`

#### shell脚本
* 内建命令和外部命令
  * 内建命令不创建子进程 对当前shell生效
    * bash ./filename.sh 不需要filename.sh执行权限
    * ./filename.sh 
  * 外部命令创建子进程 对当前shell隔离
    * source ./filename.sh
    * . filename.sh

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

* 高并发(吞吐)
* 响应快(延迟)

#### 