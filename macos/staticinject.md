# 静态代码注入

> 使用optool进行代码注入

先写一个简单的例子

```c
#import <stdio.h>
#import <string.h>

int crack(const char *str) {
	char *ss = "123456";
	if(strcmp(str, ss) == 0) {
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[] ) {
	char in[10];
	printf("password:\n");
	scanf("%s", in);
	if(crack(in) == 1) {
		printf("success\n");
	}else{
		printf("failed\n");
	}
	return 0;
}
```

然后在写注入代码

```C
#import <stdio.h>

__attribute__((constructor))
void hook_init() {
	printf("hook init\n");
}

```

将这段hook的代码编译成动态库

```shell
cc -dynamiclib -o libinsertdylib.dlib insertdylib.c
```

使用optool注入动态库

```shell
optool install -p libinsertdylib.dlib -c load -t test -o test_patched
```

修改注入后的可执行文件的权限

```
chmod a+x test_patched
```

运行

```
DYLD_LIBRARY_PATH=. ./test_patched
```

