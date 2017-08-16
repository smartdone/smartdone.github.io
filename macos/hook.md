# 代码注入

在macos下面hook大致分为以下几种：

1. DYLD_INSERT_LIBRARIES。插入库hook方式，类似于linux平台的LD_PRELOAD方式，这种方式比较常见，除了可以hook常见的c/c++开发的程序库外，还可以hook系统库中的函数。
2. SymbolTable Hook。符号表hook方式，类似于windows平台的IAT hook。
3. Method Swizzing。方法欺骗，这种hook方式是macos的objective runtime方式独有的，运用于objective-c/c++与swift的hook

