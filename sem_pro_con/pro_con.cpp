//使用信号量机制来实现生产者消费者模型
//  实现一个线程安全的环形队列
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
using namespace std;
#define MAXQ 10;    //定义最大结点的数目
class RingQueue{
  public:
    RingQueue(int capacity):_capacity(capacity),_read(0),_write(1){
      _queue.resize(_capacity);
       sem_init(&_lock, 0, 1);
       sem_init(&_idle_space, 0, _capacity);
       sem_init(&_data_space, 0, 0);
    }
    ~RingQueue(){
      sem_destroy(&_lock);
      sem_destroy(&_idle_space);
      sem_destroy(&_data_space);
    }
    bool QueuePush(int data){
      sem_wait(&_idle_space);
      sem_wait(&_lock);
      _queue[_write] = data;
      _write = (_write+1)%_capacity;
      sem_post(&_lock);
      sem_post(&_data_space);
      return true;
    }

    bool QueuePop(int& data){
      sem_wait(&_data_space);
      sem_wait(&_lock);

      data = _queue[_read];
      _read = (_read+1)%_capacity;
      sem_post(&_lock);
      sem_post(&_idle_space);
      return true;
    }

  private:
    vector<int> _queue;
    int _capacity;          //结点的最大数量
    int _read;
    int _write;
    sem_t _lock;
    sem_t _idle_space;      //空闲空间的数目
    sem_t _data_space;      //数据的数目
};

void* thr_pro(void* arg){
  RingQueue* q = (RingQueue*)arg;
  int data = 0;
  while(1){
    cout << "pro data:" << data << endl;
    q->QueuePush(data++);
  }
  return NULL;
}


void* thr_con(void* arg){
  RingQueue* q = (RingQueue*) arg;
  int data = 0;
  while(1){

    cout << "consumer data:" << data << endl;
    q->QueuePop(data);
    ++data;
  }
  return NULL;
}

int main(){
  pthread_t pro[4], con[4];
  RingQueue q(10);
  for(int i = 0; i < 4; ++i){
    int ret = pthread_create(&pro[i], NULL, thr_pro, (void*)&q);
    if(ret < 0){
      cout << "thread create error\n";
      return -1;
    }
  }

  for(int i = 0; i < 4; ++i){
    int ret = pthread_create(&con[i], NULL, thr_con, (void*)&q);
    if(ret < 0){
      cout << "thread create error\n";
      return -1;
    }
  }

  
  for(int i = 0; i < 4; ++i){
    pthread_join(con[i], NULL);
  }
  for(int i = 0; i < 4; ++i){
    pthread_join(pro[i], NULL);
  }

  return 0;
}
