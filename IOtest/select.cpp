#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

int main(){
  //对标准输入进行监控

  //定义可读事件描述符集合
  fd_set rfds;
  //初始化清空集合
  FD_ZERO(&rfds);

  while(1){
    FD_SET(0, &rfds);   //将标准输入添加到监控集合
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    printf("开始监控！！！！\n");
    //select(最大描述符+1， 可读事件集合， 可写事件集合， 异常事件集合， 阻塞超时时间)
    int nfds = select(0+1, &rfds, NULL, NULL, &tv);
    if(nfds == 0){
      printf("监控超时！！!\n");
      continue;
    }
    else if(nfds < 0){
      printf("监控出错！！！\n");
      return -1;
    }
    printf("标准输入事件就绪！！！进行读操作\n"); 
    char buf[1024] = {0};
    int ret = read(0, buf, 1023);
    if(ret <= 0){
      perror("read error!!!");
      return 0;
    }
    printf("read data from stdin:%s\n", buf);
  }
  return 0;
}
