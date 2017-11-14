# 类

类的定义和方法的实现

例子

```cpp
#ifndef Hello_hpp
#define Hello_hpp

#include <string>

class Hello {
private:
    std::string name;
    int age;
public:
    void hello();
    void setage(int age);
    void setname(std::string name);
    Hello();
    ~Hello();
};

#endif /* Hello_hpp */
```

```cpp
#include "Hello.hpp"

void Hello::hello() {
    printf("hello, %s\n", name.c_str());
}

void Hello::setage(int _age) {
    age = _age;
}

void Hello::setname(std::string _name) {
    name = _name;
}

Hello::Hello() {
    printf("constructor\n");
}

Hello::~Hello() {
    printf("delete\n");
}
```

```cpp
#include <iostream>
#include "Hello.hpp"

int main(int argc, const char * argv[]) {
    Hello *hello = new Hello();
    hello->setage(10);
    hello->setname("xxxxxxx");
    hello->hello();
    printf("will be delete\n");
    delete hello;
    return 0;
}
```

## 运算符重载

```cpp
operator+ 
operator-
operator*
operator/
```

运算符重载的限制

1. 重载后的运算符必须至少有一个操作上是用户定义的类型，这将防止用户为标准类型重载运算符。隐藏，不能将减法运算符重载为计算两个值得和，而不是他们的差。

2. 使用运算符时不能违反运算符原来的句法规则，例如不能将求模运算符重载成使用一个操作数。同样不能改变运算符的优先级。

3. 不能创建新运算符，如`operator**`来表示平方。

4. 不能重载下面的运算符：

   - sizeof
   - . : 成员运算符
   - .* : 成员指针运算符
   - :: :作用域解析运算符
   - ?: :条件运算符
   - typeid :一个RTTI运算符
   - const_cast :强制类型转换运算符
   - dynamic_cast :强制类型转换运算符
   - reinterpret_cast :强制类型转换运算符
   - static_cast :强制类型转换运算符

5. 大多数运算符重载都可以使用成员火非成员函数进行重载，但是下面的只能通过成员函数进行重载

   - = :赋值运算符
   - () :函数调用运算符
   - [] :下标运算符
   - -> :通过指针访问类成员运算符
