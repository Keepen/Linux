#include <signal.h>
#include <sys/wait.h>
#include "tcp_socket.hpp"

void sigcb(int signo){
  //在一次回调中处理完所有的僵尸进程
  //  -1：等待任意退出的子进程
  //  WNOHANG：设置为非阻塞等待
  while(waitpid(-1, NULL, WNOHANG) > 0);
}



int main(int argc, char* argv[]){
  //先判断参数是否够三个
  if(argc != 3){
    cout << "./tcp_srv 192.168.136.146 9000" << endl;
    return -1;
  }
  signal(SIGCHLD, sigcb);
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TcpSocket sock;
  //1.创建套接字
  CHECK_RET(sock.Socket());
  //2.绑定地址信息
  CHECK_RET(sock.Bind(ip,port));
  //3.开始监听
  CHECK_RET(sock.Listen());
  TcpSocket newsock;
  while(1){
    //5.获取已完成连接
    bool ret = sock.Accept(newsock);
    if(ret == false){
      continue;
    }
    //if(fork() == 0){
    //  if(fork() > 0){
    //    exit(0);
    //  }
    if(fork() == 0){
      while(1){
        //直到获取到新连接，否则就阻塞等待
        string buf;
        //接收客户端发送的数据
        ret = newsock.Recv(buf);
        if(ret == false){
          newsock.Close();
          continue;
        }
        cout << "client say:" << buf << endl;
        //7.发送数据
        buf.clear();
        cin >> buf;
        newsock.Send(buf);
      }
      newsock.Close();
      exit(0);
    }
    newsock.Close();
  }
  //8.关闭套接字
  sock.Close();
  return -1;
  }
