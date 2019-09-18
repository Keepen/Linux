//进度条程序
#include <stdio.h>


int main(){
  char bar[10]={0};
  char per=0;
  while(per<10){
    bar[per++]='-';
    printf("%s\r",bar); //回车，不换行，将光标置于行首
    fflush(stdout);     //刷新文件缓冲区
    usleep(100000);

  }
  return 0;
}
