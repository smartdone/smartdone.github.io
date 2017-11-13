# 函数模板

## 模板

比如说交换两个值，交换int，double等原理代码都是差不多的，就是数据类型不一样，手工复制修改很浪费时间，并且容易出错。函数模板可以自动完成这一过程，节省时间并且更加可靠。

示例：

```cpp
#include <iostream>

template <typename T>
void swap(T &a, T &b) {
    T temp;
    temp = a;
    a = b;
    b = temp;
}


int main(int argc, const char * argv[]) {
    int a = 1;
    int b = 2;
    printf("a = %d, b = %d\n", a, b);
    swap(a, b);
    printf("a = %d, b = %d\n", a, b);
    
    double c = 1.2;
    double d = 2.2;
    printf("c = %lf, d = %lf\n", c, d);
    swap(c, d);
    printf("c = %lf, d = %lf\n", c, d);
    
    return 0;
}
```

## 模板重载

和函数重载类似

例子

```cpp
#include <iostream>

template <typename T>
T sum(T a, T b) {
    return a + b;
}

template <typename T>
T sum(T a, T b, T c) {
    return a + b + c;
}


int main(int argc, const char * argv[]) {
    printf("sum(1, 2) = %d\n", sum(1, 2));
    printf("sum(1.1, 2.2, 3.3) = %lf\n", sum(1.1, 2.2, 3.3));
    return 0;
}
```

