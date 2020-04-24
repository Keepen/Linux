#include "select.hpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
  if(argc != 3){
    printf("usage: ./select_srv ip port\n");
    return -1;
  }
  std::string srv_ip = argv[1];
  uint16_t srv_port = std::stoi(argv[2]);

  Select s;

  TcpSocket listen_sock;
  CHECK_RET(listen_sock.Socket());  //创建套接字
  CHECK_RET(listen_sock.Bind(srv_ip, srv_port));  //绑定地址信息
  CHECK_RET(listen_sock.Listen());  //开始监听
s.Add(listen_sock);
  while(1){
    vector<TcpSocket> list;
    bool ret = s.Wait(&list);
    if(ret == false){
      return -1;
    }
    for(auto sock:list){
      if(sock.GetFd() == listen_sock.GetFd()){
        //这个就绪的套接字是监听套接字
        //表示有新连接到来

        TcpSocket client_sock;
        //获取新连接
        bool ret = listen_sock.Accept(client_sock);
        if(ret == false){
          continue;
        }
        //将通信套接字也添加监控
        s.Add(client_sock);
        
      }
      else{
        //表示是普通的通信套接字
        std::string buf;
        ret = sock.Recv(buf);
        if(ret == false){
          s.Del(sock);
          sock.Close();
          continue;
        }
        cout << "client say : " << buf << endl;
        cout << "server say : ";
        buf.clear();
        cin >> buf;
        ret = sock.Send(buf);
        if(ret == false){
          s.Del(sock);
          sock.Close();
          continue;
        }

      }
    }

  }
  listen_sock.Close();
  return 0;
}
