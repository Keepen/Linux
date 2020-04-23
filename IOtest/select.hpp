//封装实现一个select类
//外部通过实例化slect对象实现简单的多路转接

#include <iostream>
#include <vector>
#include <sys/select.h>
#include "tcp_socket.hpp"
using namespace std;


class Select{
  public:
    Select():_maxfd(-1){
      FD_ZERO(&_rfds);  //初始化清空集合
    }
    //将描述符添加到监控集合
    bool Add(const TcpSocket& sock){
      int fd = sock.GetFd();
      FD_SET(fd, &_rfds); //将描述符添加到集合中
      //每次添加，更新最大描述符编号
      _maxfd = _maxfd > fd ? _maxfd : fd;
      return true;
    }

    //从监控集合中移除描述符
    bool Del(const TcpSocket& sock){
      int fd = sock.GetFd();
      //从集合中移除描述符
      FD_CLR(fd, &_rfds);
      //更新最大描述符
      for(int i = _maxfd;i >= 0;--i){
        if(FD_ISSET(i, &_rfds)){
          _maxfd = i;
          break;
        }
      }
      return true;
    }

    //开始监控，并返回就绪的socket信息
    bool Wait(std::vector<TcpSocket>* list){
      
    }
  private:
    int _maxfd;     //最大描述符的编号
    fd_set _rfds;   //可读事件集合
};
