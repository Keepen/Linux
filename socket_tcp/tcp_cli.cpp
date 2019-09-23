//客户端程序
#include "tcp_socket.hpp"

int main(int argc, char* argv[]){
  //判断参数是否足够
  if(argc != 3){
    cout << "./tcp_cli 192.168.136.146 9000";
    return -1;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  //1.创建套接字
  TcpSocket sock;
  CHECK_RET(sock.Socket());
  //2.绑定地址信息
  //CHECK_RET(sock.Bind(ip, port));
  //4.建立连接
  CHECK_RET(sock.Connect(ip, port));
  while(1){
    string buf;
    cin >> buf;
    //5.向服务端发送数据
    bool ret = sock.Send(buf);
    if(ret == false){
      sock.Close();
      return -1;
    }
    buf.clear();
    //6.接收服务端的数据
    ret = sock.Recv(buf);
    if(ret == false){
      sock.Close();
      return -1;
    }
    cout << "server say:" << buf << endl;
  }

  sock.Close();
  return 0;
}
