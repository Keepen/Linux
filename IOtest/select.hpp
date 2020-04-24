//封装实现一个select类
//外部通过实例化slect对象实现简单的多路转接

#include <iostream>
#include <vector>
#include <sys/select.h>
#include "tcp_socket.hpp"
#include <time.h>
using namespace std;



class Select{
  public:
    Select():_maxfd(-1){
      FD_ZERO(&_rfds);  //初始化清空集合
    }
    //将描述符添加到监控集合
    bool Add(TcpSocket& sock){
      int fd = sock.GetFd();
      FD_SET(fd, &_rfds); //将描述符添加到集合中
      //每次添加，更新最大描述符编号
      _maxfd = _maxfd > fd ? _maxfd : fd;
      return true;
    }

    //从监控集合中移除描述符
    bool Del(TcpSocket& sock){
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
    bool Wait(std::vector<TcpSocket>* list, int mtimeout = 3000){
      struct timeval tv;
      tv.tv_usec = 0;
      tv.tv_sec = mtimeout / 1000;    //mtimeout -- ms
      //因为select会修改描述符集合，返回时将未就绪的描述符全部移除
      //因此不能直接使用_rfds,使用临时集合
      fd_set tmp_rfds = _rfds;
      int nfds = select(_maxfd+1, &tmp_rfds, NULL, NULL, &tv);
      if(nfds < 0){
        perror("select error");
        return false;
      }
      else if(nfds == 0){
        printf("wait timeout");
        return true;
      }
      //有几个就绪了
      //暴力遍历：从0 ~ _maxfd， 逐个判断谁在集合中，谁在，谁就是就绪的，就添加到list中
      for(int i = 0;i <= _maxfd;++i){
        if(FD_ISSET(i, &tmp_rfds)){
          TcpSocket sock;
          sock.SetFd(i);
          list->push_back(sock);
        }
      }
    }
  private:
    int _maxfd;     //最大描述符的编号
    fd_set _rfds;   //可读事件集合
};
