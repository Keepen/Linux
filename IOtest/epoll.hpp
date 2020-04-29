//封装epoll的基本操作，向外提供添加监控/移除监控/获取就绪信息接口
#include <iostream>
#include <cstdio>
#include <vector>
#include <sys/epoll.h>
#include "tcp_socket.hpp"

class Epoll{
  public:
    Epoll():_epfd(-1){
      //epoll_create(int maxevent); //参数非0即可
      _epfd = epoll_create(1);
      if(_epfd < 0){
        perror("创建epoll失败！！\n");
        exit(-1);
      }
    }

    //1.添加监控
    bool Add(TcpSocket& sock){
      int fd = sock.GetFd();  //先获取描述符
      //epoll_ctl(epoll句柄，具体操作，描述符，对应事件结构)；
      struct epoll_event ev;
      ev.events = EPOLLIN;  //EPOLLIN指的是可读事件
      ev.data.fd = fd;    //设置描述符
      int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
      if(ret < 0){
        perror("epollctl add error");
        return false;
      }
      return true;
    }
    
    //2.移除监控
    bool Del(TcpSocket& sock){
      int fd = sock.GetFd();
      int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
      if(ret < 0){
        perror("epoll ctl del error");
        return false;
      }
      return true;
    }

    //3.返回就绪的描述符就绪的事件信息
    bool Wait(std::vector<TcpSocket>* list, int timeout = 3000){
      //epoll_wait(操作句柄，事件数组，数组节点数目，超时时间)；
      struct epoll_event evs[10];
      int ret = epoll_wait(_epfd, evs, 10, timeout);
      if(ret < 0){
        perror("epollwait error");
        return false;
      }
      else if(ret == 0){
        printf("timout \n");
        return true;
      }
      //就绪的事件都放在evs中，就绪的数目就是返回值
      for(int i = 0;i < ret;++i){
        //判断就绪的事件是否是可读事件，
        //但是这里其实不用判断，因为
        //添加的时候就只监控了可读事件
        if(evs[i].events & EPOLLIN){
          int fd = evs[i].data.fd;
          TcpSocket sock;
          sock.SetFd(fd);
          list->push_back(sock);
        }
      }
      return true;
    }

  private:
    int _epfd;  //epoll的操作句柄
};
