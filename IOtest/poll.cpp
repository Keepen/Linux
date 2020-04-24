#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <poll.h>
using namespace std;

int main(){
  struct pollfd pfd;
  pfd.fd = 0; //监控标准输入
  pfd.events = POLLIN;  //可读事件
  while(1){
    int ret = poll(&pfd, 1, 4000);
    if(ret < 0){
      perror("poll");
      continue;
    }
    else if(ret == 0){
      cout << "time out" << endl;
      continue;
    }
    //如果是可读事件，就进行输入操作
    if(pfd.revents == POLLIN){
      string s;
      cin >> s;
      cout << s << endl;
    }
  }

  return 0;
}
