#include "tcpsocket.hpp"

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
    TcpSocket newsock;
    bool ret = sock.Accept(newsock);
    if(!ret){
      continue;
    }
    string buf;
    CHECK(newsock.Recv(buf));
    cout << "client say: " << buf << endl;

    buf.clear();
    cin >> buf;
    CHECK(newsock.Send(buf));
  }
  sock.Close();
  return 0;
}
