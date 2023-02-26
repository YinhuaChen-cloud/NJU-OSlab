#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdatomic.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREAD 64
enum { T_FREE = 0, T_LIVE, T_DEAD, }; // 线程状态：释放、活着、死了
struct thread {
  int id, status; // 线程 id 和 状态
  pthread_t thread;  // pthread.h 中定义的线程结构体
  void (*entry)(int); // 一个函数指针，返回值为 void，接收一个 int 参数
};

struct thread tpool[NTHREAD], *tptr = tpool; // 定义一个thread结构体数组，大小为NTHREAD=64。再定义一个 thread 结构体指针 tptr，赋值为 tpool，即，指向 tpool 数组第一个元素

void *wrapper(void *arg) {  // arg 是 tptr, 也就是指向 thread 结构体数组里的某一个元素
  struct thread *thread = (struct thread *)arg;
  thread->entry(thread->id); // 这个 entry 以该 thread 的 id 为参数并且执行
  return NULL; // 返回一个NULL，估计是 pthread_create 的函数要求格式是这样的
}

void create(void *fn) { // 参数是一个 void* 指针
  assert(tptr - tpool < NTHREAD); // 断言：已有的 thread 数量不能超过 NTHREAD
  *tptr = (struct thread) {
    .id = tptr - tpool + 1,   // 定义这个新 thread 的 id
    .status = T_LIVE,  // 新创建的 thread 状态为 T_LIVE
    .entry = fn, // entry 表示这个 thread 的入口函数，赋值为 fn，由于指针的宽度都一样，所以传入的函数实际上可以不是 “返回值为void，参数为一个int” 的函数
  };
  // pthread_create 的第一个参数是 pthread_t 指针, 第二个参数是 pthread_attr 参数， 第三个是 pthread 的入口函数（这里的wrapper用来封装入口函数），第四个是这个入口函数的参数
  pthread_create(&(tptr->thread), NULL, wrapper, tptr); 
  ++tptr;
}

void join() { // 等待所有线程运行的 fn 返回。在有其他线程未执行完时死循环，否则返回
  for (int i = 0; i < NTHREAD; i++) { // 遍历 thread 结构体数组
    struct thread *t = &tpool[i]; // 获取第 i 个 thread 结构体的指针
    if (t->status == T_LIVE) { // 如果这个线程的状态是 T_LIVE
      pthread_join(t->thread, NULL); // 等待 pthread_t 类型的变量所指向的线程返回，对于已经终止的线程会立刻返回 
      t->status = T_DEAD; // 返回后，状态设置为 T_DEAD
    }
  }
}   

__attribute__((destructor)) void cleanup() {
  join(); // 在 main 返回时，自动等待所有线程结束
}
