#include "thread.h"

__thread char *base, *cur; // thread-local variables  全局变量如果加上 __thread 前缀，那么这个全局变量就是不共享的，每个线程各有一个
__thread int id; // 注意：全局变量储存在静态存储区，既不是heap也不是stack

// objdump to see how thread-local variables are implemented
__attribute__((noinline)) void set_cur(void *ptr) { cur = ptr; }
__attribute__((noinline)) char *get_cur()         { return cur; }

void stackoverflow(int n) { // 这个函数由各个线程运行，参数 n 存于这个线程的栈帧上
  set_cur(&n); // 用 __thread 变量 cur 储存n这个形参的地址
  if (n % 1024 == 0) {
    int sz = base - get_cur(); // 栈是从高地址往低地址减少，因此栈顶在减少，base - cur相当于目前栈顶离栈底的距离
    printf("Stack size of T%d >= %d KB\n", id, sz / 1024); // cur 和 base 都是 char * 类型，因此相减，就是两者相差的字节数
    // 使用sort -nk 6可以看到最后一行是：Stack size of T4 >= 8128 KB，接近8192KB，可以验证每个线程有独立的栈帧
  }
  stackoverflow(n + 1); // 继续exploit 栈帧
}

void Tprobe(int tid) {  // 这个函数由各个线程运行
  id = tid; // 把线程id存入 _thread 变量 id
  base = (void *)&tid;  // char * 指针指向 tid 的地址，也就是形参tid的地址，这毫无疑问在当前线程的栈上，而且接近于栈底
  stackoverflow(0); 
}

int main() {
/* If BUF is NULL, make STREAM unbuffered.
   Else make it use buffer BUF, of size BUFSIZ.  */ // BUFSIZ = 8192，是一个固定值, buffer 必须是大小为 BUFSIZ 的一个 char array
  setbuf(stdout, NULL); // 即, stdout 这个 stream 不使用 buffer
  // 创建4个线程，它们都运行 Tprobe 函数
  for (int i = 0; i < 4; i++) {
    create(Tprobe);
  }
}



