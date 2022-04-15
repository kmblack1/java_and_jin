# 1更新Ubuntu软件
```bash
sudo apt-get update
sudo apt-get -y dist-upgrade
```

# 2.移除旧内核
```bash
uname -a #查看当前系统使用的内核.
sudo dpkg --get-selections|grep linux
sudo apt-get autoremove -y --purge
sudo reboot
```

# 3.修改时区和校准时间
```bash
sudo dpkg-reconfigure tzdata
sudo apt-get install ntpdate // 安装时间同步工具
sudo ntpdate cn.pool.ntp.org // 与网络服务器同步时间
timedatectl status
date // 查看时间是否已同步
```



# 4.安装编译套件和常用工具
```bash
#查找软件包
sudo apt-cache search Visual code
sudo apt-get -y install build-essential libtool automake cmake unzip clang git wget
```

# 5.安装中文支持
```bash
#查看语言支持
locale -a
#安装安装中文支持
sudo locale-gen zh_CN.UTF-8
#安装中文字体
sudo apt-get -y install fonts-wqy-zenhei fonts-wqy-microhei  xfonts-wqy
#设置操作系统语言后重启
sudo reboot
```

# 6.安装Visual Studio Code
[下载](https://code.visualstudio.com/Download)

# 6.安装Qt5
如果没有注册需要先注册
[下载](https://www.qt.io/download)


# 7.下载安装Clion
如果没有注册需要先注册
[下载](https://www.jetbrains.com/clion/)