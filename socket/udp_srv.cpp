//封装一个udpsocket类
//完成服务端的功能:
//    1.创建套接字
//    2.为套接字绑定地址信息
//    3.接收由服务端发送来的信息
//    4.发送数据
//    5.关闭套接字
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
using namespace std;

#define CHECK_RET(q) if((q)==false){return -1;}

class UdpSocket{
  public:
    UdpSocket():_sockfd(-1){}

    //1.创建套接字
    bool Socket(){
      _sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
      if(_sockfd < 0) {
        cerr << "socket error\n";
        return false;
      }
      return true;
    }

    //2.绑定地址信息
    bool Bind(string& ip, uint16_t& port){
      sockaddr_in addr;
      addr.sin_port = htons(port);
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(addr);
      int ret = bind(_sockfd, (sockaddr*)&addr, len);
      if(ret < 0){
        cerr << "bind error\n";
        return false;
      }
      return true;
    }

    //3.接收数据信息
    bool Recv(string& buf, string& ip,uint16_t& port){
      //先将数据接收到tmp中，再拷贝到buf中去
      char tmp[4096] = {0};   
      sockaddr_in addr;
      socklen_t len = sizeof(addr);
      //ssize_t recvfrom(int sockfd, void *buf,size_t len,int flags,truct sockaddr *src_addr,socklen_t *addrlen);
      //  sockfd：操作句柄--套接字描述符
      //  buf       ：接收缓冲区
      //  len       ：接收的长度
      //  flags     ：标志：0--阻塞接收；
      //  src_addr  ：接收数据的源地址
      //  addrlen   ：接收地址的常长度
      //              必须初始化，输入输出型参数；初始化的大小是希望接收的大小
      //              最后是真正地址的大小
      int ret = recvfrom(_sockfd,tmp,4096,0,(sockaddr*)&addr,&len);
      if(ret < 0){
        cerr << "recv error\n";
        return false;
      }
      //清空buf
      buf.clear();
      //将数据拷贝到buf中
      buf.assign(tmp,ret);
      //解析地址和端口号
      ip = inet_ntoa(addr.sin_addr);
      port = ntohs(addr.sin_port);
      return true;
    }

    //4.发送数据
    bool Send(string& data,const string& ip,uint16_t& port){
      cout << "send";
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

    //5.关闭套接字
    bool Close(){
      if(_sockfd < 0){
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
    int _sockfd;    //作为网络通信的操作句柄
};

int main(int argc, char* argv[]){
  if(argc != 3){
    cerr << "./udp_srv.cpp 192.168.136.145 9000";
    return -1;
  }
  string srv_ip = argv[1];
  uint16_t srv_port = atoi(argv[2]);
  //正真实例化对象
  UdpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind(srv_ip,srv_port));
  while(1){
    string buf;
    sock.Recv(buf,srv_ip,srv_port);
    cout << "client say" << buf << endl;
    cout << "svr send:";
    buf.clear();
    cin >> buf;
   // getline(cin,buf);
    sock.Send(buf,srv_ip,srv_port);
    cout << "send over";
  }
  sock.Close();
  return 0;
}
