# gFlags

gFlags 命令行参数处理的开源库[gFlags开源地址](https://github.com/gflags/gflags)

## 安装

```bash
git clone https://github.com/gflags/gflags.git
cd gflags
mkdir bin && cd bin
# cmake -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON -DINSTALL_HEADERS=ON -DINSTALL_SHARED_LIBS=ON -DINSTALL_STATIC_LIBS=ON ..
cmake ..
make
sudo make install
ls /usr/local/lib/libgflags* -l # 验证动态库是否存在
ldconfig gflags # 使链接库生效
```

## 定义flag

| 宏定义        | 类型                    |
| ------------- | ----------------------- |
| DEFINE_bool   | boolean                 |
| DEFINE_int32  | 32-bit integer          |
| DEFINE_int64  | 64-bit integer          |
| DEFINE_uint64 | unsigned 64-bit integer |
| DEFINE_double | double                  |
| DEFINE_string | C++ string              |

通过相应的宏定义

```cpp
// foo.cc
#include <gflags/glags.h>

DEFINE_string(ip_addr, "localhost", "ip_addr default 'localhost'");
DEFINE_int32(port, 12345, "port default '12345'");
```

## 使用flag

定义的flag变量需在前面加上`GLAGS_`前缀

```cpp
cout << FLAGS_ip_addr ;
cout << FLAGS_port ;
```

## 初始化所有参数

告诉程序去传入参数,使得定义的flag参数生效

```cpp
google::ParseCommandLineFlags(&argc, &argv, true);
```
