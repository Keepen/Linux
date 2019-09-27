#include <pthread.h>
#include "tcpsocket.hpp"
//使用多线程来处理通信

//定义一个线程入口函数
void* thr_start(void* arg){
  TcpSocket* newsock = (TcpSocket*)arg;
  while(1){
    string buf;
    bool ret;
    ret = newsock->Recv(buf);
    if(ret == false){
      continue;
    }
    cout << "client say: " << buf << endl;
    buf.clear();
    cin >> buf;
    ret = newsock->Send(buf);
    if(ret == false){
      continue;
    }
  }
  newsock->Close();
  delete newsock;
}


int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "./tcp_srv 192.168.136.147 9000";
    return -1;
  }

  string ip =argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSocket sock;
  CHECK(sock.Socket());
  CHECK(sock.Bind("0.0.0.0", port));
  CHECK(sock.Listen());
  while(1){
    TcpSocket* newsock =new TcpSocket();
    bool ret = sock.Accept(*newsock);
    if(!ret){
      continue;
    }
    pthread_t tid;
    pthread_create(&tid, NULL, thr_start, (void*)newsock);
    pthread_detach(tid);
  }
  sock.Close();
  return 0;
}
