//多线程：服务端程序
#include <pthread.h>
#include "tcp_socket.hpp"


void* thr_start(void* arg){
  //直到获取到新连接，否则就阻塞等待
  TcpSocket* newsock = (TcpSocket*)arg;
  while(1){
    string buf;
    //接收客户端发送的数据
    int ret = newsock->Recv(buf);
    if(ret == false){
      newsock->Close();
      return NULL;
    }
    cout << "client say:" << buf <<endl;
    //7.发送数据
    buf.clear();
    cin >> buf;
    newsock->Send(buf);
  }
  newsock->Close();
  delete newsock;
  return NULL;
}



int main(int argc, char* argv[]){
  //先判断参数是否够三个
  if(argc != 3){
    cout << "./tcp_srv 192.168.136.146 9000" << endl;
    return -1;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TcpSocket sock;
  //1.创建套接字
  CHECK_RET(sock.Socket());
  //2.绑定地址信息
  CHECK_RET(sock.Bind(ip,port));
  //3.开始监听
  CHECK_RET(sock.Listen());
  while(1){
    TcpSocket* newsock = new TcpSocket();
    //5.获取新连接
    bool ret = sock.Accept(*newsock);
    if(ret == false){
      delete newsock;
      continue;
    }
    pthread_t tid;
    //创建新线程，让子线程来解决这些问题
    pthread_create(&tid, NULL, thr_start, (void*)newsock);
    pthread_detach(tid);
  }
  //8.关闭套接字
  sock.Close();
  return -1;
}
