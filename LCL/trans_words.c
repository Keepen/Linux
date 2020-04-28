#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
  int i = 0;
  //for(; i < argc; ++i){
  //  printf("%s\n",argv[i]);
  //}
  char* ret[argc];
  for(i = 1;i < argc;++i){
    ret[i] = argv[i];
  }
  //为每一个字符串都船舰一个子进程来处理
  pid_t pid[argc];
  for(i = 1; i < argc; ++i){
    pid[i] = vfork();
    if(pid[i] < 0){
      printf("%d", i);
      perror("pid error!!!");
      exit(0);
    }
    //父进程
    else if(pid[i] > 0){
      sleep(1);    
    }
    //子进程
    else if(pid[i] == 0){
      execl("/home/wz/code/Practice/LCL/trans_a_word", "trans_a_word", ret[i],NULL);
    }
  } 
  return 0;
}
