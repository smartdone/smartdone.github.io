# ASM

> 都是从Macos软件安全与逆向上面抄的

## hello world

编写一个c的[helloworld.c](./helloworld.c)

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");     
    return 0;
}
```

使用clang编译成汇编
```shell
clang -O2 -S -masm=intel -fno-asynchronous-unwind-tables helloworld.c
```
得到一个[helloworld.s](./helloworld.s)内容如下：

```asm
	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.intel_syntax noprefix
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
## BB#0:
	push	rbp
	mov	rbp, rsp
	lea	rdi, [rip + L_str]
	call	_puts
	xor	eax, eax
	pop	rbp
	ret

	.section	__TEXT,__cstring,cstring_literals
L_str:                                  ## @str
	.asciz	"Hello, World!"


.subsections_via_symbols
```

用as汇编器把汇编代码编译成.o文件
```shell
as helloworld.s -o helloworld.o
```
然后用ld命令进行链接得到可执行文件：

```shell
ld helloworld.o -e _main -lsystem -arch x86_64 -macosx_version_min 10.12.6 -o helloworld
```

## hello world 详解

在汇编代码里面以`.`开始的都是伪指令，剩下的mov、push、call等都是x86_64指令集中的机器指令。在汇编代码中主要由伪指令和机器指令两部分组成，其中伪指令并不是指令集中真正的指令，一般不会生成机器吗，而是用于只是汇编器如何呢进行汇编操作或者定义数据的指令

代码第一行的`.section`指示了接下来的代码所位于的段和节区，`__TEXT`说明是位于程序的代码段，`__text`则是主程序代码节区，后面的参数则是这个节区的属性。这里的段对应着Mach-O文件格式中的段。

代码第二行代表了运行程序所需要的macOS的版本最低为10.12

第三行表示汇编代码使用intel语法

第四行代码表示我们希望`_main`符号可以被链接器ld使用，因为`_main`是程序的入口函数，所以它必须被链接器使用，并将入口地址写入可执行文件的LC_MAIN加载命令。

`.p2align`指示了接下来的代码的对齐方式`4, 0x90`边上2^4=16字节对齐，指令空隙部分用0x90(nop)进行填充

8-14行为真正的汇编指令，它使用字符串做产生调用了puts函数，然后返回0结束了main函数

16-18行实在代码段的字符串区定义了一个以0结尾的字符串，在汇编代码中可以通过L_str来引用该字符串。

最后的`.subsections_via_symbols`伪指令表示当前的节区可以被内嵌到其他代码中，并且如果没有被其他的代码使用就可以摆剔除掉