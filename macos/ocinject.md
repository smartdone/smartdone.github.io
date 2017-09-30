# 注入oc动态库

## 所需命令

```Shell
# 编译oc可执行文件
clang -fobjc-arc -framework Foundation test.m -o test
# 编译oc动态库
clang -fobjc-arc -shared -fPIC -framework Foundation test.m -o test.dylib
# 使用optool注入动态库
optool install -c load -p "@executable_path/xxx.dylib" -t /xxx.app/xxx
```

## 测试代码

```objective-c
#import <Foundation/Foundation.h>
#import "SimpleTest.h"

int main(int argc, char *argv[]) {
	@autoreleasepool {
		int a = 3;
		int b = 5;
		int ret = [SimpleTest add:a :b];
		NSLog(@"a + b = %d", ret);
		NSLog(@"hello wrold");
	}
	return 0;
}
```

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

编译命令

```
clang main.m SimpleTest.m -I. -fobjc-arc -framework Foundation -o test
```



## 注入的代码

```objective-c
#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import "Aspects.h"

__attribute__((constructor)) void before_load() {
	NSLog(@"load dylib");
	Class simpleTest = objc_getMetaClass([@"SimpleTest" UTF8String]);
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

编译命令

```
clang Aspects.m hook.m -I. -fobjc-arc -shared -fPIC -framework objc -o hook.dylib
```

注入命令

```
optool install -c load -p "@executable_path/hook.dylib" -t test
```

