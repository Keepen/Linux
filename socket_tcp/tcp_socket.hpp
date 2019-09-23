//传输层基于TCP协议的网络通信
//封装一个类
//步骤：
//    1.创建套接字
//    2.为套接字绑定地址
//    3.开始监听--告诉操作系统可以获取连接了
//    4.建立连接
//    5.获取已完成连接
//    6.接收数据
//    7.发送数据
//    8.关闭套接字

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <stdlib.h>
using namespace std;

#define CHECK_RET(q) if((q) == false){return -1;}

class TcpSocket{
  public:
    TcpSocket():_sockfd(-1){}

    //1.创建套接字
    bool Socket(){
      _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_sockfd < 0){
        cerr << "socket error\n";
        return false;
      }
      return true;
    }

    //2.为套接字绑定地址信息
    bool Bind(string& ip, uint16_t port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      //将字符串ip转换成整数ip，用到inet_addr函数，
      //其中参数可以传ip.c_str(),也可以传&ip[0];
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(addr);
      int ret = bind(_sockfd, (sockaddr*)&addr, len);
      if(ret < 0){
        cerr << "bind error\n";
        return false;
      }
      return true;
    }

    //3.开始监听
    bool Listen(int backlog = 5){
      //backlog ：最大并发连接数  -- 同一时间所支持的最大的连接数目
      int ret = listen(_sockfd, backlog);
      if(ret < 0){
        cerr << "listen error\n";
        return false;
      }
      return true;
    }

    //4.建立连接
    bool Connect(string& ip, uint16_t port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);
      socklen_t len = sizeof(addr);
      int ret = connect(_sockfd, (sockaddr*)&addr, len);
      if(ret < 0){
        cerr << "connect error\n";
        return false;
      }
      return true;
    }

    //5.获取已完成连接
    bool Accept(TcpSocket& newsock){
      sockaddr_in addr;
      socklen_t len = sizeof(addr);
      int fd = accept(_sockfd, (sockaddr*)&addr, &len);
      if(fd < 0){
        cerr << "accept error\n";
        return false;
      }
      newsock._sockfd = fd;
      return true;
    }

    //6.发送数据
    bool Send(string& buf){
      int ret = send(_sockfd, &buf[0], buf.size(), 0);
      if(ret < 0){
        cerr << "send error\n";
        return false;
      }
      return true;
    }

    //7.接收数据
    bool Recv(string& buf){
      char tmp[4096] = {0};
      //该函数返回的是实际接收的字节数
      int ret = recv(_sockfd, tmp, 4096, 0);
      //返回值：
      //  -1：错误
      //   0：连接已断开
      //  >0：返回实际接收的字节数
      if(ret < 0){
        cerr << "recv error\n";
        return false;
      }else if(ret == 0){
        cout << "disconnected" << endl;
        return false;
      }
      buf.clear();
      buf.assign(tmp, ret);
      return true;
    }

    //8.关闭套接字
    bool Close(){
      if(_sockfd < 0){
        cerr << "close error\n";
        return false;
      }
      close(_sockfd);
      return true;
    }
    ~TcpSocket(){
      Close();
    }
  private:
    int _sockfd;
};

