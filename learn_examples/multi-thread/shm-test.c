// 证明线程之间共享内存

#include "thread.h"

int x = 0; // 10个线程恭喜全局变量 x

void Thello(int id) {
  int y = 0; // 局部变量，每个线程都有一个y的副本，所以，局部变量并不共享
  // 由于局部变量存在栈帧上，可以推断，线程之间的栈帧是独立的
  usleep(id * 100000);
  printf("x: Hello from thread #%c\n", "123456789ABCDEF"[x++]);
  printf("y: Hello from thread #%c\n", "123456789ABCDEF"[y++]);
}

int main() {
  for(int i = 0; i < 10; i++) {
    create(Thello);
  }
}
