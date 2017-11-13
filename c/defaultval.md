# 默认参数

```c
int add(int a, int b =1);
```

如果传递参数的时候只传递了第一个值，第二个就会默认是1。默认参数必须从右至左设置，不能够左边设置了值右边却没有设置。

例子

```cpp
#include <iostream>÷

int add(int a, int b = 1);

int add(int a, int b) {
    return a + b;
}

int main(int argc, const char * argv[]) {
    int sum = add(2);
    printf("sum = %d\n", sum);
    return 0;
}

```

