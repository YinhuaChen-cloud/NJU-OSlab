# L0_amgame

jyywiki: https://jyywiki.cn/OS/2022/labs/L0

## 1. 概述

你想拥有自己的操作系统吗？你又感到畏惧吗？有传说表示，写操作系统就是 “跟底层硬件打交道”。如果去网上看 Tutorial (和很多书籍)，开篇就是底层硬件：例如 x86 的 GDT, IDT, TSS、系统启动的过程，十分令人害怕。

但实际上，操作系统和硬件之间的关系被夸大了。硬件的确提供了必要的机制使我们能实现诸如进程/线程切换，但并不意味着除非我们知道有关硬件指令的一切才能编写操作系统。只要能做出适当的抽象，我们编程的难度就会大大降低。实际上，类似的抽象存在于任何真实的操作系统中。

我们首先要树立正确的观念：操作系统在 99.99% 的时候都是遵循 C 语言形式语义的普通程序，只是偶尔会干一些 “出格” 的事情。我们当然会在适当的时候引入这些骚 (黑) 操 (科) 作 (技)，以及它们是如何借助硬件指令和机制实现的。但我们首先要使你相信，经过适当的配置，你完全可以用你熟悉的方式编写操作系统：

```cpp
int main() {
  for (const char *s = "Hello, OS World!\n"; *s; s++) {
    putch(*s);
  }
  return 0;
}
```

而这段代码可以完美地以最高权限运行在计算机硬件上，于是它就具备了 “操作系统” 的第一步：拥有系统的绝对控制权。同样，你应该不会怀疑 putch(ch) 的形式语义是在调试终端上打印一个字符，所以，实现操作系统也没有那么困难嘛！

(大部分内容在 JYYWIKI 上，请自行查看)

---

## 2. 任务：实现可移植的、直接运行在计算机硬件上的小游戏

你需要编写一个直接运行在 AbstractMachine 上 (仅仅启用 IOE 扩展，不使用其他硬件机制如中断/异常/虚拟存储/多处理器) 的小游戏；满足：

1. 有肉眼可辨认的图形输出；
2. 能使用键盘与游戏交互；
3. 游戏使用的内存 (代码、静态数据总和不超过 1 MiB、堆区 heap 使用不超过 1 MiB)，因此你不必考虑复杂的图形；
4. 按 ESC 键后调用 halt() 退出；除此之外游戏程序永不调用 halt() 结束，Game Over 后按键重新开始。

虽然不限实现什么游戏 (简单到弹球、贪吃蛇等 100 行以内搞定的游戏)，但你依然需要在<font color="orange">可移植性/通用性</font>上对你的代码做出一定的保证：

* 兼容很简单的处理器：即小游戏运行中只调用 TRM 和 IOE API，而不使用其他 API，且——这样大家的游戏就可以在各个 CPU 上广为流传下去啦！
* 你的游戏应当是可以在多个硬件体系结构之间移植的，考虑兼容以下情况：
    * 适配不同的屏幕大小。不同的体系结构中，屏幕大小可能是 320×200、640×480、800×600 等，你的游戏最好能在所有分辨率下都获得不错的体验；
    * 同 minilabs 一样，你的程序可能运行在 32/64-bit 平台，因此你应当使用 intptr_t 或 uintptr_t 来保存指针数值；
    * <font color="red">兼容大/小端，因此禁止把不同大小的指针类型强制转换；</font> 

<font color="red">我们会在 x86_64-qemu (64-bit) 和 x86-qemu (32-bit) 两个环境下运行你的游戏，你的游戏必须在两个环境下都能正确编译并运行良好。此外，AbstractMachine 假设 bare-metal 不支持浮点数指令。在 x86_64 的编译选项中，我们添加了 -mno-sse。尽管有浮点数的程序在 ARCH=native 下可以正确编译运行，但可能在其他架构下失效。这么做的目的是使 AbstractMachine 程序能运行在各种 (简化) 的处理器上；</font>尤其是同学们自己编写的处理器——也许下个学期你就可以在自己的 CPU 上运行你自己的游戏了！

<font color="orange">在我们的 am-kernels 项目中包含了一些运行在 AbstractMachine 上的代码，例如 NES 模拟器 LiteNES，欢迎大家参考——LiteNES 只使用了 IOE 的 API，和这个实验中你们所需要实现的类似。</font>

你的游戏可能运行在频率仅有 10MHz 的自制处理器上。因此，如果你希望你的游戏能在未来你自己写的处理器上流畅运行，减少每一帧的运算量和图形绘制就显得比较重要了。你可以计算出一个时钟周期大致允许运行的代码量 (假设绘制一个像素也需要若干条指令)，相应设计游戏的逻辑。

---

## 3. 正确性标准

本实验对游戏不做任何要求：你只需要响应键盘输入、输出变化的图形即可。但 Online Judge 会严格按照上面的要求来评测：

1. 通过 qemu 的 -m 选项限制内存的大小；使用超过规定的内存可能导致程序 crash；
2. 在不同的环境下 (x86-qemu 或 x86_64-qemu, native) 运行你的程序，并且可能修改屏幕的分辨率；
3. 随机发送一定的键盘事件。

但 Online Judge 不试图理解游戏的逻辑，只做基本的正确性检查：

1. 除非按 ESC 键，我们假设游戏程序不结束；如果检测到 halt() 的调用 (包括 assert 失败，我们可能会增加一些额外的合法性检查)，将判定为错 (Runtime Error 或 Wrong Answer)；
2. 如果按 ESC 键后游戏不 halt()，将判定为错；
3. 如果虚拟机或进程发生崩溃、非预期的重启等 (由 undefined behavior 导致，例如访问某非法内存可能导致异常或 CPU Reset)，则被判定为错；
4. 其他违反 specifications 的问题，如在 ioe_init() 之前调用 io_read/io_write，将被判定为错。

此外，我们会链接我们 klib 的参考实现，因此你不必担心你的 klib 有些许问题；但你要保持 klib 库函数的行为与 libc 一致。注意 native 会链接系统本地的 glibc (而非 klib)。

---

## 4. 实验指南

4.1. 实现库函数

4.2. 访问 I/O 设备

4.3. 实现游戏

4.4. 小游戏和操作系统


