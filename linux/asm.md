## linux 64位 函数调用参数传递方式

写一个参数很多的函数`asm.c`

```c
#include <stdio.h>

void func1(int a, int b, int c, int d, int e, int f, int g, int h) {

}

int main(int argc, const char *argv[]) {
	func1(1, 2, 3, 4, 5, 6, 7, 8);
	return 0;
}
```

然后使用gcc把他转成汇编

```Shell
gcc -S asm.c -o asm.s
```

转换结果如下

```asm
	.file	"asm.c"
	.text
	.globl	func1
	.type	func1, @function
func1:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -16(%rbp)
	movl	%r8d, -20(%rbp)
	movl	%r9d, -24(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	func1, .-func1
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	pushq	$8
	pushq	$7
	movl	$6, %r9d
	movl	$5, %r8d
	movl	$4, %ecx
	movl	$3, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	func1
	addq	$16, %rsp
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
```

从汇编代码可以看得出来，前六个参数是通过寄存器传递，后面的参数是通过栈来传递。

| 参数   | 传递寄存器 |
| ---- | ----- |
| 第1个  | rdi   |
| 第2个  | rsi   |
| 第3个  | rdx   |
| 第4个  | rcx   |
| 第5个  | r8    |
| 第6个  | r9    |
| 第n个  | 栈     |

X86-64有16个64位寄存器，分别是：%rax，%rbx，%rcx，%rdx，%esi，%edi，%rbp，%rsp, %r8，%r9，%r10，%r11，%r12，%r13，%r14，%r15。其中

- %rax 作为函数返回值使用。
- %rsp 栈指针寄存器，指向栈顶
- %rdi，%rsi，%rdx，%rcx，%r8，%r9 用作函数参数，依次对应第1参数，第2参数。。。
- %rbx，%rbp，%r12，%r13，%14，%15 用作数据存储，遵循被调用者使用规则，简单说就是随便用，调用子函数之前要备份它，以防他被修改
- %r10，%r11 用作数据存储，遵循调用者使用规则，简单说就是使用之前要先保存原值

## got表

elf将got拆分为两个表，`.got`和`.got.plt`。其中`.got`用来保存全局变量的引用地址，`.got.plt`用来保存函数引用地址，对于外部函数引用全部放在`.got.plt`表中。