# 代码注入

在macos下面hook大致分为以下几种：

1. [DYLD_INSERT_LIBRARIES](#DYLD_INSERT_LIBRARIES)。插入库hook方式，类似于linux平台的LD_PRELOAD方式，这种方式比较常见，除了可以hook常见的c/c++开发的程序库外，还可以hook系统库中的函数。
2. [SymbolTable Hook](#SymbolTable Hook)。符号表hook方式，类似于windows平台的IAT hook。
3. Method Swizzing。方法欺骗，这种hook方式是macos的objective runtime方式独有的，运用于objective-c/c++与swift的hook

## DYLD_INSERT_LIBRARIES

DYLD_INSERT_LIBRARIES是dyld使用的环境变量，使用该环境变量指定需要插入的动态库后，dyld在加载目标程序时会将指定插入的动态库的符号替换掉目标程序中的符号，这种机制就完成了程序中符号的hook。代码如下所示：

```c
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

int main(int argc, const char *argv[]) {
    int fd = open(argv[0], O_RDONLY);
    uint32_t magic = 0;
    read(fd, (void*)&magic, 4);
    printf("Mach-O Magic Number: %x\n", magic);
    return 0;
}
```

编译mian.c

```shell
cc mian.c -o app
```

运行`./app`得到如下结果

```shell
Mach-O Magic Number: feedfacf
```

编写hook.c

```c
#include <stdio.h>

#import <dlfcn.h>
#import <stdarg.h>
#import <stdio.h>
#import <stdlib.h>
#import <unistd.h>
#import <stdint.h>
#import <fcntl.h>
#import <string.h>

static int (*orig_open)(const char *, int, ...) = NULL;
static ssize_t (*orig_read)(int, void *, size_t) = NULL;
static int (*orig_close)(int) = NULL;

typedef int (*orig_open_type)(const char *, int, ...);
typedef ssize_t (*orig_read_type)(int, void *, size_t);
typedef int (*orig_close_type)(int);

__attribute__((constructor))
void init_funcs()
{
    printf("--------init funcs.--------\n");
    void * handle = dlopen("libSystem.dylib", RTLD_NOW);
    
    orig_open = (orig_open_type) dlsym(handle, "open");
    if(!orig_open) {
        printf("get open() addr error");
        exit(-1);
    }
    orig_read = (orig_read_type) dlsym(handle, "read");
    if(!orig_read) {
        printf("get open() addr error");
        exit(-1);
    }
    orig_close = (orig_close_type) dlsym(handle, "close");
    if(!orig_close) {
        printf("get open() addr error");
        exit(-1);
    }
    
    printf("--------init done--------\n");
}

int open(const char *path, int oflag, ...) {
    va_list ap = {0};
    mode_t mode = 0;
    
    if ((oflag & O_CREAT) != 0) {
        // mode only applies to O_CREAT
        va_start(ap, oflag);
        mode = va_arg(ap, int);
        va_end(ap);
        printf("Calling real open('%s', %d, %d)\n", path, oflag, mode);
        return orig_open(path, oflag, mode);
    } else {
        printf("Calling real open('%s', %d)\n", path, oflag);
        return orig_open(path, oflag, mode);
    }
}

ssize_t read(int fd, void *buf, size_t sz) {
    printf("Calling real read(%d)\n", fd);
    ssize_t sz_ = orig_read(fd, buf, sz);
    if (sz_ == sz) {
        memset(buf, 97, sz);
    }
    return sz_;
}

int close(int fd) {
    printf("Calling real close(%d)\n", fd);
    return orig_close(fd);
}

```

编译成动态库

```shell
cc -flat_namespace -dynamiclib -o libhook.dylib hook.c
```

设置环境变量运行

```shell
export DYLD_FORCE_FLAT_NAMESPACE=1
DYLD_INSERT_LIBRARIES=libhook.dylib ./app
```

运行结果为

```shell
--------init funcs.--------
--------init done--------
Calling real open('./app', 0)
Calling real read(3)
Mach-O Magic Number: 61616161
```

这个程序首先构造了一个初始化的方法`init_funcs`，通过为方法添加预处理指令`__attribute__((constructor))`可以指定该方法为初始化方法，初始化方法会在动态库加载的时候最先被调用，这里这个方法的作用是获取原来三个系统函数的地址，并且保存下来使用。接着在代码里面实现自己要hook的函数的内容，与原函数名称一样。

## SymbolTable Hook

符号表hook，通过对目标程序的符号表做手脚达到hook的目的，Mach-O的程序中的符号分为两种，一种是直接在动态链接程序的时候就需要绑定的符号non-lazily symol，即非延迟绑定的符号、他保存在`__DATA`段中的`__nl_symbol_ptr`节区中。另一种是程序运行后第一次调用才会绑定的符号`lazily symbol`，即延迟绑定的符号，他保存在`__DATA`段中的`__la_symbol_ptr`节区中。延迟绑定符号的绑定操作是在dyld在加载程序时通过`dyld_stub_binder`完成的，这两张表都保存了符号的名称与内存中的地址，符号表hook的原理就是在镜像加载绑定符号时，修改符号表指向的内存地址，通过这种方式来实现hook。

通过开源库[https://github.com/facebook/fishhook](https://github.com/facebook/fishhook)来进行符号表hook。

下面列子是hook `getpid()`的一个例子

```c
#import "fishhook.h"
#import <unistd.h>
#import <stdio.h>

pid_t (*orig_getpid)() = NULL;

pid_t new_getpid() {
	return (*orig_getpid)() + 1;
}

int main(int argc, char *argv[]) {
	pid_t pid = getpid();
	printf("before hook pid = %d\n", pid);
    struct rebinding rebindings[1];
    rebindings[0].name = "getpid";
    rebindings[0].replacement = new_getpid;
    rebindings[0].replaced = (void*)&orig_getpid;
	rebind_symbols(rebindings, 1);
    pid = getpid();
    printf("after hook pid = %d\n", pid);
	return 0;
}

```

