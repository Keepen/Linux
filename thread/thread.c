//对线程的一些操作：
//  创建：
//    int pthread_create(pthread_t* tid, pthread_attr_t* attr, void*(*start_routine)(void*), void* arg);
//    参数：
//      tid：线程id
//      attr：通常置空
//      start_routine：线程入口函数
//      arg：线程入口函数的参数
//  终止：
//    void pthread_exit(void* retval);
//    参数：
//      retval：退出线程的返回值保存在retval中
//    int pthread_cancel(pthread_t thread);
//    参数：
//      thread：让指定线程取消
//  等待：
//    int pthread_join(pthread_t tid, void** retval);
//    参数：
//      retval：将退出线程的返回值保存在retval中
//  分离：
//    int pthread_detach(pthread_t tid);
//    将tid线程分离
//  线程自身id：
//    pthread_t pthread_self();
//    返回调用线程的线程id
#include <stdio.h>
#include <pthread.h>

void* thr_start(void* arg){
  pthread_t tid = pthread_self();
  printf("ordinary thread say:%s -- 普通线程id:%lu\n",(char*)arg, tid);
  return NULL;
}

int main(){
  pthread_t tid;
  int ret = pthread_create(&tid, NULL, thr_start, (void*)"nihao!");
  if(ret != 0){
    printf("create error\n");
    return -1;
  }
  pthread_t mid = pthread_self();
  printf("main thread:%lu ---- ordinary thread:%lu\n",mid, tid);
  //void* retval;
  //ret =  pthread_detach(tid);
  if(ret != 0){
    printf("detach error\n");
    return -1;
  }
  ret = pthread_join(tid, NULL);
  if(ret != 0){
    printf("join error\n");
    return -1;
  }
  return 0; 
}

