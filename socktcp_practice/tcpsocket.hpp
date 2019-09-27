//传输层基于TCP协议的网络通信

#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
using namespace std;

#define CHECK(q) if((q) == false){return -1;}

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

    //2.绑定地址信息
    bool Bind(const string& ip, uint16_t port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(&ip[0]);
      socklen_t len = sizeof(addr);
      int ret = bind(_sockfd, (sockaddr*)&addr, len);
      if(ret < 0){
        cerr << "bind error\n";
        return false;
      }
      return true;
    }

    //3.开始监听--告诉操作系统可以进行三次握手建立连接了
    bool Listen(int backlog = 5){     //backlog--最大并发连接数，最多可以建立连接的会话数目
      int ret = listen(_sockfd, backlog);
      if(ret < 0){
        cerr << "listen error\n";
        return false;
      }
      return true;
    }

    //4.获取已完成连接
    bool Accept(TcpSocket& newsock){    //将任务处理交给新的sock
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
    
    //5.建立连接
    bool Connect(string& ip,uint16_t& port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(&ip[0]);
      socklen_t len = sizeof(addr);
      int ret = connect(_sockfd, (sockaddr*)&addr, len);
      if(ret < 0){
        cerr << "connect error\n";
        return false;
      }
      return true;
    }

    //6.接收数据
    bool Recv(string& buf){
      char tmp[1024] = {0};
      int ret = recv(_sockfd, tmp, 1024, 0);      //最后一个0表示阻塞接收
      if(ret < 0){
        cerr << "recv error\n";
        return false;
      }
      else if(ret == 0){
        cerr << "peer shutdown\n";
        return false;
      }
      
      //接收成功，将数据拷贝到缓冲区内
      buf.assign(tmp, ret);
      return true;
    }

    //7.发送数据
    bool Send(string& buf){
      int ret = send(_sockfd, &buf[0], buf.size(), 0);
      if(ret < 0){
        cerr <<"send error\n";
        return false;
      }
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
    int _sockfd;      //作为网络通信的操作句柄
};

