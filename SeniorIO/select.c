//了解高并发模型中的select模型：
//  实现步骤：
//    1.创建用户关心的时间集合
//    2.将fd添加到集合中进行监控
//    3.对返回的就绪fd进行判断目标时间是否在集合中
//        在集合中：对其进行操作
//        每次都要重新将所有的fd咋此添加到集合中，因为select每一次都会将未就绪的fd移出集合

//对标准输入进行监控
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>

int main(){
  fd_set set;
  FD_ZERO(&set);
  FD_SET(0, &set);
  while(1){
    FD_SET(0, &set);  //每次都要将所有的fd在重新添加到集合中
    struct timeval t;
    t.tv_sec = 3;
    t.tv_usec = 0;
    int ret = select(2, &set, NULL, NULL, &t);
    if(ret == 0){
      printf("select timeout!!!\n");
      return -1;
    }
    else if(ret < 0){
      printf("select error!!!\n");
      return -1;
    }
    printf("input======:");
    //监控成功，返回了就绪的fd的数目
    for(int i = 0; i < ret; ++i){
      if(FD_ISSET(i,&set)){
        char buf[1024] = {0};
        int ret = read(i, buf, 1023);
        if(ret < 0){
          printf("read error\n");
          return -1;
        }
        printf("%s\n", buf);
      }
    }
  }
  return 0;
}
