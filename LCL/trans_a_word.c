#include <stdio.h>

int main(int argc, char* argv[]){
  char* s = argv[1];
  //printf("%s\n",s);
  //printf("%c",*s);
  char ret[1024];   //保存转换后的结果
  int i = 0;        //遍历字符串的下标
  while(s[i] != '\0'){
    //大小写转换
    if(s[i] >= 'A' && s[i] <= 'Z'){
      ret[i] = s[i] + 32;
    }
    if(s[i] >= 'a' && s[i] <= 'z'){
      ret[i] = s[i] - 32;
    }
    i++;
  }
  printf("%s\n",ret);
  return 0;
}
