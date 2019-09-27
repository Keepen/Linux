#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

//为什么连接会断开？需要用到多进程或者多线程处理
//  多进程处理。

void sigcb(int signo){
  while(waitpid(-1, NULL, WNOHANG) > 0);
}


int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "./tcp_srv 192.168.136.147 9000";
    return -1;
  }

  signal(SIGCHLD, sigcb);

  string ip =argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSocket sock;
  CHECK(sock.Socket());
  CHECK(sock.Bind("0.0.0.0", port));
  CHECK(sock.Listen());
  while(1){
    TcpSocket newsock;
    bool ret = sock.Accept(newsock);
    if(!ret){
      continue;
    }
    if(fork() == 0){
      while(1) {
        string buf;
        CHECK(newsock.Recv(buf));
        cout << "client say: " << buf << endl;

        buf.clear();
        cin >> buf;
        CHECK(newsock.Send(buf));
      }
    }
  }
    sock.Close();
    return 0;
  }
