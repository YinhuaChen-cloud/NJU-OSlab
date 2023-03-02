#include "thread.h"

#define A 1
#define B 2

atomic_long count;

// 这个测试算法的原始版本应该只能在单核CPU上成立（不加 mfence 时）

void critical_section() {
  long cnt = atomic_fetch_add(&count, 1);
  if(atomic_load(&count) != 1) {
    printf("%ld\n", count);
  }
  atomic_fetch_add(&count, -1);
}

int volatile x = 0, y = 0, turn = A;

void TA() {
    while (1) {
/* PC=1 */  x = 1;
/* PC=2 */  turn = B;
            asm volatile(
              "mfence;"
            );
/* PC=3 */  while (y && turn == B) ;
            critical_section();
/* PC=4 */  x = 0;
    }
}

void TB() {
  while (1) {
/* PC=1 */  y = 1;
/* PC=2 */  turn = A;
            asm volatile(
              "mfence;"
            );
/* PC=3 */  while (x && turn == A) ;
            critical_section();
/* PC=4 */  y = 0;
  }
}

int main() {
  count = 0;
  create(TA);
  create(TB);
}
