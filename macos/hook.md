# 代码注入

在macos下面hook大致分为以下几种：

1. [DYLD_INSERT_LIBRARIES](#DYLD_INSERT_LIBRARIES)。插入库hook方式，类似于linux平台的LD_PRELOAD方式，这种方式比较常见，除了可以hook常见的c/c++开发的程序库外，还可以hook系统库中的函数。
2. SymbolTable Hook。符号表hook方式，类似于windows平台的IAT hook。
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

