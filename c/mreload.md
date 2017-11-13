# 函数重载

示例

```cpp
#include <iostream>

int sum(int a, int b) {
    return a + b;
}

int sum(int a, int b, int c) {
    return a + b + c;
}

int main(int argc, const char * argv[]) {
    int s = sum(1, 2);
    printf("sum(1, 2) = %d\n", s);
    s = sum(1, 2, 3);
    printf("sum(1, 2, 3) = %d\n", s);
    
    return 0;
}
```

