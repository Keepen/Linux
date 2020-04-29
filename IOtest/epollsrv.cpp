//epoll模型的服务器

#include <iostream>
#include <string>
#include "epoll.hpp"


int main(int argc, char* argv[]){
  if(argc != 3){
    printf("usage: ./epollsrv ip port\n");
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = std::stoi(argv[2]);

  TcpSocket lst_sock;
  //创建套接字
  CHECK_RET(lst_sock.Socket());
  //绑定地址信息
  CHECK_RET(lst_sock.Bind(ip, port));
  //开始监听
  CHECK_RET(lst_sock.Listen());

  Epoll epoll;
  CHECK_RET(epoll.Add(lst_sock));

  while(1){
    std::vector<TcpSocket> list;
    bool ret = epoll.Wait(&list);
    if(ret == false){
      return -1;
    }

    for(size_t i =0;i < list.size();++i){
      if(list[i].GetFd() == lst_sock.GetFd()){
        TcpSocket cli_sock;
        bool ret = lst_sock.Accept(cli_sock);
        //在每一次出错的地方移除监控
        if(ret == false){
          epoll.Del(list[i]);
          continue;
        }
        epoll.Add(cli_sock);
      }
      else{
        std::string buf;
        ret = list[i].Recv(buf);
        if(ret == false){
          epoll.Del(list[i]);
          list[i].Close();
          continue;
        }
        std::cout << "client say : " << buf << std::endl;
        buf.clear();
        std::cout << "server say : " ;
        std::cin >> buf;
        ret = list[i].Send(buf);
        if(ret == false){
          epoll.Del(list[i]);
          list[i].Close();
          continue;
        }
      }
    }
  }
  lst_sock.Close();
  return 0;
}
