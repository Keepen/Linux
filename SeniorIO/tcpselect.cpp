//封装一个select类，来实现tcp的通信流程

#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <vector>
#include <string>
#include "tcpsocket.hpp"
using namespace std;

class Select{
  public:
    Select(){
      FD_ZERO(&rfds);
      maxfd = -1;
    }
    bool Add(TcpSocket& sock){
      int fd = sock.Getfd();
      FD_SET(fd, &rfds);
      //更改最大fd的数目
      maxfd = maxfd > fd ? maxfd:fd;
      return true;
    }

    bool Del(TcpSocket& sock){
      int fd = sock.Getfd();
      FD_CLR(fd, &rfds);
      //更改maxfd的值
      for(int i = maxfd; i >=0; --i){
        if(FD_ISSET(i,&rfds)){
          maxfd = i;
          return true;
        }
      }
      maxfd = -1;
      return true;
    }

    bool Wait(vector<TcpSocket>&list, int sec = 3){
        struct timeval t;
        t.tv_sec = sec;
        t.tv_usec = 0;
        fd_set tmp = rfds;    //由于每次都要重新添加所有的fd到fd结合中，故先保存下来
        int count = select(maxfd+1, &tmp, NULL, NULL, &t);
        if(count < 0){
          cout << "select error\n";
          return false;
        }
        else if(count == 0){
          cout << "select timeout!!!\n";
          return false;
        }
        //select成功，返回了就绪fd的数目
        for(int i = 0; i < maxfd; ++i){
          if(FD_ISSET(i,&tmp)){
            TcpSocket sock;
            sock.Setfd(i);
            list.push_back(sock);
          }
        }
      return true;
    }

  private:
    fd_set rfds;
    int maxfd;
};

int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "./tcpselect srvip srv port\n";
    return -1;
  }
  string sip = argv[1];
  uint16_t sport = atoi(argv[2]);
  TcpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind(sip, sport));
  CHECK_RET(sock.Listen());
  Select s;
  s.Add(sock);
  vector<TcpSocket> list;
  while(1){
    //在获取新连接之前进行select监控
    if(s.Wait(list) == false){
      sleep(2);
      continue;
    }
    for(auto c_sock:list){
      //判断就绪的fd是否是监听fd
      //对监听fd所作的操作就是  --- 获取新连接
      if(c_sock.Getfd() == sock.Getfd()){
        TcpSocket newsock;
        if(sock.Accept(newsock) == false){
          continue;
        }
        s.Add(newsock);
      }
      //不是监听fd，就执行接收信息的操作
      else{
        string buf;
        //判断是否接收成功，失败就要将其从就绪时间中移除，再关闭套接字
        if(c_sock.Recv(buf) == false){
          s.Del(c_sock);
          c_sock.Close();
          continue;
        }
        cout << "client say:" << buf << endl;
        buf.clear();

        //发送数据
        cin >> buf;
        if(c_sock.Send(buf) == false){
          s.Del(c_sock);
          c_sock.Close();
          continue;
        }
      }
    }
  }
  sock.Close();
  return 0;
}


