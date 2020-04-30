#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
using namespace std;


#define CHECK_RET(q) if((q) == false) {return -1;}

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
    bool Bind(const string& ip, u_int16_t port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(&ip[0]);
      addr.sin_port = htons(port);
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
      int ret = listen(_sockfd, backlog);
      if(ret < 0){
        cerr << "listen error\n";
        return false;
      }
      return true;
    }

    //4.建立连接
    bool Connect(string& ip,u_int16_t port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(&ip[0]);
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

    //6.接收数据
    bool Recv(string& buf){
      char tmp[1024] = {0};
      int ret = recv(_sockfd, tmp, 1023, 0);
      if(ret < 0){
        cerr << "recv error\n";
        return false;
      }
      else if(ret == 0){
        cerr << "peer shutdown\n";
        return false;
      }
      //接收成功，接收到了ret个字节的数据
      buf.assign(tmp, ret);
      return true;
    }

    //7.发送数据
    bool Send(string& buf){
      int ret = send(_sockfd, &buf[0], buf.size(), 0);
      if(ret < 0){
        cerr << "send error\n";
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
      return true;
    }

    ~TcpSocket(){
      Close();
    }
  private:
    int _sockfd;
};
