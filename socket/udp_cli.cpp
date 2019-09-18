//封装一个udpsocket类
//来实现客户端的一些操作：
//    1.创建套接字
//    2.为套接字绑定地址
//    3.向服务端放数据
//    4.接收服务端发送的数据
//    5.关闭套接字

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
using namespace std;

#define CHECK_RET(q) if((q) == false){return -1;}

class UdpSocket{
  public:
  UdpSocket():_sockfd(-1){}

  //1.创建套接字
  bool Socket(){
    _sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(_sockfd < 0){
      cerr << "socket error\n";
      return false;
    }
    return true;
  }

  //2.为套接字绑定地址
  bool Bind(const string& ip,uint16_t port){
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    socklen_t len = sizeof(sockaddr_in);
    int ret = bind(_sockfd, (sockaddr*)&addr, len);
    if(ret < 0){
      cerr << "bind error\n";
      return false;
    }
    return true;
  }

  //3.向服务端发送数据
  bool Send(string& data,string& ip,uint16_t& port){
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    socklen_t len = sizeof(addr);
    int ret = sendto(_sockfd, &data[0], data.size(), 0, (sockaddr*)&addr, len);
    if(ret < 0){
      cerr << "send error\n";
      return false;
    }
    return true;
  }

  //4.接收数据
  bool Recv(string& buf, string& ip,uint16_t port){
    cout << "recv";
    char tmp[4096] = {0};
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int ret = recvfrom(_sockfd, tmp, 4096, 0, (sockaddr*)&addr, &len);
    if(ret < 0){
      cerr << "recvfrom error\n";
      return false;
    }
    //将接收的数据拷贝到buf中
    buf.assign(tmp, ret);
    ip = inet_ntoa(addr.sin_addr);
    port = ntohs(port);
    return true;
  }

  //5.关闭套接字
  bool Close(){
    if(_sockfd == -1){
      cerr << "close error\n";
      return false;
    }
    close(_sockfd);
    return true;
  }
  ~UdpSocket(){
    Close();
  }
  private:
    int _sockfd;
};



int main(int argc, char* argv[]){
  if(argc != 3){
    cerr << "./udp_cli.cpp 192.168.136.145 9000";
    return -1;
  }
  string cli_ip = argv[1];
  uint16_t cli_port = atoi(argv[2]);
  UdpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind(cli_ip, 8000));
  while(1){
    //先向服务端发送数据
    string buf;
    getline(cin,buf);
    sock.Send(buf, cli_ip, cli_port);
    buf.clear();
    //接收服务端发送的数据
    sock.Recv(buf, cli_ip, cli_port);
    cout << "srv say:" << buf <<endl;

  }
  sock.Close();
  return 0;
}
