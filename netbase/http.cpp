#include <sstream>
#include "tcpsocket.hpp"

int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "./http 192.168.136.146 9000";
    return -1;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TcpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind("192.168.136.146", port));
  CHECK_RET(sock.Listen());
  while(1){
    TcpSocket clisock;
    if(!sock.Accept(clisock)){
      continue;
    }
    string buf;
    clisock.Recv(buf);
    cout << "buf [" << buf << "]";
    stringstream ss;
    string firstline = "HTTP/1.1 200 OK\r\n";
    string body = "<html><body><h1>哈哈</h1></body></html>";
    ss << "Content Length:" << body.size() << "\r\n";
    string head = ss.str();
    string blankline = "\r\n";
    
    clisock.Send(firstline);
    clisock.Send(head);
    clisock.Send(blankline);
    clisock.Send(body);
  }
  sock.Close();
  return 0;
}
