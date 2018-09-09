# Writing an LLVM Pass

> 基于llvm7.0源码
>
>  (http://llvm.org/git/clang.git 284236c047631c8b0eabac3ddd3d0c95253f4361  
>
> http://llvm.org/git/llvm.git 01fcdec33ed56f14f51fc1aff3fb73b8e7b41e2a

## 前期准备

为了方便我们修改代码，我们用clion导入llvm工程，导入工程的时候选择导入已有工程。

## 写一个hello world

### 设置编译环境

pass插件放到`lib/Transforms`下面的，在这下面创建一个名字叫`Hello`的文件夹，然后添加一个`CMakeLists.txt`的文件，内容如下：

```cmake
add_llvm_loadable_module( LLVMHello
  Hello.cpp

  PLUGIN_TOOL
  opt
  )
```

然后修改`lib/Transforms/CMakeLists.txt`，添加下面这一句：

```cmake
add_subdirectory(Hello)
```

> llvm的源码里面已经包含了这个例子，如果你想要自己来实践这些操作，那你可以创建一个叫其他名字的文件夹

编译脚本会把这个目录下面的`Hello.cpp`编译并且链接到动态库`lib/LLVMHello.so`中。这个可以动态的被`opt`工具使用`-load`操作加载。

### 必须的代码

假如我们只需要写一个`Pass`，然后我们不需要去操作他的`Function`，我们不需要打印什么东西，我们需要导入下面这个头文件:

```c++
#include "llvm/Pass.h"http://llvm.org/git/llvm.git 01fcdec33ed56f14f51fc1aff3fb73b8e7b41e2a
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
```

我们需要引用命名空间:

```c++
using namespace llvm;
```

我们定义一个`Hello`的类作为`FunctionPass`的子类，不同的[子类](http://llvm.org/docs/WritingAnLLVMPass.html#writing-an-llvm-pass-pass-classes)有不同的功能，我们现在只需要知道操作function需要`FunctionPass`

```c++
struct Hello : public FunctionPass 
```

我们定义一个`ID`去标识pass

```c++
static char ID;
Hello() : FunctionPass(ID) {}
```

llvm我们定义一个`runOnFunction`方法去重写在`FunctionPass`里面的虚方法。在这里面将或做一些我们需要实现的业务逻辑，但是在这个地方我们就仅仅输出一些信息来。

```c++
  bool runOnFunction(Function &F) override {
    errs() << "Hello: ";
    errs().write_escaped(F.getName()) << '\n';
    return false;
  }
}; // end of struct Hello
}  // end of anonymous namespace
```

我们去初始化pass ID,llvm使用id去标识一个pass，所以初始化的值是多少不重要。

```c++
char Hello::ID =llvm 0;
```

最后我们注册我们的类`Hello`，给他的命令行参数为`hello`，然后给他命名为`Hello World Pass`，最后两个参数是用来描述他的行为。

完整的代码如下：

```c++
//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "hello"

STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct Hello : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    Hello() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++HelloCounter;
      errs() << "Hello: ";
      errs().write_escaped(F.getName()) << '\n';
      return false;
    }
  };
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass");

namespace {
  // Hello2 - The second implementation with getAnalysisUsage implemented.
  struct Hello2 : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    Hello2() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++HelloCounter;
      errs() << "Hello: ";
      errs().write_escaped(F.getName()) << '\n';
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }
  };
}

char Hello2::ID = 0;
static RegisterPass<Hello2>
Y("hello2", "Hello World Pass (with getAnalysisUsage implemented)");

```

现在我们使用make去编译整个工程，然后应该会编译出来一个`lib/LLVMHello.so`的文件。

### 使用opt来启用pass

写一个测试的源文件

```c++
#include <stdio.h>

static char * test = "global variable";

int main(int argc, char *argv[]) {
	printf("hello world");
	return 0;
}
```

使用下面的命令生产bc文件

`clang -emit-llvm test.c -c -o test.bc`

然后使用opt命令来启用这个pass

`opt -load lib/LLVMHello.so -hello -time-passes < test.bc > /dev/null`

## 跟着张总使用LLVM做字符串混淆

参考：[自己动手实现基于llvm的字符串加密](http://iosre.com/t/llvm/10610)

文中只说了实现的方式，没有写具体的步骤，一个刚开始学llvm的可能并不知道文件应该放到哪里，编译之后文件放到哪里的，我们就按新手能理解的来一步一部学习使用llvm做字符串混淆。

