# Aspects

项目地址 [https://github.com/steipete/Aspects](https://github.com/steipete/Aspects)

## 使用方法

1. 把[Aspects.h](https://github.com/steipete/Aspects/blob/master/Aspects.h)和[Aspects.m](https://github.com/steipete/Aspects/blob/master/Aspects.m)放到你的工程目录下

2. 编写你的代码去做一些hook操作
  1. 找到class
  2. 使用@selector找到hook的目标
  3. 使用下面的方法是hook目标方法

  ```objective-c
  + (id<AspectToken>)aspect_hookSelector:(SEL)selector
                             withOptions:(AspectOptions)options
                              usingBlock:(id)block
                                   error:(NSError **)error;
  ```

  ​


## 示例

定义一个SimpleTest类

###SimpleTest.h

```obj
#import <Foundation/Foundation.h>

@interface SimpleTest : NSObject

+ (int)t;

+ (int)add: (int)num1: (int)num2;

- (int)test;
- (int)test: (int)num1;
- (int)test: (int)num1: (int)num2;

@end

```

###SimpleTest.m

```objective-c
#import "SimpleTest.h"

@implementation SimpleTest

+ (int)t {
    return 1;
}

+ (int)add: (int)num1: (int)num2 {
    return num1 + num2;
}

- (int)test {
    return 1;
}
- (int)test: (int)num1 {
    return num1;
}
- (int)test: (int)num1: (int)num2 {
    return num1 + num2;
}

@end
```

###hook

```objective-c
void hook() {
// 类方法使用下面这种方法
//    Class simpleTest = objc_getMetaClass(NSStringFromClass(SimpleTest.class).UTF8String);
// 对象方法使用下面这种方法
//    Class simpleTest = NSClassFromString(@"SimpleTest");
    
    Class simpleTest = objc_getMetaClass(NSStringFromClass(SimpleTest.class).UTF8String);
    unsigned int mc = 0;
    Method * mlist = class_copyMethodList(simpleTest, &mc);
    NSLog(@"mc = %d", mc);
    for(int i = 0; i < mc; i++) {
        SEL selector = method_getName(mlist[i]);
        NSString * selectorName = NSStringFromSelector(selector);
        NSLog(@"find selector: %@", selectorName);
    }
    [simpleTest aspect_hookSelector:@selector(add::) withOptions:AspectPositionInstead usingBlock:^(id<AspectInfo> info){
        NSLog(@"add called: %@", [info arguments]);
        int ret;
        NSInvocation *invocation = info.originalInvocation;
        [invocation invoke];
        [invocation getReturnValue:&ret];
        NSLog(@"ret = %d", ret);
        int newret = 10;
        if(ret == 8) {
            [invocation setReturnValue:&newret];
        }
    } error:NULL];
}
```

### main.m

```objective-c
#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "SimpleTest.h"
#import <objc/runtime.h>
#import "Aspects.h"

void hook() {
// 类方法使用下面这种方法
//    Class simpleTest = objc_getMetaClass(NSStringFromClass(SimpleTest.class).UTF8String);
// 对象方法使用下面这种方法
//    Class simpleTest = NSClassFromString(@"SimpleTest");
    
    Class simpleTest = objc_getMetaClass(NSStringFromClass(SimpleTest.class).UTF8String);
    unsigned int mc = 0;
    Method * mlist = class_copyMethodList(simpleTest, &mc);
    NSLog(@"mc = %d", mc);
    for(int i = 0; i < mc; i++) {
        SEL selector = method_getName(mlist[i]);
        NSString * selectorName = NSStringFromSelector(selector);
        NSLog(@"find selector: %@", selectorName);
    }
    [simpleTest aspect_hookSelector:@selector(add::) withOptions:AspectPositionInstead usingBlock:^(id<AspectInfo> info){
        NSLog(@"add called: %@", [info arguments]);
        int ret;
        NSInvocation *invocation = info.originalInvocation;
        [invocation invoke];
        [invocation getReturnValue:&ret];
        NSLog(@"ret = %d", ret);
        int newret = 10;
        if(ret == 8) {
            [invocation setReturnValue:&newret];
        }
    } error:NULL];
}

int main(int argc, char * argv[]) {
    @autoreleasepool {
        int a = 3;
        int b = 5;
        int ret = [SimpleTest add:a :b];
        NSLog(@"before hook 3 + 5 = %d", ret);
        hook();
        ret = [SimpleTest add:a :b];
        NSLog(@"after hook 3 + 5 = %d", ret);
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
```

