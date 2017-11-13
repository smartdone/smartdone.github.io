- 调试app

`lldb xxx`

- 设置断点

```txt
breakpoint set --name '-[xxx xx]'
或者
b -[xxx xx]
```

- 查看调用堆栈

```
thread backtrace
或者
bt
```

- 反汇编

```
disassemble --name 'xxxx'
或者
di -n 'xxxx'
```

默认情况下，lldb使用的是AT&T格式的反汇编。可以说使用下面的命令让调试器显示Intel格式的反汇编

```
settings set target.x86-disassembly-flavor intel
```

- 读取寄存器

```
register read <寄存器名>
或者
re r <寄存器名>
```

- 写寄存器

```
register write <寄存器> <值>
或者
re w <寄存器> <值>
```

- 显示所有寄存器的值

```
register read --all
或者
re r -a
```

- 调用框架方法

```
po [xxx xx]
```

