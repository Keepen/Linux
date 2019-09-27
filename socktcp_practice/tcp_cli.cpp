#include "tcpsocket.hpp"

int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "./tcp_cli 192.168.136.147 9000";
    return -1;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  
  TcpSocket sock;
  CHECK(sock.Socket());
  //CHECK(sock.Bind(ip, port));
  CHECK(sock.Connect(ip, port));
  while(1){
    string buf;
    cout << "client send: ";
    cin >> buf;
    CHECK(sock.Send(buf));

    buf.clear();

    CHECK(sock.Recv(buf));
    cout << "server say: " << buf << endl; 
  }
  sock.Close();
  return 0;
}
