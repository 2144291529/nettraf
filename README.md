# nettraf

show NIC traffic stats

sample output:
```
+-----------------------+-----------------------+
|         eth0          |         eth1          |
|      TX         RX    |      TX         RX    |
|  PPS   BPS   PPS   BPS|  PPS   BPS   PPS   BPS|
+-----------------------+-----------------------+
|   7K   79M    3K    4M|   3K   13M   11K  107M|
|   4K   35M    2K    3M|   6K    7M   35K  418M|
|  32K  373M    6K    6M|   4K   13M   15K  171M|
|   8K   81M    4K    4M|   5K    6M   26K  305M|
|   8K   73M    4K    4M|   7K   10M   35K  410M|
|   6K   52M    4K    4M|   3K    7M   12K  130M|
|  10K  102M    4K    4M|   5K    7M   22K  255M|
|   6K   55M    3K    3M|   3K    6M    7K   76M|
|   7K   59M    5K    5M|   5K    9M   24K  277M|
|   6K   59M    3K    3M|   3K    6M   16K  184M|
|   5K   36M    3K    3M|   2K    4M    4K   35M|
|   8K   87M    3K    3M|   4K    6M   23K  276M|
|   5K   41M    4K    5M|  21K  170M   37K  312M|
|   4K   36M    3K    4M|   3K    6M    8K   88M|
|   5K   38M    3K    3M|   5K    7M   22K  259M|
|  11K  107M    7K   30M|   7K   29M   22K  233M|
|  10K  100M    4K    4M|   2K    6M    7K   79M|
|   8K   79M    5K    4M|   7K   40M   21K  216M|
```
---
基于开发者，进行二次编译RPM包，适用于centos7.9，其他版本未测试，理论上不依赖任何依赖包。

## 使用方法
下载rpm软件包，上传到Centos操作系统中
 yum -y install nettraf-master-1.el7.x86_64.rpm

```
[root@ali-hk x86_64]# yum -y install nettraf-master-1.el7.x86_64.rpm
Loaded plugins: fastestmirror, getpagespeed
Examining nettraf-master-1.el7.x86_64.rpm: nettraf-master-1.el7.x86_64
Marking nettraf-master-1.el7.x86_64.rpm to be installed
Resolving Dependencies
--> Running transaction check
---> Package nettraf.x86_64 0:master-1.el7 will be installed
--> Finished Dependency Resolution

Dependencies Resolved

==================================================================================================
 Package         Arch           Version                Repository                            Size
==================================================================================================
Installing:
 nettraf         x86_64         master-1.el7           /nettraf-master-1.el7.x86_64         0.0  

Transaction Summary
==================================================================================================
Install  1 Package

Installed size: 0  
Downloading packages:
Running transaction check
Running transaction test
Transaction test succeeded
Running transaction
  Installing : nettraf-master-1.el7.x86_64                                                    1/1 
  Verifying  : nettraf-master-1.el7.x86_64                                                    1/1 

Installed:
  nettraf.x86_64 0:master-1.el7                                                                   

Complete!
[root@ali-hk x86_64]# net
netreport  netstat    nettraf    
[root@ali-hk x86_64]# nettraf eth0
+-----------------------+
|         eth0          |
|      TX         RX    |
|  PPS   BPS   PPS   BPS|
+-----------------------+
|   3     3K    3     1K|
|   4    24K    4     2K|
|   2     1K    3     1K|


```

## 命令
nettarf 网卡名字

