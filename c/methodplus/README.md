一些C++函数的新特新

- 内联函数 [inline.cpp](./inline.cpp)

> 内敛函数是C++为了提高程序运行效率所做的一项改进，常规函数和内联函数之间的主要区别不在于编写方式，而是在于C++编译器如何将他们组合到程序中。要使用这个特性就是在函数的定义和声明的时候都加上关键字inline

- 引用变量 [firstref.cpp](./firstref.cpp) [sceref.cpp](./sceref.cpp) [swaps.cpp](./swaps.cpp)

> C++中新增加了一种复合类型-引用变量。引用是一定要的变量的一个别名。例如将ctwain作为clement变量的应用，则可以交替使用twain和clement来 表示该变量。引用变量的主要用途是用作函数的形参。通过将引用变量用作参数，函数将使用原始数据，而不是其副本。这样除指针之外，引用也为函数处理大型结构提供了一种非常方便的途径，同时对于设计类来说，引用也是必不可少的。

- 如何按引用传递函数参数
- 默认参数
- 函数重载
- 函数模板
- 函数模板具体化