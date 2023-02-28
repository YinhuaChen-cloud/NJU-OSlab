# M1_pstree

Linux 提供了 procfs，目录是 /proc。如果你进去看一眼，就会发现除了一些比如 cpuinfo 这样的文件，还有很多以数字命名的目录——聪明的你也许已经猜到了，每个目录的名字就是进程号，目录里存储了进程相关的运行时数据。

procfs, 也称为 proc 文件系统，是一个类Unix操作系统中特别的文件系统。它展示了关于进程的信息以及其它的系统信息，提供了一种更加方便和标准化的方式来动态访问内核中的进程数据(相比传统的tracing方法和直接访问内核内存)

pid_t: The pid_t data type is a signed integer type which is capable of representing a process ID. In the GNU C Library, this is an int.

https://en.wikipedia.org/wiki/Procfs 这篇wiki里，提到了关于 procfs 的概念和诸多使用方法

---

## 学到的 C++ 相关东西

### C++ 如何打印变量类型

来源：https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c

```cpp
const int ci = 0;
std::cout << typeid(ci).name() << '\n';
```

来源：https://stackoverflow.com/questions/27117076/c-typeidx-name-returns-ph

通过上述方法只能获得 mangled type string，人类很难看懂

通过下面的方法能够获得更好读的字符串

```cpp
#include <cxxabi.h>
#include <iostream>

int main() {
    char * name = abi::__cxa_demangle("Ph", 0, 0, 0);
    std::cout << name << std::endl;
    free(name);
}
```

多使用 ChatGPT 帮助写代码

---


