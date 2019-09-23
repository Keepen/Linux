//服务端程序
#include "tcp_socket.hpp"

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
  TcpSocket newsock;
  while(1){
    //5.获取新连接
    bool ret = sock.Accept(newsock);
    if(ret == false){
      continue;
    }
    //直到获取到新连接，否则就阻塞等待
    string buf;
    //接收客户端发送的数据
    ret = newsock.Recv(buf);
    if(ret == false){
      newsock.Close();
      continue;
    }
    cout << "client say:" << buf <<endl;
    //7.发送数据
    buf.clear();
    cin >> buf;
    newsock.Send(buf);
  }
  //8.关闭套接字
  sock.Close();
  return -1;
}
