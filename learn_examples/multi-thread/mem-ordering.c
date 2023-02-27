// 关于原子操作的靠谱文档:https://en.cppreference.com/w/cpp/atomic/atomic_fetch_xor
#include "thread.h"

int x = 0, y = 0;

// 这个 flag 使用 bit0 和 bit1，默认初始状态是 （0,0）
atomic_int flag; // 来自库 stdatomic.h，这个类型由标准库保证没有数据竞争。不需要使用critical sections去保护这个变量
// 相当于用FLAG表示原子变量的值
#define FLAG atomic_load(&flag) // 返回原子变量的当前值           
#define FLAG_XOR(val) atomic_fetch_xor(&flag, val) // Atomically replaces the value pointed by obj with the result of bitwise XOR between the old value of obj and arg. Returns the value obj held previously.
#define WAIT_FOR(cond) while (!(cond)) ; // 等待 cond 变为 true，否则死循环

 __attribute__((noinline))
void write_x_read_y() { 
  int y_val; // 这是 y 的副本，真正的 y 是全局变量，由两个线程共享
  asm volatile(
    "movl $1, %0;" // x = 1     // $1 表示数字1         %0(asmSymbolicName) 表示遵循第一个约束(constraint)
    "mfence;"
    "movl %2, %1;" // y_val = y  // %1 表示遵循第二个约束，     %2表示遵循第三个约束
    : "=m"(x), "=r"(y_val)  // 输出约束必须以 “=” 或者 “+” 为开头，使用 “=” 时，表示相应的 C operand 只从汇编中读取数据，"+" 表示既读又写
    // "r" 和 “m” 表示 C operand 要读取的数据的存在地，r 表示寄存器， m 表示内存 。 "rm" 表示由编译器选择最高效的方式(编译器会想办法，把数据先弄进你指定的方式的所在地里，再由c operand 读取)
    : "m"(y) // 当约束为0时，表示这个输入必须和 %0 的输出在同一个地方
    // 输入的约束有  i, r, m
  );
  printf("%d ", y_val); // 打印 y 的值
}

 __attribute__((noinline))
void write_y_read_x() {
  int x_val; // 这是 x 的副本，真正的 x 是全局变量，由两个线程共享
  asm volatile(
    "movl $1, %0;" // y = 1
    "mfence;"
    "movl %2, %1;" // x_val = x
    : "=m"(y), "=r"(x_val) : "m"(x)
  );
  printf("%d ", x_val);
}

void T1(int id) { // 参数为 tid
  while (1) {
    WAIT_FOR((FLAG & 1)); // 等待 FLAG bit0 为 1
    write_x_read_y();
    FLAG_XOR(1);  // 把FLAG bit0 设置为0
  }
}

void T2() {
  while (1) {
    WAIT_FOR((FLAG & 2)); 
    write_y_read_x();
    FLAG_XOR(2);  
  }
}

void Tsync() {
  while (1) {
    x = y = 0; // k
    __sync_synchronize(); // full barrier     确保 x=y=0 写入内存，这会编译成一个 mfence 指令
    // usleep(1);            // + delay
    assert(FLAG == 0);
    FLAG_XOR(3); // 对 3 做异或, flag 的 bit0 和 bit1 都设置为1
    // T1 and T2 clear 0/1-bit, respectively
    WAIT_FOR(FLAG == 0); // 等待 FLAG 的值回归到0
    printf("\n"); fflush(stdout);
  }
}

int main() {
  create(T1);
  create(T2);
  create(Tsync);
}
