# SLCore

#更新系统
sudo yum -y update

#安装Development Tools
sudo yum -y groupinstall "Development Tools"
sudo yum -y install wget lrzsz net-tools

#删除随版本安装的git
sudo yum -y remove git

#通过源码安装GIT
sudo yum -y install perl-ExtUtils-MakeMaker zlib-devel curl-devel

cd ~
mkdir git-source && cd git-source
wget https://github.com/git/git/archive/v2.23.0.tar.gz
tar zxvf v2.23.0.tar.gz && cd git-2.23.0
autoconf
./configure --with-curl --with-expat
make -j4
sudo make install

#安装git-lfs
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.rpm.sh | sudo bash
sudo yum -y install git-lfs
git lfs install
git lfs version

#通过源码安装CMake
sudo yum -y install ncurses-devel

cd ~
mkdir cmake-source && cd cmake-source
wget https://github.com/Kitware/CMake/releases/download/v3.15.3/cmake-3.15.3.tar.gz
tar zxvf cmake-3.15.3.tar.gz && cd cmake-3.15.3
./configure
make -j4
sudo make install

#安装mysql
sudo yum -y install libuuid-devel mysql-devel

#ssh
ssh -vT git@civ.bj.leyoutech.com -p 7999

#Clone代码
cd ~
mkdir proj_civ && cd proj_civ
git clone ssh://git@civ.bj.leyoutech.com:7999/civ/common.git Common
git clone ssh://git@civ.bj.leyoutech.com:7999/civ/devops.git DevOps
git clone ssh://git@civ.bj.leyoutech.com:7999/civ/server.git Server
mkdir _build && cd _build
cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ../Server
make -j4

#安装DebugInfo(CentOS7)
修改/etc/yum.repos.d/CentOS-Debuginfo.repo中的enabled参数，改为1
sudo yum -y install nss-softokn-debuginfo --nogpgcheck
（如果glibc按装出现问题，先运行下面中括弧内的命令）
[sudo yum -y install yum-utils]
sudo debuginfo-install glibc





