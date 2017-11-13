# DTrace

## 示例

```
sudo dtrace -n 'proc::posix_spawn:exec-success { printf("execname: %s", execname); }'
```

这个示例是打开一个应用，输出应用的名字

## D语言脚本传递方式

- **DTrace直接执行**。通过-n参数传递给dtrace来执行。
- **DTrace调用脚本执行**。把脚本写入文件中，用`dtrace -s <script path>`来执行。
- **作为脚本文件直接执行**。本质就是第二种。

```dtrace
#!/usr/sbin/dtrace -s

proc::posix_spawn:exec-success {
  printf("execname: %s", execname);
}
```

## D语言语法

> D语言没有函数，d语言代码一般是由一个或者多个探测器子句组成，在探测器子句外卖只可以有类型定义或者声明语句，不可以有定义变量火执行操作的语句。

### 探测器说明语句

探测器是dtrace内核模块提供的一些监控系统的接入点，当操作系统中程序