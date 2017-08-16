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

